/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppassos <ppassos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:20 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/02 18:05:33 by ppassos          ###   ########.fr       */
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
//
//numero de parms para cada comando min para max:
//JOIN -> 1-2     1- seria o chanel e o 2- seria a password
//PART -> 1-2	  1- seria o chanel e o 2- mensagem de despedida
//PRIVMSG -> 2   SEMPRE 1-seria chanel ou user/users 2-mensagem a ser enviada
//NOTICE -> 2    1- target 2- message
//MODE -> 2-"infinite"    1-target 2- mode !!3+ args...!!
//KICK -> 2-3  1-chanel   2- user  3(optional)-rasao do kick
//INVITE -> 2  1-user 2-chanel
//TOPIC  -> 1-2 1-chanel  2-topic
 
//#include "includes/Channel.hpp"
//o join do meu colega ja : verifica se o primeiro
//sao iguais: "JOIN #42 ola" e "JOIN #42 :ola"
// JOIN #42 ,OLA
//tenho de fazer um getspefic param 
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
	
	std::string getCommand() const;
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
	this->params = std::vector<std::string>();
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
void Message::FillMessage(std::string buffer, std::string ip, int len)
{
	size_t start = buffer.find_first_not_of(" \t");
	buffer = buffer.substr(start);
	if (len == 2 ){
		std::cout << std::endl;
		return ;
	}
	if (len > 511){
		std:: cout << "417 ERR_INPUTTOOLONG" <<std::endl;
		return ;
		//teria de ser sendReply(user->getClientFd(), SERVER_NAME, "417", user->getNickname(), "", "Input line was too long")
	}
	this->prefix = ip;
	this->command = Fillcommand(buffer);
	this->params = Fillparams(buffer);
}


//void Message::FillMessage(Client* user, int len)
//char *buffer;

#endif
