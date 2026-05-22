/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:08 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/21 21:12:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <set>
# include "Channel.hpp"

class Client
{
private:
	std::string	_username;
	std::string	_nickname;
	std::string	_realname;

	bool	_authenticated;
	bool	_registered;

	std::set<Channel*> _channels; // stores channels pointers, maximum 10 channels per user

	std::string	_ip;
	int	_port;

	int	_socket_fd; // client's socket fd

	std::string	_buffer;
public:
	Client(std::string ip, std::string port, int fd, std::string buffer);
	~Client();
	int	getClientFd();
};

#endif
