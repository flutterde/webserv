/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:18:20 by ochouati          #+#    #+#             */
/*   Updated: 2025/04/23 13:01:56 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <dirent.h>

void	deleteFile(const std::string& path)
{
	if (std::remove(path.c_str()) != 0)
		std::cerr << "Error deleting file: " << path << std::endl;
}

void deleteFolder(const std::string& path)
{
    DIR* dir = opendir(path.c_str());

    if (!dir)
    {
        std::cerr << "Error opening directory: " << path << std::endl;
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            continue;
        std::string filePath = path + "/" + entry->d_name;
        if (entry->d_type == DT_DIR)
            deleteFolder(filePath);
        else
            deleteFile(filePath);
    }
    closedir(dir);
    if (std::remove(path.c_str()) != 0)
        std::cerr << "Error deleting directory: " << path << std::endl;
}

int	main() {
	deleteFolder("./myfld");
}
