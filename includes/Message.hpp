/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:20 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/18 21:01:31 by joao-vri         ###   ########.fr       */
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

class Message
{
private:
	std::string prefix; // prefix = |:"nickname"!"username"@"userip"|
	std::string command; // command (exp:PASS, NICK, USER, JOIN, PRIVMSG)
	std::vector<std::string> params; // after command
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

#endif
