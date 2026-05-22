/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:51 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/22 15:28:38 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Channel.hpp"

static void sendReply(int Clientfd, const std::string& server, const std::string& code, 
	const std::string& target, const std::string& params, const std::string& trailing) 
{
	std::string msg = ":" + server + " " + code + " " + target + " " + params + " :" + trailing + "\r\n";
	send(Clientfd, msg.c_str(), msg.size(), 0);
}

/* :nick!user@host JOIN :#channel
:server 331 nick #channel :No topic is set
:server 353 nick = #channel :@nick* 
:server 366 nick #channel :End of /NAMES list */

void Channel::joinChannel(Client *new_user, std::string password) 
{
	if (_password_active && _password != password)
	{
		sendReply(new_user->getClientFd(), SERVER_NAME, "475", new_user->getNickname(), _channel_name, "Cannot join channel +k");
		return ; 
	}
	if (_invite_active && !_invited.count(new_user->getNickname()))
	{
		sendReply(new_user->getClientFd(), SERVER_NAME, "473", new_user->getNickname(), _channel_name, "Cannot join channel +i");
		return ;
	}
	if (_user_limit_active && _users.size() >= _user_limit)
	{
		sendReply(new_user->getClientFd(), SERVER_NAME, "471", new_user->getNickname(), _channel_name, "Cannot join channel +l");
		return ;
	}
	_users.insert(new_user);
	
	// send join message to each client
	std::string join_msg = ":" + new_user->getNickname() + "@" + new_user->get_ip() + " JOIN :" + _channel_name + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), join_msg.c_str(), join_msg.size(), 0);
	
	// if topic message is set
	if (_topic_active && !_topic.empty())
		sendReply(new_user->getClientFd(), SERVER_NAME, "332",new_user->getNickname(), _channel_name, _topic);
	else
		sendReply(new_user->getClientFd(), SERVER_NAME, "331",new_user->getNickname(), _channel_name, "No topic is set");
	// send name list to new user
	std::string names = "";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (_operators.count(*it))
			names += "@";
		names += (*it)->getNickname() + " ";
	}
	sendReply(new_user->getClientFd(), SERVER_NAME, "353", new_user->getNickname() + " =", _channel_name, names);
	sendReply(new_user->getClientFd(), SERVER_NAME, "366", new_user->getNickname(), _channel_name, "End of /NAMES list");
}

void Channel::partChannel(Client *user_delete) {
	if (_operators.count(user_delete))
		_operators.erase(user_delete);
	_users.erase(user_delete);
	
}

std::string Channel::getName() const {
	return (this->_channel_name);
}

Channel::Channel(){}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel::Channel(const std::string channel_name, Client* creator) :
	_channel_name(channel_name),
	_invite_active(false),
	_topic_active(false),
	_topic(""),
	_password_active(false),
	_password(""),
	_user_limit_active(false),
	_user_limit(0)
{
	_users.insert(creator);
	_operators.insert(creator);
	std::cout << "New Channel " << channel_name << " created by " << creator->getNickname() << std::endl;
}
Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		this->_channel_name = other._channel_name;
		this->_invite_active = other._invite_active;
		this->_invited = other._invited;
		this->_operators = other._operators;
		this->_password = other._password;
		this->_password_active = other._password_active;
		this->_topic = other._topic;
		this->_topic_active = other._topic_active;
		this->_user_limit = other._user_limit;
		this->_user_limit_active = other._user_limit_active;
		this->_users = other._users;
	}
	return *this;
}

Channel::~Channel() {}