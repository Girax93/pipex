/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrawe <agrawe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:19:11 by agrawe            #+#    #+#             */
/*   Updated: 2023/09/23 17:30:17 by agrawe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// 1. Opens a pipe
// 2. Makes the parent simply wait for the child process,
//    while the child process reads from input and writes to the pipe until EOF
// 3. The parent duplicates the STDIN, and replaces the STDIN with pipefd[0]
//    the write end of the pipe (pipefd[1]) is closed as it is not used.
void	here_doc(char *limiter)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		error_exit();
	pid = fork();
	if (pid == 0)
		here_doc_child_process(pipefd, limiter);
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		wait(NULL);
	}
}

// Opens a file with the name specified as the last input argument of argv
// Makes it so that the here_doc becomes the input (terminal)
// Runs every agrument as a new process in a while loop.
// Replaces STDOUT with fileout (the last argument).
// Executes the last command, which outputs its output (stdout) to the new file
void handle_here_doc(char **argv, int argc, char **envp)
{
    int i;
    int fileout;

    i = 3;
    fileout = open_file(argv[argc - 1], 0);
    here_doc(argv[2]);
    while (i < argc - 2) {
        new_process(argv[i], envp);
        i++;
    }
    dup2(fileout, STDOUT_FILENO);
    execute(argv[argc - 2], envp);
}

// Opens a file for read only (the infile specified in argument 1)
// Replaces STDIN with the infile, so that this becomes the input for the cmds.
// Opens a file to be written to (truncated to), which is the output file.
// Opens a new process for every argument in the while loop
// Replaces STDOUT with the outfile since it will be written to
// Executes the last command, which outputs its output to the new stdout.
void handle_normal_pipes(char **argv, int argc, char **envp)
{
    int i;
    int filein;
    int fileout;

    i = 2;
    filein = open_file(argv[1], 2);
    dup2(filein, STDIN_FILENO);
    fileout = open_file(argv[argc - 1], 1);
    while (i < argc - 2)
	{
        new_process(argv[i], envp);
        i++;
    }
    dup2(fileout, STDOUT_FILENO);
    execute(argv[argc - 2], envp);
}

// If there are 5 or more arguments run the program
// If not, print error messages.
// If the first argument is "here_doc", runs the bonus.
int main(int argc, char **argv, char **envp)
{
    if (argc >= 5)
	{
        if (ft_strncmp(argv[1], "here_doc", 8) == 0)
            handle_here_doc(argv, argc, envp);
        else
            handle_normal_pipes(argv, argc, envp);
    }
	else
        wrong_input();
}
