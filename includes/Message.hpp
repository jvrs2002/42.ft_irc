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
#include "Client.hpp"
#include "Utils.hpp"


//funcoes que preciso de ti joao:
//getBuffer()
//getip()
//setNickname()
//setUsername()
//setRealName()
//setAuthenticatedTrue();tb pode ser so setregisted e sai eu escolho
//setRegisteredTrue(); tb pode ser so setregisted e sai eu escolho

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
	Message(Client *user, const std::string& command);
	Message &operator=(const Message &other);
	~Message();
	
	std::string getCommand() const;
	std::vector<std::string> getParams() const;
	std::string getPrefix() const;
	std::string SetPrefix(Client *user);

	std::string Fillcommand(std::string line);
	std::vector<std::string> Fillparams(std::string line);
};
//void Message::FillMessage(Client* user, int len)
//char *buffer;

#endif
