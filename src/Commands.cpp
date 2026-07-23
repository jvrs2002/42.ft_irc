/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/21 22:07:01 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp" //extra
#include "Utils.hpp"

Commands::Commands()
{
	_handler["JOIN"] = &join_handler;
	_handler["PART"] = &part_handler;
	_handler["PRIVMSG"] = &privmsg_handler;
	_handler["NOTICE"] = &notice_handler;
	_handler["MODE"] = &mode_handler;
	_handler["TOPIC"] = &topic_handler;
	_handler["KICK"] = &kick_handler;
	_handler["INVITE"] = &invite_handler;
	_handler["PASS"] = &pass_handler;
	_handler["USER"] = &user_handler;
	_handler["NICK"] = &nick_handler;
	_handler["LIST"] = &list_handler;
}

Commands::~Commands()
{

}

void Commands::Commandhandler(const Message& msg, Client* user, Server* server) 
{
	if (_handler.count(msg.getCommand()) == 0)
		return ;
	_handler[msg.getCommand()] (msg, user, server);
}

void Commands::join_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered())
	{
		sendReply(user->getClientFd(), server->NAME, "451",
			user->getNickname(), "", "You have not registered");
		return;
	}

	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461",
			user->getNickname(), "JOIN", "Not enough parameters");
		return;
	}

	const std::string& channel_name = params[0];
	std::string password = (params.size() > 1) ? params[1] : ""; 

	if (channel_name.size() <= 1 || channel_name[0] != CHANNEL)
	{
		sendReply(user->getClientFd(), server->NAME, "403",
			user->getNickname(), channel_name, "No such channel");
		return;
	}

	Channel* channel = server->getChannel(channel_name);

	if (!channel)
	{
		server->createChannel(channel_name, user);
		channel = server->getChannel(channel_name);
		if (!channel)
			return;
	}
	if (channel->hasUser(user))
	{
		sendReply(user->getClientFd(), server->NAME, "443",
			user->getNickname(), channel_name, "User is already in channel");
		return;
	}
	if (!user->addToChannel(channel))
		return;
	channel->joinChannel(msg.getPrefix(), user, password, server->NAME);
}

void Commands::part_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "PART", "Not enough parameters");
		return ;
	}
	const std::string& channel_name = params[0];
	Channel* channel = server->getChannel(channel_name);
	std::string reason = (params.size() > 1) ? params [1] : "Leaving";
	
	if (channel == NULL)
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (!channel->hasUser(user))
	{
		sendReply(user->getClientFd(), server->NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return ;
	}

	channel->partChannel(msg.getPrefix(), user, reason);
	if (channel->emptyChannel())
		server->deleteChannel(channel_name);
}

void Commands::privmsg_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();
	if (params.size() < 1)
	{
		sendReply(user->getClientFd(), server->NAME, "411", user->getNickname(), "PRIVMSG", "No recipient given");
		return ;
	}
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "412", user->getNickname(), "", "No text to send");
		return ;
	}

	const std::string& target = params[0];
	const std::string& message = params[1];
	if (!target.empty() && target[0] == CHANNEL)
	{
		Channel* channel = server->getChannel(target);
		if (channel == NULL)
		{
			sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), target, "No such channel");
			return ;
		}
		if (!channel->hasUser(user))
		{	
			sendReply(user->getClientFd(), server->NAME, "404", user->getNickname(), target, "Cannot send to channel");
			return ;
		}
		channel->ChannelMessage(msg.getPrefix(), user, " PRIVMSG ", message);
	}
	else
	{
		int target_fd = server->getClientFd(target);
		if (target_fd == -1)
		{
			sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target, "No such nick");
			return ;
		}
		std::string priv_msg = msg.getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		send(target_fd, priv_msg.c_str(), priv_msg.size(), MSG_NOSIGNAL);
	}
}
//o notice serve para: enviar mensagens sem gerar erros automático sendo usado por bots, servers, serviços
void Commands::notice_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered())
		return ;
	std::vector<std::string> params = msg.getParams();

	if (params.size() < 2)
		return ;

	const std::string& target = params[0];
	const std::string& message = params[1];
	if (!target.empty() && target[0] == CHANNEL)
	{
		Channel* channel = server->getChannel(target);
		if (channel == NULL)
			return ;
		if (!channel->hasUser(user))
			return ;
		channel->ChannelMessage(msg.getPrefix(), user, " NOTICE ", message);
	}
	else
	{
		int target_fd = server->getClientFd(target);
		if (target_fd == -1)
			return ;
		std::string notice_msg = msg.getPrefix() + " NOTICE " + target + " :" + message + "\r\n";
		send(target_fd, notice_msg.c_str(), notice_msg.size(), MSG_NOSIGNAL);
	}
}

