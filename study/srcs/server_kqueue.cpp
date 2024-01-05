/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_kqueue.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 10:11:08 by soma              #+#    #+#             */
/*   Updated: 2024/01/05 18:33:32 by soma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

// クローズする最大ディスクリプタ値
#define	MAXFD	64
// 最大同時処理数
#define	MAX_CHILD	20
// コマンドライン引数、環境変数のアドレス保持用
int *argc_;
char ***argv_;
char ***envp_;

// シグナルハンドラ
void	sig_hangup_handler(int sig) {
	(void) fprintf(stderr, "sig_hangup_handler(%d)\n", sig);
	// stdin,stdout,stderr以外をクローズ
	for (int i = 3; i < MAXFD; i++) {
		(void) close(i);
	}
	// 自プロセスの上書き
	if (execve((*argv_)[0], (*argv_), (*envp_)) == -1) {
		perror("execve");
		exit(1);
	}
}

// サーバーソケットの準備
int		server_socket(const char *portnm) {
	char	nbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	struct	addrinfo hints, *res0;
	int		soc, opt, errcode;
	socklen_t	opt_len;

	// アドレス情報のヒントをゼロクリア
	(void) memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	// ソケットがbind()を利用される予定であることを示す
	hints.ai_flags = AI_PASSIVE;
	// アドレス情報の決定
	if ((errcode = getaddrinfo(NULL, portnm, &hints, &res0) != 0)) {
		(void) fprintf(stderr, "getaddrinfo():%s\n", gai_strerror(errcode));
		return (-1);
	}
	if ((errcode = getnameinfo(res0->ai_addr, res0->ai_addrlen,
								nbuf, sizeof(nbuf),
								sbuf, sizeof(sbuf),
								NI_NUMERICHOST | NI_NUMERICSERV) != 0)) {
		(void) fprintf(stderr, "getnameinfo():%s\n", gai_strerror(errcode));
		freeaddrinfo(res0);
		return(1);
	}
	(void) fprintf(stderr, "addr=%s\nport=%s\n", nbuf, sbuf);
	// ソケットの生成
	if ((soc = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol)) == 1) {
		perror("socket");
		freeaddrinfo(res0);
		return (-1);
	}
	// ソケットオプション（再利用フラグ）設定
	opt = 1;
	opt_len = sizeof(opt);
	if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len) == -1) {
		perror("socket");
		(void) close(soc);
		freeaddrinfo(res0);
		return (-1);
	}
	// ソケットにアドレスを指定
	if (bind(soc, res0->ai_addr, res0->ai_addrlen) == -1) {
		perror("bind");
		(void) close(soc);
		freeaddrinfo(res0);
		return (-1);
	}
	// アクセスバックログの指定
	if (listen(soc, SOMAXCONN) == -1) {
		perror("listen");
		(void) close(soc);
		freeaddrinfo(res0);
		return (-1);
	}
	freeaddrinfo(res0);
	return (soc);
}

void accept_loop(int soc) {
    int child[MAX_CHILD];
    struct kevent events[MAX_CHILD + 1];
    struct timespec timeout = { 10, 0 };  // 10 seconds timeout
    int child_no = 0;
    int i, ret;

    // Initialize child array
    for (i = 0; i < MAX_CHILD; i++) {
        child[i] = -1;
    }

    // Create a new kqueue
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        return;
    }

    // Register the server socket for read events
    struct kevent event;
    EV_SET(&event, soc, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        close(kq);
        return;
    }

    for (;;) {
        // Wait for events using kqueue
        int num_events = kevent(kq, NULL, 0, events, MAX_CHILD + 1, &timeout);
        if (num_events == -1) {
            perror("kevent");
            break;
        } else if (num_events == 0) {
            // Timeout
            continue;
        }

        // Check for events
        for (i = 0; i < num_events; i++) {
            if ((int)events[i].ident == soc) {
                // Server socket has a read event (new connection)
                struct sockaddr_storage from;
                socklen_t len = sizeof(from);

                // Accept the new connection
                int acc = accept(soc, (struct sockaddr*)&from, &len);
                if (acc == -1) {
                    if (errno != EINTR) {
                        perror("accept");
                    }
                } else {
                    // Handle the new connection
                    fprintf(stderr, "New connection accepted.\n");
                    
                    // Find an empty slot in the child array
                    int pos = -1;
                    for (int j = 0; j < MAX_CHILD; j++) {
                        if (child[j] == -1) {
                            pos = j;
                            break;
                        }
                    }

                    if (pos == -1) {
                        // No empty slot found
                        if (child_no + 1 >= MAX_CHILD) {
                            fprintf(stderr, "child is full: cannot accept\n");
                            close(acc);
                        } else {
                            child_no++;
                            pos = child_no - 1;
                        }
                    }

                    if (pos != -1) {
                        // Store the new connection in the child array
                        child[pos] = acc;
                        
                        // Register the new child socket for read events
                        EV_SET(&event, acc, EVFILT_READ, EV_ADD, 0, 0, NULL);
                        if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
                            perror("kevent");
                            close(kq);
                            return;
                        }
                    }
                }
            } else {
                // One of the child sockets has a read event
                int child_index = -1;

                // Find the child socket in the child array
                for (int j = 0; j < MAX_CHILD; j++) {
                    if (child[j] == (int)events[i].ident) {
                        child_index = j;
                        break;
                    }
                }

                if (child_index != -1) {
                    // Handle send/recv for the corresponding child
                    ret = send_recv(child[child_index], child_index);
                    if (ret == -1) {
                        // Error or disconnection, close the child socket
                        close(child[child_index]);
                        child[child_index] = -1;
                    }
                }
            }
        }
    }

    // Close the kqueue
    close(kq);
}

// サイズ指定文字列連結
size_t	mystrlcat(char *dst, const char *src, size_t size) {
	const char	*ps;
	char		*pd, *pde;
	size_t		dlen, lest;
	
	for (pd = dst, lest = size; *pd != '\0' && lest != 0; pd++, lest--);
	dlen = pd - dst;
	if (size - dlen == 0) {
		return (dlen + strlen(src));
	}
	pde = dst + size -1;
	for (ps = src; *ps != '\0' && pd < pde; pd++, ps++){
		*pd = *ps;
	}
	for (; pd <= pde; pd++) {
		*pd = '\0';
	}
	while (*ps++);
	return (dlen + (ps - src - 1));
}

// 送受信ループ
int	send_recv(int acc, int child_no) {
	char	buf[512], *ptr;
	ssize_t	len;
	
		// 受信
		if ((len = recv(acc, buf, sizeof(buf), 0)) == -1) {
			// エラー
			perror("recv");
			return (-1);
		}
		if (len == 0) { 
			// エンド・オブ・ファイル
			(void) fprintf(stderr, "[child%d]recv:EOF\n", child_no);
			return (-1);
		}
		// 文字列化・表示
		buf[len] = '\0';
		if ((ptr = strpbrk(buf, "\r\n")) != NULL) {
			*ptr = '\0';
		}
		(void) fprintf(stderr, "[child%d]%s\n", child_no, buf);
		// 応答文字列作成
		(void) mystrlcat(buf, ":OK\r\n", sizeof(buf));
		len = (ssize_t) strlen(buf);
		// 応答
		if ((len = send(acc, buf, (size_t) len, 0)) == -1) {
			// エラー
			perror("send");
			return (-1);
		}	
	return (0);
}

int main(int args, char *argv[], char *envp[]) {
	int soc;
	struct sigaction	sa;
	// 引数にポート番号が指定されているか？
	if (args <= 1) {
		(void) fprintf(stderr, "server address port\n");
		return (EXIT_FAILURE);
	}
	argc_ = &args;
	argv_ = &argv;
	envp_ = &envp;
	
	// シグナルハンドラ設定
	sigaction(SIGHUP, (struct sigaction *) NULL, &sa);
	sa.sa_handler = sig_hangup_handler;
	sa.sa_flags = SA_NODEFER;
	sigaction(SIGHUP, &sa, (struct sigaction *) NULL);
	fprintf(stderr, "sigaction():end\n");
	// サーバーソケットの準備
	if ((soc = server_socket(argv[1])) == -1) {
		(void) fprintf(stderr, "server_socket(%s):error\n", argv[1]);
		return (EXIT_FAILURE);
	}
	(void) fprintf(stderr, "ready for accept\n");
	// アクセプトループ
	accept_loop(soc);
	// ソケットクローズ
	(void) close(soc);
	return (EX_OK);
}