/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/03 16:49:59 by joao-vri         ###   ########.fr       */
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

bool		Client::addToChannel(Channel* channel)
{
	if (!channel || channel->hasUser(this) || !_registered || _channels.size() >= 10)
		return false;

	_channels.insert(channel);
	return true;
}


const std::string& Client::getUsername() const
{
	return _username;
}

const std::string& Client::getNickname() const
{
	return _nickname;
}

const std::string& Client::getRealName() const
{
	return _realname;
}

int	Client::getClientFd() const
{
	return _socket_fd;
}

bool	Client::isAuthenticated() const
{
	return _authenticated;
}

bool	Client::isRegistered() const
{
	return _registered;
}