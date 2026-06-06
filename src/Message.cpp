/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:33 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/13 19:24:01 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Message.hpp"

Message::Message()
{
	this->command = "";
	this->prefix = "";
	this->params = std::vector<std::string>();
}

Message::~Message(){}
std::string Message::SetPrefix(Client *user)
{
	std::string  username = user->getUsername();
	std::string  nickname = user->getNickname();
	std::string  ip = user->getip();

	if (username.empty() || nickname.empty() || ip.empty())
		return("");
	std::string prefix = ":" + nickname + "!" +username + "@" + ip;
	return(prefix);
}
Message::Message(Client *user) //std::string buffer, std::string ip, int len
{
	std::string buffer = user->getBuffer();
	int len = buffer.size();
	size_t start = buffer.find_first_not_of(" \t");
	if (start == std::string::npos)
    	return;
	buffer = buffer.substr(start);
	if (len == 2 ){
		std::cout << std::endl;
		return ;
	}
	if (len > 511){
		sendReply(user->getClientFd(), SERVER_NAME, "417", user->getNickname(), "", "Input line was too long");
		return ;
	}
	this->prefix = SetPrefix(user);
	this->command = Fillcommand(buffer);
	this->params = Fillparams(buffer);
}

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

std::string Message::getCommand() const
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
bool isSpace(char c)
{
	return (c == ' ' || c == '\t');
}
std::vector<std::string> Message::Fillparams(std::string line)
{
	std::vector<std::string> params;
	size_t space = line.find_first_of(" \t");
	if (space == std::string::npos)
        return params;
	std::string rest = line.substr(space + 1);
	while (!rest.empty())
	{
		while (!rest.empty() && isSpace(rest[0]))
            rest.erase(0, 1);
        if (rest.empty())
            break;
		if (rest[0] == ':')
		{
			params.push_back(rest.substr(1));
			break ;
		}
		size_t pos = rest.find_first_of(" \t");
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
	size_t space = line.find_first_of(" \t");
	std::string command = line.substr(0, space);
	return (command);
}