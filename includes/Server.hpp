/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:25 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/18 21:16:06 by joao-vri         ###   ########.fr       */
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
#include "Commands.hpp"

class Server
{
private:
	std::string	_ip; // usually 0.0.0.0
	int	_port;
	std::string _password;
	int	_socket_fd; // server's socket fd

	Commands	command_handler;
	std::map<std::string, Channel>	_channels; // each channel is mapped by their name
	std::map<int, Client>	_clients; // each client is mapped by it's socket fd
public:
	Server(std::string ip, int port, std::string password);
	~Server();
	Client*		create_user(int fd, const std::string& ip, int port);
	void		delete_user(Client *user);
};

#endif
