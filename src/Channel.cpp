/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:51 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/15 18:45:07 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

/* :nick!user@host JOIN :#channel
:server 331 nick #channel :No topic is set
:server 353 nick = #channel :@nick* 
:server 366 nick #channel :End of /NAMES list */

void	Channel::joinChannel(std::string prefix, Client *new_user, std::string password, std::string server_name) 
{
	if (_password_active && _password != password)
	{
		sendReply(new_user->getClientFd(), server_name, "475", new_user->getNickname(), _channel_name, "Cannot join channel +k");
		return ; 
	}
	if (_invite_active && !_invited.count(new_user->getNickname()))
	{
		sendReply(new_user->getClientFd(), server_name, "473", new_user->getNickname(), _channel_name, "Cannot join channel +i");
		return ;
	}
	if (_user_limit_active && (int)_users.size() >= _user_limit)
	{
		sendReply(new_user->getClientFd(), server_name, "471", new_user->getNickname(), _channel_name, "Cannot join channel +l");
		return ;
	}
	_users.insert(new_user);
	
	// send join message to each client
	std::string join_msg = prefix + " JOIN :" + _channel_name + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), join_msg.c_str(), join_msg.size(), 0);
	
	// if topic message is set
	if (_topic_active && !_topic.empty())
		sendReply(new_user->getClientFd(), server_name, "332",new_user->getNickname(), _channel_name, _topic);
	else
		sendReply(new_user->getClientFd(), server_name, "331",new_user->getNickname(), _channel_name, "No topic is set");
	// send name list to new user
	std::string names = "";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (_operators.count(*it))
			names += "@";
		names += (*it)->getNickname() + " ";
	}
	sendReply(new_user->getClientFd(), server_name, "353", new_user->getNickname() + " =", _channel_name, names);
	sendReply(new_user->getClientFd(), server_name, "366", new_user->getNickname(), _channel_name, "End of /NAMES list");
}

void	Channel::partChannel(std::string prefix, Client *user_delete, std::string reason) {
	
	std::string part_msg = prefix + " PART " + _channel_name + " :" + reason + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), part_msg.c_str(), part_msg.size(), 0);

	if (_operators.count(user_delete))
		_operators.erase(user_delete);
	_users.erase(user_delete);
	user_delete->disconnectChannel(this);
}

void	Channel::setTopicText(std::string prefix, Client* user, std::string topic, std::string server_name) 
{
	if (_topic_active && !isOperator(user))
	{
		sendReply(user->getClientFd(), server_name, "482", user->getNickname(), _channel_name, "You're not channel operator");
		return ;
	}
	_topic = topic;
	this->ChannelBroadcast(prefix, user, "TOPIC", _topic);

}

void	Channel::setInvite(char sign, std::string prefix, Client* user) {
	if (sign == '+')
		_invite_active = true;
	else 
		_invite_active = false;
	this->ModeBroadcast(prefix, user, "MODE", std::string(1, sign) + "i");
}

void	Channel::setTopic(char sign, std::string prefix, Client* user) {
	if (sign == '+')
		_topic_active = true;
	else
		_topic_active = false;
	this->ModeBroadcast(prefix, user, "MODE", std::string(1, sign) + "t");
}

void	Channel::setPassword(char sign, std::string password, std::string prefix, Client* user) 
{
	if (sign == '+') 
		_password_active = true;
	else
		_password_active = false;
	_password = password;
	this->ModeBroadcast(prefix, user, "MODE", std::string(1, sign) + "k " + password);
}

void	Channel::setOperator(char sign, std::string nickname, std::string prefix, Client* user)
{
	Client *tmp = NULL;
	for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++ )
	{
		if (nickname == (*it)->getNickname()) {
			tmp = *it;
			break;
		}
	}
	if (sign == '+' && _users.count(tmp))
		_operators.insert(tmp);
	else if (sign == '-' && _operators.count(tmp))
		_operators.erase(tmp);
	else 
	 // return error
	this->ModeBroadcast(prefix, user, "MODE", std::string(1, sign) + "o " + nickname);
}

void	Channel::setLimit(char sign, int max_size, std::string prefix, Client* user) {
	if (sign == '+')
	{
		_user_limit_active = true;
		_user_limit = max_size;
		this->ModeBroadcast(prefix, user, "MODE", std::string(1, sign) + "l " + intToString(max_size));
	}
	else if (sign == '-')
	{
		_user_limit_active = false;
		this->ModeBroadcast(prefix, user, "MODE", std::string(1, sign) + "l ");
	}
}

void	Channel::ChannelMessage(std::string prefix, Client *sender, std::string command, std::string buffer) {
	std::string msg = prefix + command + _channel_name + " :" + buffer + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (sender->getClientFd() == (*it)->getClientFd())
			continue ;
		send((*it)->getClientFd(), msg.c_str(), msg.size(), 0);
	}
}

void	Channel::ChannelBroadcast(std::string prefix, Client *sender, std::string command, std::string buffer) {
	std::string msg = prefix + " " + command + " " + _channel_name + " :" + buffer + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), msg.c_str(), msg.size(), 0);
}

void	Channel::ModeBroadcast(std::string prefix, Client *sender, std::string command, std::string buffer) {
	std::string msg = prefix + " " + command + " " + _channel_name + " " + buffer + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), msg.c_str(), msg.size(), 0);
}

std::string Channel::getName() const {
	return (this->_channel_name);
}

std::string Channel::getTopic() const {
	return (this->_topic);
}

bool	Channel::hasUser(Client *user) const {
	return (_users.count(user));
}

bool	Channel::hasTopic() {
	return (!_topic.empty());
}

bool	Channel::emptyChannel() const {
	return (_users.empty());
}

void	Channel::removeUser(Client *user)
{
	if (_users.count(user))
		_users.erase(user);

	if (_operators.count(user))
		_operators.erase(user);
}

bool	Channel::isOperator(Client *user) {
	return (_operators.count(user));
}

Channel::Channel() :
	_channel_name(),
	_invite_active(false),
	_topic_active(false),
	_topic(),
	_password_active(false),
	_password(),
	_user_limit_active(false),
	_user_limit(0)
{
}

void	Channel::init(const std::string& channel_name, Client *creator)
{
	_channel_name = channel_name;
	_operators.insert(creator);
}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel::Channel(const std::string& channel_name, Client* creator) :
	_channel_name(channel_name),
	_invite_active(false),
	_topic_active(false),
	_topic(""),
	_password_active(false),
	_password(""),
	_user_limit_active(false),
	_user_limit(0)
{
	_operators.insert(creator);
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