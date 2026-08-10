/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:09:58 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/30 13:12:13 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string intToString(int number)
{
	std::stringstream	ss;
	ss << number;
	return ss.str();
}

// Helper function to get the socket address (IPv4 or IPv6)
void *utils_get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Helper function to get the socket port
std::string utils_get_port_str(struct sockaddr *sa)
{
	int port = 0;

	if (sa->sa_family == AF_INET) {
		port = ntohs(((struct sockaddr_in*)sa)->sin_port);
	}
	else {
		port = ntohs(((struct sockaddr_in6*)sa)->sin6_port);
	}

	return intToString(port);
}

void sendReply(int Clientfd, const std::string& server, const std::string& code, 
	const std::string& target, const std::string& params, const std::string& trailing)
{
	std::string msg = ":" + server + " " + code + " " + target + (params.empty() ? "" : " " + params) + " :" + trailing + "\r\n";
	send(Clientfd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

bool validChannelName(const std::string& name)
{
	bool i = true;
	i = name.size() > 1
		&& name[0] == '#'
		&& name.find(' ') == std::string::npos
		&& name.find(',') == std::string::npos;
	return (i);
}

std::string toLower(const std::string& str)
{
	std::string result = str;
	for (std::size_t j = 0; j < result.size(); ++j)
		result[j] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[j])));
	return (result);
}
