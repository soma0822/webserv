/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaltest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 17:06:46 by soma              #+#    #+#             */
/*   Updated: 2023/12/03 18:26:04 by soma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

int *argc_;
char ***argv_;
char ***envp_;

void	sig_hangup_handler(int sig)
{
	(void) fprintf(stderr, "sig_hangup_handler(%d)\n", sig);
	// 自プロセスの上書き
	if (execve((*argv_)[0], (*argv_), (*envp_)) == -1) {
		perror("execve");
		exit(1);
	}
}

int main(int argc, char *argv[], char *envp[])
{
	int		i;
	struct sigaction	sa;

	argc_ = &argc;
	argv_ = &argv;
	envp_ = &envp;
	// コマンドライン引数、環境変数の表示
	fprintf(stderr, "start pid=%d\n", getpid());
	fprintf(stderr, "argc=%d\n", argc);
	for (i = 0; i < argc; i++)
		fprintf(stderr, "argv[%d]=%s\n", i, argv[i]);
	for (i = 0; envp[i] != NULL; i++)
		fprintf(stderr, "envp[%d]=%s\n", i, envp[i]);
	// sleep()によるSIGALRMを解除:Linuxでは行わなくても問題ないが移植性のため
	signal(SIGALRM, SIG_IGN);
	// SIGHUPのシグナルハンドラを設定
#ifdef USE_SIGNAL
	signal(SIGHUP, sig_hangup_handler);
	sigaction(SIGHUP, (struct sigaction *)NULL, &sa);
	fprintf(stderr, "SA_ONSTACK=%d\n", (sa.sa_flags & SA_ONSTACK)?1:0);
	fprintf(stderr, "SA_RESETHAND=%d\n", (sa.sa_flags & SA_RESETHAND)?1:0);
	fprintf(stderr, "SA_NODEFER=%d\n", (sa.sa_flags & SA_NODEFER)?1:0);
	fprintf(stderr, "SA_RESTART=%d\n", (sa.sa_flags & SA_RESTART)?1:0);
	fprintf(stderr, "SA_SIGINFO=%d\n", (sa.sa_flags & SA_SIGINFO)?1:0);
	fprintf(stderr, "signal():end\n");
#else
	sigaction(SIGHUP, (struct sigaction *)NULL, &sa);
	sa.sa_handler = sig_hangup_handler;
	sa.sa_flags = SA_NODEFER;
	sigaction(SIGHUP, &sa, (struct sigaction *)NULL);
	// 現状の表示
	sigaction(SIGHUP, (struct sigaction *)NULL, &sa);
	fprintf(stderr, "sigaction():end\n");
	fprintf(stderr, "SA_ONSTACK=%d\n", (sa.sa_flags & SA_ONSTACK)?1:0);
	fprintf(stderr, "SA_RESETHAND=%d\n", (sa.sa_flags & SA_RESETHAND)?1:0);
	fprintf(stderr, "SA_NODEFER=%d\n", (sa.sa_flags & SA_NODEFER)?1:0);
	fprintf(stderr, "SA_RESTART=%d\n", (sa.sa_flags & SA_RESTART)?1:0);
	fprintf(stderr, "SA_SIGINFO=%d\n", (sa.sa_flags & SA_SIGINFO)?1:0);
#endif
	// ５秒おきにカウント表示
	for (i = 0;; i++) {
		(void) fprintf(stderr, "count=%d\n", i);
		(void) sleep(5);
	}
}