/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppassos <ppassos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:51 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/18 16:03:41 by ppassos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

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

void	Channel::init(const std::string& channel_name, Client *creator)
{
	_channel_name = channel_name;
	_operators.insert(creator);
}

void	Channel::joinChannel(const std::string& prefix, Client *new_user, const std::string& password, const std::string& server_name) 
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

	std::string join_msg = prefix + " JOIN :" + _channel_name + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), join_msg.c_str(), join_msg.size(), MSG_NOSIGNAL);
	
	if (_topic_active && !_topic.empty())
		sendReply(new_user->getClientFd(), server_name, "332",new_user->getNickname(), _channel_name, _topic);
	else
		sendReply(new_user->getClientFd(), server_name, "331",new_user->getNickname(), _channel_name, "No topic is set");
	std::string names = "";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (_operators.count(*it))
			names += "@";
		names += (*it)->getNickname() + " ";
	}
	sendReply(new_user->getClientFd(), server_name, "353", new_user->getNickname() + " =", _channel_name, names);
	sendReply(new_user->getClientFd(), server_name, "366", new_user->getNickname(), _channel_name, "End of /NAMES list");
}

void	Channel::partChannel(const std::string& prefix, Client *user_delete, const std::string& reason) {
	
	std::string part_msg = prefix + " PART " + _channel_name + " :" + reason + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), part_msg.c_str(), part_msg.size(), MSG_NOSIGNAL);

	if (_operators.count(user_delete))
		_operators.erase(user_delete);
	_users.erase(user_delete);
	user_delete->disconnectChannel(this);
}

void	Channel::setTopicText(const std::string& prefix, Client* user, const std::string& topic, const std::string& server_name) 
{
	if (_topic_active && !isOperator(user))
	{
		sendReply(user->getClientFd(), server_name, "482", user->getNickname(), _channel_name, "You're not channel operator");
		return ;
	}
	_topic = topic;
	this->ChannelBroadcast(prefix, "TOPIC", _topic);
}

void	Channel::kickUser(const std::string& prefix, Client* target, const std::string& reason) 
{
	std::string kick_msg = prefix + " KICK " + _channel_name + " " + target->getNickname() + " :" + reason + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), kick_msg.c_str(), kick_msg.size(), MSG_NOSIGNAL);

	_operators.erase(target);
	_users.erase(target);
	target->disconnectChannel(this);
}

void	Channel::addInvite(const std::string& target_nick) 
{
	_invited.insert(target_nick);
}


void	Channel::setInvite(char sign, const std::string& prefix) {
	if (sign == '+')
		_invite_active = true;
	else 
		_invite_active = false;
	this->ModeBroadcast(prefix, "MODE", std::string(1, sign) + "i");
}

void	Channel::setTopic(char sign, const std::string& prefix) {
	if (sign == '+')
		_topic_active = true;
	else
		_topic_active = false;
	this->ModeBroadcast(prefix, "MODE", std::string(1, sign) + "t");
}

void	Channel::setPassword(char sign, const std::string& password, const std::string& prefix) 
{
	if (sign == '+') 
		_password_active = true;
	else
		_password_active = false;
	_password = password;
	std::string mode_param = std::string(1, sign) + "k";
	if (sign == '+' && !password.empty())
    	mode_param += " " + password;
	this->ModeBroadcast(prefix, "MODE", mode_param);
}

void	Channel::setOperator(char sign, const std::string& nickname, const std::string& prefix)
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
	this->ModeBroadcast(prefix, "MODE", std::string(1, sign) + "o " + nickname);
}

void	Channel::setLimit(char sign, int max_size, const std::string& prefix) {
	if (sign == '+')
	{
		_user_limit_active = true;
		_user_limit = max_size;
		this->ModeBroadcast(prefix, "MODE", std::string(1, sign) + "l " + intToString(max_size));
	}
	else if (sign == '-')
	{
		_user_limit_active = false;
		this->ModeBroadcast(prefix, "MODE", std::string(1, sign) + "l");
	}
}

std::string Channel::getName() const {
	return (this->_channel_name);
}

std::string Channel::getTopic() const {
	return (this->_topic);
}

std::string Channel::getModeString() const {
	std::string modes = "+";
	std::string params;

	if (_invite_active)
		modes += "i";
	if (_topic_active)
		modes += "t";
	if (_password_active)
	{
		modes += "k";
		params += " " + _password;
	}
	if (_user_limit_active)
	{
		modes += "l";
		params += " " + intToString(_user_limit);
	}
	return (modes + params);
}

bool	Channel::getInvite() const {
	return (this->_invite_active);
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

void	Channel::ChannelMessage(const std::string& prefix, Client *sender, const std::string& command, const std::string& buffer) {
	std::string msg = prefix + command + _channel_name + " :" + buffer + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (sender->getClientFd() == (*it)->getClientFd())
			continue ;
		send((*it)->getClientFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	}
}

void	Channel::ChannelBroadcast(const std::string& prefix, const std::string& command, const std::string& buffer) {
	std::string msg = prefix + " " + command + " " + _channel_name + " :" + buffer + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void	Channel::ModeBroadcast(const std::string& prefix, const std::string& command, const std::string& buffer) {
	std::string msg = prefix + " " + command + " " + _channel_name + " " + buffer + "\r\n";
	for (std::set<Client*>::iterator it = _users.begin(); it != _users.end(); it++)
		send((*it)->getClientFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

const std::set<Client*>& Channel::getUsers() const
{
	return _users;
}
