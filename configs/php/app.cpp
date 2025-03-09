/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:13:52 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/09 01:24:27 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int ac, char **av, char **env) {
	
	int pid = fork();
	char *path = "/usr/bin/php";
	char *args[] = {"php", "index.php", NULL};
	setenv("QUERY_STRING", "NAME=YourName", 1);  
	if (pid == 0) {
		execve(path, args, env);
	}
	else {
		waitpid(pid, NULL, 0);
	}
	return (0);
}
