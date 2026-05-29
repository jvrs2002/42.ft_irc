/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/28 11:41:15 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Utils.hpp"

Commands::Commands()
{
	_handler["JOIN"] = &join_handler;
	_handler["PART"] = &part_handler;
	_handler["PRIVMSG"] = &privmsg_handler;
}

Commands::~Commands()
{

}

void Commands::Commandhandler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map) 
{
	_handler[msg.getCommand()] (msg, user, client_map, channel_map);
}

// static void sendReply(int Clientfd, const std::string& server, const std::string& code, 
// 	const std::string& target, const std::string& params, const std::string& trailing) 
// {
// 	std::string msg = ":" + server + " " + code + " " + target + " " + params + " :" + trailing + "\r\n";
// 	send(Clientfd, msg.c_str(), msg.size(), 0);
// }

void Commands::privmsg_handler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map) 
{
	std::vector<std::string> params = msg.getParams();

	if (params[0].empty())
	{
		sendReply(user->getClientFd(), SERVER_NAME, "411", user->getNickname(), "PRIVMSG", "No recipient given");
		return ;
	}
	if (params[1].empty())
	{
		sendReply(user->getClientFd(), SERVER_NAME, "412", user->getNickname(), "", "No text to send");
		return ;
	}

	std::string target = params[0];
	std::string message = params[1];
	if (!channel_map.count(target) && target[0] == '#' )
	{
		sendReply(user->getClientFd(), SERVER_NAME, "403", user->getNickname(), target, "No such channel");
		return ;
	}
	if (!client_map.count(user->getClientFd()) && target[0] != '#')
	{
		sendReply(user->getClientFd(), SERVER_NAME, "401", user->getNickname(), target, "No such nick");
		return ;
	}
	if (!channel_map[target].hasUser(user) && target[0] == '#')
	{
		sendReply(user->getClientFd(), SERVER_NAME, "404", user->getNickname(), target, "Cannot send to channel");
		return ;
	}
	if (target[0] == '#')
		channel_map[target].ChannelMessage(msg.getPrefix(), user, message);
	else
		client_map[].

}

void Commands::join_handler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map)
{
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), SERVER_NAME, "461", user->getNickname(), "JOIN", "Not enough parameters");
		return ;
	}

	std::string channel_name = params[0];
	std::string password = (params.size() > 1) ? params[1] : "";

	if (channel_name.size() <= 1 || channel_name[0] != '#')
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

void Commands::part_handler(Message msg, Client* user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map) {
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