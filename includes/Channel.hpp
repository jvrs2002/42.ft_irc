/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:03 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/15 18:44:52 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <set>

class Client;

class Channel
{
private:
	std::string				_channel_name;
	std::set<Client*>		_users;
	std::set<Client*>		_operators;
	
	// mandatory modes
	bool _invite_active;
	std::set<std::string> _invited;
	bool _topic_active;
	std::string	_topic;
	bool _password_active;
	std::string _password;
	bool _user_limit_active;
	int _user_limit;
		
public:
	Channel();
	Channel(const std::string& channel_name, Client* creator);
	Channel(const Channel& other);
	Channel& operator=(const Channel &other);
	~Channel();

	void	init(const std::string& channel_name, Client *creator);
	std::string getName() const;
	std::string	getTopic() const;
	bool	getInvite() const;
	bool	hasUser(Client *user) const;
	bool	hasTopic();
	bool	emptyChannel() const;
	bool	isOperator(Client *user);
	
	void	joinChannel(std::string prefix, Client* new_user, std::string password, std::string server_name);
	void	partChannel(std::string prefix, Client* user_delete, std::string reason);
	void	setTopicText(std::string prefix, Client* user, std::string topic, std::string server_name);
	void	kickUser(std::string prefix, Client* target, std::string reason);
	void	addInvite(std::string target_nick);
	void	ChannelMessage(std::string prefix, Client* sender, std::string command, std::string buffer);
	void	ChannelBroadcast(std::string prefix, Client* sender, std::string command, std::string buffer);
	void	ModeBroadcast(std::string prefix, Client* sender, std::string command, std::string buffer);
	void	removeUser(Client *user);



	void	setInvite(char c, std::string prefix, Client* user);
	void	setTopic(char c, std::string prefix, Client* user);
	void	setPassword(char c, std::string password, std::string prefix, Client* user); 
	void	setOperator(char sign, std::string nickname, std::string prefix, Client* user);
	void	setLimit(char sign, int max_size, std::string prefix, Client* user);
	

};



#endif
