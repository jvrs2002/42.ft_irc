/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:29 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/29 20:55:57 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string ip, std::string port, std::string password) :
	_ip(ip),
	_port(port), 
	_password(password),
	_socket_fd(-1),
	_BACKLOG(10),
	NAME("irc.ft_irc.net")
{
	Server::initServer();
}
Server::~Server()
{
	if (_socket_fd != -1)
		close(_socket_fd);
}

void	Server::initServer()
{
	int	status;
	struct addrinfo	hints;
	struct addrinfo	*serv_info;

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	status = getaddrinfo(NULL, _port.c_str(), &hints, &serv_info);

	if (status != 0){
		std::cerr << "getaddrinfo() error." << std::endl;
		exit(EXIT_FAILURE);
	}

	_socket_fd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);

	if (_socket_fd == -1) {
		std::cerr << "socket() error." << std::endl;
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}

	int yes=1;
	status = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

	if (status == -1){
		std::cerr << "setsockopt() error." << std::endl;
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}
	
	status = bind(_socket_fd, serv_info->ai_addr, serv_info->ai_addrlen);

	if (status == -1)
	{
		std::cerr << "bind() error." << std::endl;
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(serv_info);
	status = listen(_socket_fd, _BACKLOG);

	if (status == -1)
	{
		std::cerr << "listen() error." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::acceptClient()
{
	struct sockaddr_storage	client_addr;
	std::string				client_port;
	socklen_t				sin_size = sizeof client_addr;
	char					client_ip[INET6_ADDRSTRLEN];

	int	client_fd = accept(_socket_fd, (struct sockaddr *)&client_addr, &sin_size);

	if (client_fd == -1)
		std::cerr << "new client error" << std::endl;
	
	inet_ntop(client_addr.ss_family, utils_get_in_addr((struct sockaddr *)&client_addr), client_ip, sizeof client_ip);
	client_port = utils_get_port_str((struct sockaddr *)&client_addr);

	std::cout << "server: got connection from IP " << client_ip << "using PORT "<< client_port << std::endl; // testing

	addClient(client_ip, client_port, client_fd);
}

/* void	Server::addClient(const std::string& ip, std::string port, int client_fd)
{	
	if (ip.empty() || port.empty() || !client_fd || client_fd == -1)
		return ;

	Client	new_client(ip, port, client_fd);
	std::string	buffer = recv();

	if (new_client.parse(buffer) != -1)
		_clients[client_fd] = new_client;

	
} */

int	Server::getClientFd(std::string nickname) const
{
	if (nickname.empty())
		return (-1);
	
	for (std::map<int, Client>::const_iterator it = _client_map.begin(); it != _client_map.end(); ++it) {
		if (it->second.getNickname() == nickname)
			return it->first;
	}

	return (-1);
}

Channel*	Server::getChannel(std::string channel)
{
	if (channel.empty() || !_channel_map.count(channel))
		return NULL;

	return &_channel_map[channel];
}

bool	Server::deleteChannel(std::string channel_name)
{
	if (channel_name.empty() || !_channel_map.count(channel_name))
		return false;
	
	if (!_channel_map[channel_name].emptyChannel())
		return false;
	
	_channel_map.erase(channel_name);
	return true;
}

void	Server::deleteUser(Client *user)
{
	_client_map.erase(user->getClientFd());
	delete user;
}