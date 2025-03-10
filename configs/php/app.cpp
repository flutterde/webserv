/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:13:52 by ochouati          #+#    #+#             */
/*   Updated: 2025/03/09 16:26:29 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>  // For waitpid
#include <string.h>    // For strdup

int main(int ac, char **av, char **env) {
    std::vector<char *> envs;
    
    // Copy existing environment variables
    for (int i = 0; env[i]; i++) {
        envs.push_back(env[i]);
    }
    
    // Add PHP specific environment variables to simulate $_GET
    envs.push_back(strdup("PHP_VALUE=variables_order=EGPCS"));
    envs.push_back(strdup("REQUEST_METHOD=GET"));
    
    // We're going to pass the NAME parameter directly as an argument
    // This will make it available through argv in PHP
    
    // Null terminator for environment array
    envs.push_back(NULL);
    
    int pid = fork();
    char *path = "/usr/bin/php";
    
    // Pass the GET parameter as an argument
    char *args[] = {"php", "-d", "register_argc_argv=1", "-d", "auto_globals_jit=0", 
                    "-r", "parse_str('NAME=ochouati', $_GET); include('index.php');", NULL};
    
    if (pid == 0) {
        execve(path, args, envs.data());
        // If execve returns, it failed
        perror("execve failed");
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
    
    // Free allocated memory
    for (size_t i = 0; i < envs.size() - 1; i++) {
        if (strncmp(envs[i], "PHP_VALUE=", 10) == 0 ||
            strncmp(envs[i], "REQUEST_METHOD=", 15) == 0) {
            free(envs[i]);
        }
    }
    
    return 0;
}