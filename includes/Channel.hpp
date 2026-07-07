/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:03 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/07 16:28:23 by joao-vri         ###   ########.fr       */
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

	void	joinChannel(const std::string& prefix, Client* new_user, const std::string& password, const std::string& server_name);
	void	partChannel(const std::string& prefix, Client* user_delete, const std::string& reason);
	void	setTopicText(const std::string& prefix, Client* user, const std::string& topic, const std::string& server_name);
	void	kickUser(const std::string& prefix, Client* target, const std::string& reason);
	void	addInvite(const std::string& target_nick);

	std::string getName() const;
	std::string	getTopic() const;
	bool		getInvite() const;
	bool		hasUser(Client *user) const;
	bool		hasTopic();
	bool		emptyChannel() const;
	void		removeUser(Client *user);
	bool		isOperator(Client *user);

	void	setInvite(char c, const std::string& prefix);
	void	setTopic(char c, const std::string& prefix);
	void	setPassword(char c, const std::string& password, const std::string& prefix); 
	void	setOperator(char sign, const std::string& nickname, const std::string& prefix);
	void	setLimit(char sign, int max_size, const std::string& prefix);
	
	void	ChannelMessage(const std::string& prefix, Client* sender, const std::string& command, const std::string& buffer);
	void	ChannelBroadcast(const std::string& prefix, const std::string& command, const std::string& buffer);
	void	ModeBroadcast(const std::string& prefix, const std::string& command, const std::string& buffer);
	const	std::set<Client*>&getUsers() const;
};



#endif
