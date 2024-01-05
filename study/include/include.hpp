/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 09:54:57 by soma              #+#    #+#             */
/*   Updated: 2024/01/05 17:48:33 by soma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_HPP
# define INCLUDE_HPP

# include <sys/param.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
#include <sys/event.h>
#include <sys/time.h>

# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>

# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sysexits.h>
# include <syslog.h>
# include <unistd.h>

int	send_recv(int acc, int child_no);

#endif