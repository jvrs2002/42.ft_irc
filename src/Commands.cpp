/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/18 12:15:00 by manelcarval      ###   ########.fr       */
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
}

Commands::~Commands()
{

}

void Commands::Commandhandler(Message msg, Client* user, Server* server) 
{
	_handler[msg.getCommand()] (msg, user, server);
}

// static void sendReply(int Clientfd, const std::string& server, const std::string& code, 
// 	const std::string& target, const std::string& params, const std::string& trailing) 
// {
// 	std::string msg = ":" + server + " " + code + " " + target + " " + params + " :" + trailing + "\r\n";
// 	send(Clientfd, msg.c_str(), msg.size(), 0);
// }

void Commands::join_handler(Message msg, Client* user, Server* server)
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "JOIN", "Not enough parameters");
		return ;
	}

	std::string channel_name = params[0];
	Channel*	channel = server->getChannel(channel_name);
	std::string password = (params.size() > 1) ? params[1] : "";

	if (channel_name.size() <= 1 || channel_name[0] != CHANNEL)
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (channel != NULL) 
	{
		if (!user->addToChannel(channel))
		{
			sendReply(user->getClientFd(), server->NAME, "443", user->getNickname(), channel_name, "User is already in channel");
			return ;
		}
		channel->joinChannel(msg.getPrefix(), user, password, server->NAME);
	}
	else {
		if (!server->createChannel(channel_name, user))
		{ 	//Correct error?
			sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
			return ;
		}
		channel = server->getChannel(channel_name);
		if (!user->addToChannel(channel))
		{	//Correct error?
			sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), channel_name, "Error joining channel");
			return ;
		}
		channel->joinChannel(msg.getPrefix(), user, password, server->NAME);
	}
}

void Commands::part_handler(Message msg, Client* user, Server* server) 
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
	std::string channel_name = params[0];
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

void Commands::privmsg_handler(Message msg, Client* user, Server* server) 
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

	std::string target = params[0];
	std::string message = params[1];
	if (target[0] == CHANNEL)
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
		send(target_fd, priv_msg.c_str(), priv_msg.size(), 0);
	}

}

void Commands::notice_handler(Message msg, Client* user, Server* server) 
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();

	if (params.size() < 1)
	{
		sendReply(user->getClientFd(), server->NAME, "411", user->getNickname(), "NOTICE", "No recipient given");
		return ;
	}
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "412", user->getNickname(), "", "No text to send");
		return ;
	}

	std::string target = params[0];
	std::string message = params[1];
	if (target[0] == CHANNEL)
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
		channel->ChannelMessage(msg.getPrefix(), user, " NOTICE ", message);
	}
	else
	{

		int target_fd = server->getClientFd(target);
		if (target_fd == -1)
		{
			sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target, "No such nick");
			return ;
		}
		std::string notice_msg = msg.getPrefix() + " NOTICE " + target + " :" + message + "\r\n";
		send(target_fd, notice_msg.c_str(), notice_msg.size(), 0);
	}

}

void Commands::mode_handler(Message msg, Client* user, Server* server) 
{
	if (!user->isRegistered()){
		sendReply(user->getClientFd(), server->NAME, "451", user->getNickname(), "", "You have not registered");
		return ;
	}
	std::vector<std::string> params = msg.getParams();
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "MODE", "Not enough parameters");
		return ;
	}
	std::string channel_name = params[0];
	std::string modestring = params[1];
	Channel* channel = server->getChannel(channel_name);
	if (!channel)
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return;
	}
	if (!channel->isOperator(user))
	{
		sendReply(user->getClientFd(), server->NAME, "482", user->getNickname(), channel_name, "You're not a channel operator");
		return ;
	}
	char sign = '+';
	int arg_index = 2;
	std::string password = "";
	std::string nickname = "";
	int max_size = 0;
	for (int i = 0; modestring[i] != '\0'; i++) {
		if (modestring[i] == '+' || modestring[i] == '-')
			sign = modestring[i++];
		switch (modestring[i])
		{
			case 'i':
				channel->setInvite(sign, msg.getPrefix(), user);
				break;
			case 't':
				channel->setTopic(sign, msg.getPrefix(), user);
				break;
			case 'k':
				if (arg_index < (int)params.size() && sign == '+') {
					password = params[arg_index++];
					channel->setPassword(sign, password, msg.getPrefix(), user);
				}
				else if (sign == '-')
					channel->setPassword(sign, password, msg.getPrefix(), user);
				break;
			case 'o':
				if (arg_index < (int)params.size()) {
					nickname = params[arg_index++];
					channel->setOperator(sign, nickname, msg.getPrefix(), user);
				}
				break;
			case 'l':				
				if (arg_index < (int)params.size() && sign == '+') {
					max_size = atoi(params[arg_index++].c_str());
					channel->setLimit(sign, max_size, msg.getPrefix(), user);
				}
				else if (sign == '-')
					channel->setLimit(sign, max_size, msg.getPrefix(), user);
				break;
			default:
				break;
		}
	}
}

