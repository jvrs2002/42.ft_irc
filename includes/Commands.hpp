/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:15 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/18 21:02:23 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Message.hpp"
#include "Client.hpp"

#define CHANNEL '#'

class Server;

class Commands
{
private:
		static void join_handler(const Message& msg, Client* user, Server* server);
		static void part_handler(const Message& msg, Client* user, Server* server);
		static void privmsg_handler(const Message& msg, Client* user, Server* server);
		static void notice_handler(const Message& msg, Client* user, Server* server);
		static void pass_handler(const Message& msg, Client* user, Server* server);
		static void user_handler(const Message& msg, Client* user, Server* server);
		static void nick_handler(const Message& msg, Client* user, Server* server);
		static void mode_handler(const Message& msg, Client* user, Server* server);
		static void topic_handler(const Message& msg, Client* user, Server* server);
		static void kick_handler(const Message& msg, Client* user, Server* server);
		static void invite_handler(const Message& msg, Client* user, Server* server);
		static void list_handler(const Message& msg, Client* user, Server* server);
		static void quit_handler(const Message& msg, Client* user, Server* server);
		

		
		typedef void (*PFnCommandHandler)(const Message&, Client*, Server*);
		std::map<std::string, PFnCommandHandler> _handler;
		
public:
		Commands();
		~Commands();
		void Commandhandler(const Message& msg, Client* user, Server* server);
};

#endif