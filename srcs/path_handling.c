/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrawe <agrawe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 20:49:41 by bsengeze          #+#    #+#             */
/*   Updated: 2023/09/23 18:45:15 by agrawe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"



// 1. Parses the argv (cmd) based on whether it is wrapped in quotes or not
// 2. Finds the path to the command to be executed
// 3. Frees and exits the program if path doesn't exist
// 4. Executes the command together with its path using execve.
//    - This replaces the current process with the command (other program)
void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = parse_cmd_based_on_quotes(argv);
	path = path_finder(cmd[0], envp);
	if (!path)
	{
		free_command(cmd);
		error_exit();
	}
	if (execve(path, cmd, envp) == -1)
		error_exit();
}

// Extracts the PATH from the envp.
// For a better understanding - type "env" into the terminal
// strnstr returns 0 when PATH is found. So if it doesn't return 0,
// it checks the next string instead. It then returns envp[i] + 5, to
// skip the first 5 characters, which are "PATH=".
char *extract_path_from_envp(char **envp)
{
    int i;

	i = 0;
    while (ft_strnstr(envp[i], "PATH", 4) == 0)
        i++;
    return (envp[i] + 5);
}

// 1. Adds the last "/" character to the path of the program
// 2. Then adds the program to the end of the path, which completes it
// 3. Frees the now unused memory of path_except_cmd
// 4. Checks with access() to see if the program will be successfully accessed
//    With the given path. Returns the full path if it will.
// 5. If access() doesn't return successfully, it frees the memory of the path,
//    and runs step 1-4 again, with i++ (the next possible path candidate).
// Returns NULL, or the successfully accessed path if found.
char *find_cmd_in_paths(char **dir_paths, char *cmd)
{
    int i = 0;
    char *path_except_cmd;
    char *path;
    while (dir_paths[i])
    {
        path_except_cmd = ft_strjoin(dir_paths[i], "/");
        path = ft_strjoin(path_except_cmd, cmd);
        free(path_except_cmd);
        if (access(path, X_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    return (NULL);
}

// 1. First extracts the PATH from the envp variable.
// 2. Then splits the different paths based on the : character to retrieve a
//    list of full paths to possible directories where the program might be in
// 3. Then uses find_cmd_in_paths to find which of the paths the binary is in.
// 4. Lastly, it frees the memory of the strings (paths) created,
//    And frees the pointer to those strings itself.
// Returns: the full path to the program to be executed.
char *path_finder(char *cmd, char **envp)
{
    char	**dir_paths;
    char	*path;
    char	*full_path;
	int		i;

	path = extract_path_from_envp(envp);
    dir_paths = ft_split(path, ':');
	full_path = find_cmd_in_paths(dir_paths, cmd);
	i = 0;
    while (dir_paths[i])
    {
        free(dir_paths[i]);
        i++;
	}
    free(dir_paths);
    return (full_path);
}
