/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppassos <ppassos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:20 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/20 17:10:13 by ppassos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
//#include "includes/Channel.hpp"


/*
Exemple of messages the class wil handle:
:joao!user@host PRIVMSG #42 :hi guys how is going?
NICK joao
USER joao 0 * :Joao Bruno
JOIN #42
KICK #42 joao :motivo
PRIVMSG #42 :hi, how is going?
PRIVMSG maria :hi Ze!
*/
class Message
{
private:
	std::string prefix; //prefix = |:"nickname"!"username"@"userip"|
	std::string command; //the comand (exp:PASS, NICK, USER, JOIN, PRIVMSG)
	std::vector<std::string> params; //all after command
public:
	Message();
	Message(std::string prefix, std::string command, std::vector<std::string> params);
	Message(const Message &src);
	Message &operator=(const Message &other);
	~Message();
	
	std::string getComand() const;
	std::vector<std::string> getParams() const;
	std::string getPrefix() const;

	void FillMessage(std::string buffer, std::string ip, int len);
	std::string Fillcommand(std::string line);
	std::vector<std::string> Fillparams(std::string line);
};

Message::Message()
{
	this->command = "";
	this->prefix = "";
	this->params = {};
}

Message::~Message(){}

Message::Message(std::string prefix, std::string command, std::vector<std::string> params)
{
	this->command = command;
	this->prefix = prefix;
	this->params = params;
}
Message::Message(const Message &src)
{
	*this = src;
}

Message &Message::operator=(const Message &other)
{
	if (this != &other)
	{
		this->command = other.command;
		this->prefix = other.prefix;
		this->params = other.params;
	}
	return(*this);
}

std::string Message::getComand() const
{
	return(this->command);
}

std::vector<std::string> Message::getParams() const
{
	return(this->params);
}

std::string Message::getPrefix() const
{
	return(this->prefix);
}
std::vector<std::string> Message::Fillparams(std::string line)
{
	size_t space = line.find(' ');
	std::string rest = line.substr(space + 1);
	std::vector<std::string> params;

	while (!rest.empty())
	{
		if (rest[0] == ':')
		{
			params.push_back(rest.substr(1));
			break ;
		}
		size_t pos = rest.find(' ');
		if (pos == std::string::npos)
		{
			params.push_back(rest);
			break ;
		}
		params.push_back(rest.substr(0, pos));
		rest = rest.substr(pos + 1);
	}
	return (params);
}

std::string Message::Fillcommand(std::string line)
{
	size_t space = line.find(' ');
	std::string command = line.substr(0, space);
	return (command);
}
void Message::FillMessage(std::string buffer, std::string ip, int len)
{
	if (len == 2){
		std::cout <<"ERROR" << std::endl;
		return ;
	}
	this->prefix = ip;
	this->command = Fillcommand(buffer);
	this->params = Fillparams(buffer);
}



//char *buffer;

#endif
