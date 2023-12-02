/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 17:25:17 by soma              #+#    #+#             */
/*   Updated: 2023/12/02 17:47:45 by soma             ###   ########.fr       */
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
				if (FD_ISSET())
		}
	}
}