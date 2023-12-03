/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 15:23:53 by soma              #+#    #+#             */
/*   Updated: 2023/12/03 16:54:23 by soma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

// クローズする最大ディスクリプタ値
#define MAXFD 64
// デーモン化
int	daemonize(int nochdir, int noclose)
{
	int		i,fd;
	pid_t	pid;
	// 親プロセスを終了させる
	if ((pid = fork()) < 0)
		return (-1);
	else if (pid != 0)
		exit(0);
	// セッションリーダーになる
	setsid();
	// シグナルハンドラを初期化する
	signal(SIGHUP, SIG_IGN);
	// フォークして孫プロセスを作る
	if ((pid = fork()) < 0)
		return (-1);
	else if (pid != 0)
		exit(0);
	// ルートディレクトリ以外で作業しない
	if (!nochdir)
		chdir("/");
	if (noclose == 0) {
		// 全てのファイルディスクリプタをクローズする
		for (i = 0; i < MAXFD; i++)
			close(i);
	// 標準入出力、標準エラー出力を/dev/nullにつなぐ
		if ((fd = open("/dev/null", O_RDWR, 0) != -1)) {
			dup2(fd, STDIN_FILENO);
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			if (fd > 2) {
				close(fd);
			}
		}
	}
	return (0);
}

#ifdef UNIT_TEST

int main(void)
{
	char	buf[256];
	
	// デーモン化
	daemonize(0, 0);
	// ディスクリプタークローズのチェック
	fprintf(stderr, "stderr\n");
	syslog(LOG_USER | LOG_NOTICE, "daemon:cwd=%s\n", getcwd(buf, sizeof(buf)));
	return (EX_OK);
}
#endif