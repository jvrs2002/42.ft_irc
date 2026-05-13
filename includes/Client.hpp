/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:08 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/13 19:59:44 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Channel.hpp"

class Client
{
private:
	std::string	_username;
	std::string	_nickname;
	std::string	_realname;

	bool	_authenticated;
	bool	_registered;

	std::vector<std::string> _channels; // stores only channels names

	std::string	_ip;
	int	_port;

	int	_fd; // client's socket fd

	std::string	_buffer;
public:
	Client(/* args */);
	~Client();
};

Client::Client()
{
}

Client::~Client()
{
}

#endif
