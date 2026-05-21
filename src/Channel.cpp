/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:51 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/20 18:30:58 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Channel.hpp"

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

void Channel::joinChannel(Client *new_user, std::string password) {
	if (_invite_active)
	{
		if (_invited.count(new_user->getNickname())) 
		{
			_users.insert(new_user);
			std::cout << new_user << " has joined the channel " << this->_channel_name << std::endl;
			return ;
		}
		std::cout << new_user << " wasn't invited to join the channel " << this->_channel_name << std::endl;
		return ;
	}
	if (_password_active && _password != password)
	{
		std::cout << "Wrong password, access denied" << std::endl;
		return ; 
	}
	_users.insert(new_user);
	std::cout << new_user << " has joined the channel " << this->_channel_name << std::endl;
}

void Channel::partChannel(Client *user_delete) {
	if (_operators.count(user_delete))
		_operators.erase(user_delete);
	_users.erase(user_delete);
		
}