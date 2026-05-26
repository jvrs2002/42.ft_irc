/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/15 16:55:24 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Commands.hpp"

Commands::Commands()
{
	_handler["JOIN"] = &join_handler;
	_handler["PART"] = &part_handler;
}

Commands::~Commands()
{

}

void Commands::Commandhandler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map) 
{
	_handler[msg.getCommand()] (msg, user, client_map, channel_map);
}

static void sendReply(int Clientfd, const std::string& server, const std::string& code, 
	const std::string& target, const std::string& params, const std::string& trailing) 
{
	std::string msg = ":" + server + " " + code + " " + target + " " + params + " :" + trailing + "\r\n";
	send(Clientfd, msg.c_str(), msg.size(), 0);
}

void join_handler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map)
{
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), SERVER_NAME, "461", user->getNickname(), "JOIN", "Not enough parameters");
		return ;
	}

	std::string channel_name = params[0];
	std::string password = (params.size() > 1) ? params[1] : "";

	if (channel_name.empty() || channel_name[0] != '#')
	{
		sendReply(user->getClientFd(), SERVER_NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (channel_map.count(channel_name))
		channel_map[channel_name].joinChannel(user, password);
	else {
		channel_map[channel_name] = Channel(channel_name, user);
		channel_map[channel_name].joinChannel(user, password);
	}
}

void part_handler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map) {
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), SERVER_NAME, "461", user->getNickname(), "PART", "Not enough parameters");
		return ;
	}
	std::string channel_name = params[0];
	std::string reason = (params.size() > 1) ? params [1] : "Leaving";
	
	if (!channel_map.count(channel_name))
	{
		sendReply(user->getClientFd(), SERVER_NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (!channel_map[channel_name].hasUser(user))
	{
		sendReply(user->getClientFd(), SERVER_NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return ;
	}

	channel_map[channel_name].partChannel(user, reason);
	if (channel_map[channel_name].emptyChannel())
		channel_map.erase(channel_name);

}