/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:25 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/13 19:59:35 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "Client.hpp"

class Server
{
private:
	std::string _ip; // usually 0.0.0.0
	int	_port;
	int	_fd; // server's socket fd

	std::map<std::string, Channel> _channels;
	std::map<int, Client> _clients;
public:
	Server(/* args */);
	~Server();
};

Server::Server()
{
}
Server::~Server()
{
}

#endif
