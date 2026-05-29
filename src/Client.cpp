/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/29 22:25:41 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

static const size_t MAX_IRC_MSG_LEN = 512;

Client::Client(std::string ip, std::string port, int fd) :
	_ip(ip),
	_port(port),
	_socket_fd(fd),
	_authenticated(false),
	_registered(false),
	_buffer()
{
	// connectClient();
	// add here new_client_handler function
}

Client::~Client()
{
	// add here destroy_client function (it has to delete also from Channel's)
}

/*	(RFC 2812) IRC messages are always lines of characters terminated with a CR-LF (\r\n)
	pair, and these messages SHALL NOT exceed 512 characters in length, counting
	all characters including the trailing CR-LF. Thus, there are 510 characters 
	maximum allowed for the command and its parameters.*/
bool	Client::receiveBuffer()
{
	if (_socket_fd == -1)
		return false;

	int	bytes_received;
	char temp_buffer[MAX_IRC_MSG_LEN + 1];

	bytes_received = recv(_socket_fd, temp_buffer, MAX_IRC_MSG_LEN, 0);

	if (bytes_received <= 0)
		return false;
	
	temp_buffer[bytes_received] = '\0';
	_buffer.append(temp_buffer);
	return true;
}

int	Client::getClientFd() const
{
	return _socket_fd;
}