void Commands::mode_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered())
	{
		sendReply(user->getClientFd(), server->NAME, "451",
			user->getNickname(), "", "You have not registered");
		return;
	}

	std::vector<std::string> params = msg.getParams();
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461",
			user->getNickname(), "MODE", "Not enough parameters");
		return;
	}
	const std::string& channel_name = params[0];

	Channel* channel = server->getChannel(channel_name);
	if (!channel)
	{
		sendReply(user->getClientFd(), server->NAME, "403",
			user->getNickname(), channel_name, "No such channel");
		return;
	}

	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "324",
			user->getNickname(), channel_name, channel->getModeString());
		return;
	}
	const std::string& modestring = params[1];

	if (!channel->isOperator(user))
	{
		sendReply(user->getClientFd(), server->NAME, "482",
			user->getNickname(), channel_name, "You're not a channel operator");
		return;
	}

	char sign = '+';
	int arg_index = 2;

	std::string used_args = "";
	for (int i = 0; modestring[i]; i++)
	{
		if (modestring[i] == '+' || modestring[i] == '-')
			sign = modestring[i++];
		char mode = modestring[i];

		switch (mode)
		{
			case 'i':
				channel->setInvite(sign, msg.getPrefix());
				break;

			case 't':
				channel->setTopic(sign, msg.getPrefix());
				break;

			case 'k':
				if (sign == '+') 
				{
					if (arg_index >= (int)params.size())
					{
						sendReply(user->getClientFd(), server->NAME, "461",
							user->getNickname(), "MODE", "Not enough parameters");
						return;
					}
					std::string password = params[arg_index++];
					used_args += " " + password;
					channel->setPassword('+', password, msg.getPrefix());
				}
				else
					channel->setPassword('-', "", msg.getPrefix());
				break;

			case 'o':
				if (arg_index >= (int)params.size()) {
					sendReply(user->getClientFd(), server->NAME, "461",
						user->getNickname(), "MODE", "Not enough parameters");
					return;
				}
				{
					std::string nickname = params[arg_index++];
					used_args += " " + nickname;
					channel->setOperator(sign, nickname, msg.getPrefix());
				}
				break;

			case 'l':
				if (sign == '+') 
				{
					if (arg_index >= (int)params.size())
					{
						sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "MODE", "Not enough parameters");
						return;
					}
					int max_size = atoi(params[arg_index++].c_str());
					used_args += " " + params[arg_index - 1];
					channel->setLimit('+', max_size, msg.getPrefix());
				}
				else 
					channel->setLimit('-', 0, msg.getPrefix());
				break;

			default:
			{
				std::string modechar(1, mode);
				sendReply(user->getClientFd(), server->NAME, "472", user->getNickname(), modechar, "is unknown mode char to me");
				break;
			}
		}
	}
}

void	Commands::topic_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered())
	{
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "TOPIC", "Not enough parameters");
		return ;
	}
	const std::string& channel_name = params[0];
	Channel *channel = server->getChannel(channel_name);
	if (!channel)
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return;
	}
	if (!channel->hasUser(user)) 
	{
		sendReply(user->getClientFd(), server->NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return;
	}
	if (params.size() < 2)
	{
		if(channel->hasTopic())
			sendReply(user->getClientFd(), server->NAME, "332", user->getNickname(), channel_name, channel->getTopic());
		else
			sendReply(user->getClientFd(), server->NAME, "331", user->getNickname(), channel_name, "No Topic is set");
		return ;
	}
	const std::string& topic = params[1];
	channel->setTopicText(msg.getPrefix(), user, topic, server->NAME);
}

void	Commands::kick_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "KICK", "Not enough parameters");
		return ;
	}
	const std::string& channel_name = params[0];
	const std::string& target_nick = params[1];
	std::string reason = (params.size() > 2) ? params[2] : user->getNickname();
	Channel* channel = server->getChannel(channel_name);
	if (!channel) 
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return;
	}
	if (!channel->hasUser(user))
	{
		sendReply(user->getClientFd(), server->NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return;
	}
	if (!channel->isOperator(user))
	{
		sendReply(user->getClientFd(), server->NAME, "482", user->getNickname(), channel_name, "You're not channel operator");
		return;
	}
	
	int target_fd = server->getClientFd(target_nick);
	if (target_fd == -1)
	{
		sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target_nick, "No such nick");
		return;
	}
	
	Client* target = server->getClientInstance(target_fd);
	if (!channel->hasUser(target))
	{
		sendReply(user->getClientFd(), server->NAME, "441", user->getNickname(), target_nick, "They aren't on that channel");
		return;
	}
	channel->kickUser(msg.getPrefix(), target, reason); 
	if (channel->emptyChannel())
		server->deleteChannel(channel_name);
}

