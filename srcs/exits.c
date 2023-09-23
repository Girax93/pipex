/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrawe <agrawe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 15:37:04 by agrawe            #+#    #+#             */
/*   Updated: 2023/09/23 17:33:05 by agrawe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Prints error and exits the program
void	error_exit(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

// Prints an instruction manual on how to use pipex, then exits th program.
void	wrong_input(void)
{
	ft_putstr_fd("\x1b[31mError: \x1b[0mInvalid arguments\n", 2);
	ft_putstr_fd("\n\x1b[32mAccepted formats are: \x1b[0m\n", 1);
	ft_putstr_fd("/pipex file1 cmd1 cmd2 ... file2\n", 1);
	ft_putstr_fd("/pipex \"here_doc\" <LIMITER> cmd1 cmd2 ... outfile\n", 1);
	exit(EXIT_FAILURE);
}

// Frees the given command (an array of strings made up of the cmd argument)
void free_command(char **cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        free(cmd[i]);
        i++;
    }
    free(cmd);
}