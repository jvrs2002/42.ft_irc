/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/19 15:21:13 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"

static const size_t MAX_IRC_MSG_LEN = 512;

Client::Client() :
	_authenticated(false),
	_registered(false),
	_ip(),
	_port(),
	_socket_fd(-1),
	_buffer()
{

}

Client::Client(const std::string& ip, const std::string& port, int fd) :
	_authenticated(false),
	_registered(false),
	_ip(ip),
	_port(port),
	_socket_fd(fd),
	_buffer()
{
}

Client::~Client()
{
	if (_socket_fd != -1)
		close(_socket_fd);
	
	_buffer.clear();
	std::set<Channel*>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); ++it) {
			(*it)->removeUser(this); // what happens if he's not connected to any?
	}

	_channels.clear();
}

bool	Client::initClient(const std::string& ip, const std::string& port, int fd)
{
	if (ip.empty() || port.empty() || fd == -1)
		return false;
	
	_ip = ip;
	_port = port;
	_socket_fd = fd;
	return true;
}

/*	(RFC 2812) IRC messages are always lines of characters terminated with a CR-LF (\r\n)
	pair, and these messages SHALL NOT exceed 512 characters in length, counting
	all characters including the trailing CR-LF. Thus, there are 510 characters 
	maximum allowed for the command and its parameters.
	
	This function does not deal with incomplete buffers. */
Client::RecvReturn	Client::receiveBuffer()
{
	if (_socket_fd == -1)
		return RECV_ERROR;

	char temp_buffer[MAX_IRC_MSG_LEN + 1];
	int bytes_received = recv(_socket_fd, temp_buffer, MAX_IRC_MSG_LEN, 0);

	if (bytes_received == 0)
		return RECV_EOF;
	
	if (bytes_received < 0)
		return RECV_ERROR;

	temp_buffer[bytes_received] = '\0';
	_buffer.append(temp_buffer);
	return RECV_SUCCESS;
}

bool	Client::addToChannel(Channel* channel)
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

bool	Client::setRegistered()
{
	if (!_authenticated)
		return false;

	if (_nickname.empty() || _username.empty())
		return false;

	_registered = true;
	return true;
}

// only to be used within authenticate()
void	Client::setAuthenticated()
{
	_authenticated = true;
}

bool	Client::setNickname(const std::string& nickname)
{
	if (nickname.empty())
		return false;
	
	_nickname = nickname;
	return true;
}

bool	Client::setRealname(const std::string& realname)
{
	if (realname.empty())
		return false;
	
	_realname = realname;
	return true;
}

bool	Client::setUsername(const std::string& username)
{
	if (username.empty())
		return false;
	
	_username = username;
	return true;
}
const std::string& Client::getClientIP() const
{
	return _ip;
}

bool	Client::disconnectChannel(Channel *channel)
{
	if (!channel)
		return false;

	_channels.erase(channel);
	return true;
}

std::string	Client::handlePartialBuffer()
{
	if (_buffer.empty())
		return "";
	
	std::string::size_type delim = _buffer.find("\r\n");

	if (delim == std::string::npos)
		return "";

	std::string	substr_buffer = _buffer.substr(0, delim);
	_buffer.erase(0, delim + 2);
	return substr_buffer;
}

//extra from protocolist person


void Client::Cbroadcast(const std::string& msg) // so para os chanels que o client esta
{
    std::set<Client*> sent;

    std::set<Channel*>::iterator it = _channels.begin();
    while (it != _channels.end())
    {
        Channel* channel = *it;

        const std::set<Client*>& users = channel->getUsers();

        std::set<Client*>::iterator cit = users.begin();
        while (cit != users.end())
        {
            Client* target = *cit;

            if (target && target != this && sent.find(target) == sent.end())
            {
                send(target->getClientFd(), msg.c_str(), msg.size(), 0);
                sent.insert(target);
            }
            ++cit;
        }
        ++it;
    }
}

bool Client::ValidNick(const std::string& nick)
{
    if (nick.empty())
        return false;

    // limite comum
    if (nick.size() > 9)
        return false;

    // primeiro char deve ser letra ou alguns símbolos permitidos
    if (!isalpha(nick[0]) && nick[0] != '_' && nick[0] != '-')
        return false;

    for (size_t i = 0; i < nick.size(); i++)
    {
        char c = nick[i];

        // permitido: letras, números, '_' e '-'
        if (!isalnum(c) && c != '_' && c != '-')
            return false;
    }

    return true;
}