void Commands::invite_handler(const Message& msg, Client* user, Server* server)
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "INVITE", "Not enough parameters");
		return ;
	}
	const std::string& target_nick = params[0];
	const std::string& channel_name = params[1];
	Channel* channel = server->getChannel(channel_name);
	if (!channel) 
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return;
	}
	if (!channel->hasUser(user))
	{
		sendReply(user->getClientFd(), server->NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return;
	}
	if (!channel->isOperator(user))
	{
		sendReply(user->getClientFd(), server->NAME, "482", user->getNickname(), channel_name, "You're not channel operator");
		return;
	}

	int target_fd = server->getClientFd(target_nick);
	if (target_fd == -1)
	{
		sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target_nick, "No such nick");
		return;
	}
	
	Client* target = server->getClientInstance(target_fd);
	if (channel->hasUser(target)) {
		sendReply(user->getClientFd(), server->NAME, "443", user->getNickname(), target_nick, "is already on channel");//fd, server->NAME, "443", user->getNickname(), target_nick + " " + channel_name, "is already on channel
		return;
	}

	channel->addInvite(target_nick);

	sendReply(user->getClientFd(), server->NAME, "341", user->getNickname(), target_nick, channel_name);


	std::string invite_msg = msg.getPrefix() + " INVITE " + target_nick + " :" + channel_name + "\r\n";
	send(target->getClientFd(), invite_msg.c_str(), invite_msg.size(), MSG_NOSIGNAL);

	
}

void Commands::nick_handler(const Message& msg, Client* user, Server* server) 
{
	std::vector<std::string> params = msg.getParams();
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "431", user->getNickname(), "", "No nick given");
		return ;
	}
	if(!user->ValidNick(params[0]))
	{
		sendReply(user->getClientFd(), server->NAME, "432", user->getNickname(), "", "Invalid nickname");
		return ;
	}
	if (server->userExists(params[0]))
	{
		sendReply(user->getClientFd(), server->NAME, "433", user->getNickname(), "", "Nickname is already in use");
		return ;
	}
	std::string oldNick = user->getNickname();
	user->setNickname(params[0]);
	std::string rmsg = ":" + oldNick + "!" + user->getUsername() + "@" + user->getClientIP() + " NICK :" + user->getNickname() + "\r\n";
	if (user->isRegistered())
	{
		user->Cbroadcast(rmsg);
		send(user->getClientFd(), rmsg.c_str(), rmsg.size(), MSG_NOSIGNAL);
	}
	if (!(user->isRegistered()) && !user->getNickname().empty() && !user->getUsername().empty() && user->isAuthenticated())
	{
		user->setRegistered();
		sendReply(user->getClientFd(), server->NAME, "001", user->getNickname(), "", "Welcome to the IRC server");
	}
}

void Commands::pass_handler(const Message& msg, Client* user, Server* server) 
{
	
	std::vector<std::string> params = msg.getParams();
	if (user->isAuthenticated()){
		sendReply(user->getClientFd(), server->NAME, "462", user->getNickname(), "PASS", "Already registered");
		return ;
	}
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "PASS", "Not enough parameters");
		return ;
	}
	if (!server->authenticate(params[0], user))
	{
		sendReply(user->getClientFd(), server->NAME, "464", user->getNickname(), "", "Password incorrect");
		return ;
	}
	user->setAuthenticated();
	if (!user->getNickname().empty() && !user->getUsername().empty() && user->isAuthenticated())
	{
		user->setRegistered();
		sendReply(user->getClientFd(), server->NAME, "001", user->getNickname(), "", "Welcome to the IRC server");
	}
}

void Commands::user_handler(const Message& msg, Client* user, Server* server)
{
	std::vector<std::string> params = msg.getParams();
	if (user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "462", user->getNickname(), "USER", "Already registered");
		return ;
	}
	if(params.size() < 4){
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "USER", "Not enough parameters");
		return ;	
	}
	user->setUsername(params[0]);
	user->setRealname(params[3]);
	if (!user->getNickname().empty() && !user->getUsername().empty() && user->isAuthenticated())
	{
		user->setRegistered();
		sendReply(user->getClientFd(), server->NAME, "001", user->getNickname(), "", "Welcome to the IRC server");
	}
}

void Commands::list_handler(const Message& msg, Client* user, Server *server)
{
	for ()
}