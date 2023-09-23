/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrawe <agrawe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:17:46 by agrawe            #+#    #+#             */
/*   Updated: 2023/09/23 18:01:49 by agrawe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Opens a file with the following flags:
// 0 - Write Only, Create if file doesn't exist, Append (add) to the file
// 1 - Write Only, Create if file doesn't exist, Truncate (overwrite) the file
// 2 - Read only.
// Note: Every opened file is set to chmod 777 with 0777 (which is octal)
int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0777);
	if (file == -1)
		error_exit();
	return (file);
}

// 1. Closes the read-end of the pipe
// 2. Makes it so that the write-end of the pipe becomes STDOUT
//    - Remember: every process has its own set of FD's.
// 3. Executes the argument as a program found in the given path (envp).
void	new_child_process(int *pipefd, char *argv, char **envp)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	execute(argv, envp);
}

// 1. Closes the write-end of the pipe.
// 2. Makes the read-end the new STDIN for the current process
//    - This means the parent reads from the pipe that the child is writing to
//    - The child is writing the output of the program it executes.
//    - The program the child executes replaces the process,
//      Which releases the parent when it's finished.
// Waits for the corresponding child progress to finish it's job.
void	new_parent_process(int *pipefd, pid_t child_pid)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	waitpid(child_pid, NULL, 0);
}

// Simply creates a pipe, and a new process, and splits the child and parent
// into two different functions; new_child/parent_process.
void	new_process(char *argv, char **envp)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		error_exit();
	pid = fork();
	if (pid == -1)
		error_exit();
	if (pid == 0)
		new_child_process(pipefd, argv, envp);
	else
		new_parent_process(pipefd, pid);
}

// 1. Closes the read side of the pipe (pipefd[0])
// 2. Runs GNL on STDIN (terminal in this case) which returns line by line to
//    a string varible. This happens in an infinite loop.
// 3. If the line retrieved by GNL is equal to the limiter (EOF), it exits aka:
//    terminates itself (the child process).
// 4. If the line doesn't match, it writes the line to the parent via the pipe.
void	here_doc_child_process(int *pipefd, char *limiter)
{
	char	*line;

	close(pipefd[0]);
	line = NULL;
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 &&
			ft_strlen(limiter) == ft_strlen(line) - 1)
		{
			exit(EXIT_SUCCESS);
		}
		write(pipefd[1], line, ft_strlen(line));
	}
}