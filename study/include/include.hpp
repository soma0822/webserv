/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soma <soma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 09:54:57 by soma              #+#    #+#             */
/*   Updated: 2023/12/03 16:49:20 by soma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_HPP
# define INCLUDE_HPP

# include <sys/param.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

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

void	send_recv_loop(int acc);

#endif