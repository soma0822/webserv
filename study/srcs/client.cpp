/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 17:25:17 by soma              #+#    #+#             */
/*   Updated: 2023/12/02 18:36:10 by soma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

// サーバーソケットに接続
int	client_socket(const char *hostnm, const char *portnm) {
	char nbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	struct addrinfo hints, *res0;
	int soc, errcode;

	// アドレス情報のヒントをゼロクリア
	(void) memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	// アドレス情報の決定
	if ((errcode = getaddrinfo(hostnm, portnm, &hints, &res0)) != 0) {
		(void) fprintf(stderr, "fetaddrinfo():%s\n", gai_strerror(errcode));
		return (-1);
	}
	if ((errcode = getnameinfo(res0->ai_addr, res0->ai_addrlen,
								nbuf, sizeof(nbuf),
								sbuf, sizeof(sbuf),
								NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
		(void) fprintf(stderr, "getnameinfo():%s\n", gai_strerror(errcode));
		freeaddrinfo(res0);
		return (-1);							
	}
	(void) fprintf(stderr, "addr=%s\n", nbuf);
	(void) fprintf(stderr, "port=%s\n", sbuf);
	// ソケットの生成
	if ((soc = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol)) == -1) {
		perror("socket");
		freeaddrinfo(res0);
		return (-1);
	}
	// コネクト
	if (connect(soc, res0->ai_addr, res0->ai_addrlen) == -1) {
		perror("connect");
		(void) close(soc);
		freeaddrinfo(res0);
		return (-1);
	}
	freeaddrinfo(res0);
	return (soc);
}

void	send_recv_loop(int soc) {
	char buf[512];
	struct timeval	timeout;
	int end, width;
	ssize_t	len;
	fd_set mask, ready;
	// select()用マスク
	FD_ZERO(&mask);
	// ソケットディスクリプタをセット
	FD_SET(soc, &mask);
	// 標準入力をセット
	FD_SET(0, &mask);
	width = soc +1;
	// 送受信
	for (end = 0;; ) {
		// マスクの代入
		ready = mask;
		// タイムアウト値のセット
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		switch (select(width, (fd_set *) &ready, NULL, NULL, &timeout)) {
			case -1:
				// エラー
				perror("select");
				break;
			case 0:
				// タイムアウト
				break;
			default:
				// レディ有り
				// ソケットレディ
				if (FD_ISSET(soc, &ready)) {
					// 受信
					if ((len = recv(soc, buf, sizeof(buf), 0)) == -1) {
						// エラー
						perror("recv");
						end = 1;
						break;
					}

					if (len == 0) {
						// エンド・オブ・ファイル
						(void) fprintf(stderr, "recv:EOF\n");
						end = 1;
						break;
					}
					// 文字列化・表示
					buf[len] = '\0';
					(void) fprintf(stderr, "[server]%s\n", buf);
				}
				// 標準入力レディ
				if (FD_ISSET(0, &ready)) {
					// 標準入力から１行読み込み
					(void) fgets(buf, sizeof(buf), stdin);
					if (feof(stdin)) {
						end = 1;
						break;
					}
					// 送信
					if ((len = send(soc, buf, strlen(buf), 0)) == -1) {
						// エラー
						perror("send");
						end = 1;
						break;
					}
				}
				break;
		}
		if (end) {
			break;
		}
	}
}

int main(int argc, char *argv[]) {
	int soc;
	
	// 引数にホスト名とポート番号が指定されているか？
	if (argc <= 2) {
		(void) fprintf(stderr, "client server-host port\n");
		return (EX_USAGE);
	}
	
	// サーバーソケットに接続
	if ((soc = client_socket(argv[1], argv[2])) == -1) {
		(void) fprintf(stderr, "client_socket():error\n");
		return (EXIT_FAILURE);
	}
	// 送受信ループ
	send_recv_loop(soc);
	// ソケットクローズ
	(void) close(soc);
	return (EXIT_SUCCESS);
}