void	Commands::topic_handler(Message msg, Client* user, Server* server)
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
	std::string channel_name = params[0];
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
	std::string topic = params[1];
	channel->setTopicText(msg.getPrefix(), user, topic, server->NAME);
}

void	Commands::kick_handler(Message msg, Client* user, Server* server)
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
	std::string channel_name = params[0];
	std::string target_nick = params[1];
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

void Commands::invite_handler(Message msg, Client* user, Server* server)
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
	std::string target_nick = params[0];
	std::string channel_name = params[1];
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
	if (!channel->isOperator(user) && channel->getInvite())
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
		sendReply(user->getClientFd(), server->NAME, "443", user->getNickname(), target_nick, "is already on channel");
		return;
	}

	channel->addInvite(target_nick);

	sendReply(user->getClientFd(), server->NAME, "341", user->getNickname(), target_nick, channel_name);


	std::string invite_msg = msg.getPrefix() + " INVITE " + target_nick + " :" + channel_name + "\r\n";
	send(target->getClientFd(), invite_msg.c_str(), invite_msg.size(), 0);

	
}

//protocolist work(on going)

void Commands::nick_handler(Message msg, Client* user, Server* server) 
{
	std::string oldNick = user->getNickname();
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
	std::string rmsg = ":" + oldNick + "!" + user->getUsername() + "@" + user->getClientIP() + " NICK :" + user->getNickname() + "\r\n";
	user->setNickname(params[0]);
	if (user->isRegistered())
		user->Cbroadcast(rmsg);//comunicar com  todos os clientes que foi mudado o nick do cliente que esta ligado pelos canais 
	if (!(user->isRegistered()) && !user->getNickname().empty() && !user->getUsername().empty() && user->isAuthenticated())
    	user->setRegistered();
}

void Commands::pass_handler(Message msg, Client* user, Server* server) 
{
	///o get nickname ou user tem de vericar se exite ou nao porque caso nao exista tenho de mandar unknow ou '*' como o nick ou user (if(empty) = '*' || = unknow)
	std::vector<std::string> params = msg.getParams();
	if (user->isAuthenticated()){ //fazer funcao
		sendReply(user->getClientFd(), server->NAME, "462", user->getNickname(), "PASS", "Already registered"); //independente de estar registrado ou nao, tenho mandar esta mensagem se estiver autenthicado
		return ;
	}
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "PASS", "Not enough parameters");
		return ;
	}
	std::string params_password = params[0];
	if (!server->authenticate(params[0], user))
	{
		sendReply(user->getClientFd(), server->NAME, "464", user->getNickname(), "", "Password incorrect");
		return ;
	}
	user->setAuthenticated();	//fazer funcao
	if (!user->getNickname().empty() && !user->getUsername().empty() && user->isAuthenticated())
    	user->setRegistered();
}

void Commands::user_handler(Message msg, Client* user, Server* server)
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
    	user->setRegistered();
}
//falta as funcoes: hasNick() hasUser()
//ATENCAO!!! -> estado do cliente tem prioridade sobre parsing leve
//authenticado = password aceite que o user deu ou o sv nao tem pass
//registrado = com um user valido e um nick porem diferente de todos os presentes