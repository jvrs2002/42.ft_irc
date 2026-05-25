/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:29 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/23 02:15:00 by joao-vri         ###   ########.fr       */
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

// Helper function to get the socket address (IPv4 or IPv6)
static void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void	Server::acceptClient()
{
	struct sockaddr_storage	new_client_addr;
	socklen_t				sin_size = sizeof new_client_addr;
	char					s[INET6_ADDRSTRLEN];

	int	new_client_fd = accept(_socket_fd, (struct sockaddr *)&new_client_addr, &sin_size);

	if (new_client_fd == -1)
		std::cerr << "new client error" << std::endl;
	
	inet_ntop(new_client_addr.ss_family, get_in_addr((struct sockaddr *)&new_client_addr), s, sizeof s);
}

void	Server::addClient(const std::string& ip, std::string port, std::string buffer, int client_fd)
{	
	Client	new_client(ip, port, buffer, client_fd);
}

/* void	Server::delete_user(Client *user)
{
	_clients.erase(user->getClientFd());
	delete user;
} */