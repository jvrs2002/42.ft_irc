/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:54:26 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/27 19:07:46 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef utils_get_HPP
# define utils_get_HPP

# include "Server.hpp"
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>

void*		utils_get_in_addr(struct sockaddr *sa);
std::string	utils_get_port_str(struct sockaddr *sa);

#endif
