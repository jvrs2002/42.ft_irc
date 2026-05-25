/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:29 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/19 18:07:28 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Server.hpp"

Server::Server(std::string ip, int port, std::string password) : _ip(ip),
		_port(port), _password(password)
{
	Server::init_server();
}
Server::~Server()
{
}

void	Server::init_server()
{
	int	server_fd = socket()
}

/* Client*	Server::create_user(const std::string& ip, int port, int fd)
{
} */

void	Server::delete_user(Client *user)
{
	_clients.erase(user->get_fd());
	delete user;
}