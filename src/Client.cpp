/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/26 14:38:19 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(std::string ip, std::string port, int fd) : _ip(ip), _port(port),
		_fd(fd), _authenticated(false), _registered(false), _buffer()
{
	// connectClient();
	// add here new_client_handler function
}

Client::~Client()
{
	// add here destroy_client function (it has to delete also from Channel's)
}

// void	receiveIncomingMessage()
// {
// }

int	Client::getClientFd()
{
	return _fd;
}