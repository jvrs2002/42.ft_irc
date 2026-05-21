/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:29 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/20 22:35:13 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Server.hpp"

const std::string Server::_ip = "127.0.0.1";

Server::Server(std::string ip, int port, std::string password) : _ip(ip),
		_port(port), _password(password)
{
	Server::init_server(_ip, _port, _password);
}
Server::~Server()
{
}

void	Server::init_server(std::string ip, int port, std::string password, int socket_fd)
{
	int	status;
	struct addrinfo	hints;
	struct addrinfo	*serv_info;

	memset(&hints, 0, sizeof hints);
	getaddrinfo(NULL, _port, &hints, &serv_info);
	_socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	if (_socket_fd == -1) {
		perror("Socket creation error");
		exit(EXIT_FAILURE);
	}

	bind(_socket_fd, )
}

/* Client*	Server::create_user(const std::string& ip, int port, int fd)
{
} */

void	Server::delete_user(Client *user)
{
	_clients.erase(user->get_fd());
	delete user;
}