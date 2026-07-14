/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppassos <ppassos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:54:26 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/14 13:32:00 by ppassos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef utils_get_HPP
# define utils_get_HPP

# include <string>
# include <sys/socket.h>
# include <sstream>
# include <netinet/in.h>

std::string	intToString(int number);
void*		utils_get_in_addr(struct sockaddr *sa);
std::string	utils_get_port_str(struct sockaddr *sa);
void sendReply(int Clientfd, const std::string& server, const std::string& code, 
const std::string& target, const std::string& params, const std::string& trailing); //por apagar
//void sendsReply(int Clientfd, const std::string& msg);// nova por aplicar no codigo	


#endif
