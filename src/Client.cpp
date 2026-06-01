/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/01 10:13:43 by joao-vri         ###   ########.fr       */
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
}

Client::~Client()
{
	if (_socket_fd != -1)
		close(_socket_fd);
	
	_buffer.clear();
}

/*	(RFC 2812) IRC messages are always lines of characters terminated with a CR-LF (\r\n)
	pair, and these messages SHALL NOT exceed 512 characters in length, counting
	all characters including the trailing CR-LF. Thus, there are 510 characters 
	maximum allowed for the command and its parameters.
	
	This function does not deal with incomplete buffers. */
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

std::string	Client::getUsername() const
{
	return _username;
}

std::string	Client::getNickname() const
{
	return _nickname;
}

std::string	Client::getRealName() const
{
	return _realname;
}

int	Client::getClientFd() const
{
	return _socket_fd;
}