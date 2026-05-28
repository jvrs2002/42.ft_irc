/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:09:58 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/28 11:27:39 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// Helper function to get the socket address (IPv4 or IPv6)
static void *utils_get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Helper function to get the socket port
static std::string utils_get_port_str(struct sockaddr *sa)
{
	int port = 0;

	if (sa->sa_family == AF_INET) {
		port = ntohs(((struct sockaddr_in*)sa)->sin_port);
	}
	else {
		port = ntohs(((struct sockaddr_in6*)sa)->sin6_port);
	}

	return std::to_string(port);
}

void sendReply(int Clientfd, const std::string& server, const std::string& code, 
	const std::string& target, const std::string& params, const std::string& trailing) 
{
	std::string msg = ":" + server + " " + code + " " + target + " " + params + " :" + trailing + "\r\n";
	send(Clientfd, msg.c_str(), msg.size(), 0);
}
