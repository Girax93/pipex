/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrawe <agrawe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:02:28 by agrawe            #+#    #+#             */
/*   Updated: 2023/09/23 18:02:31 by agrawe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/wait.h>
# include <fcntl.h>

// utils.c
int		open_file(char *argv, int i);
void	new_process(char *argv, char **envp);
void	here_doc_child_process(int *fd, char *limiter);
void	new_parent_process(int *pipefd, pid_t child_pid);
void	new_child_process(int *pipefd, char *argv, char **envp);

// path_handling.c
void	execute(char *argv, char **envp);
char	*path_finder(char *cmd, char **envp);
char	*find_cmd_in_paths(char **dir_paths, char *cmd);
char	*extract_path_from_envp(char **envp);
void	execute(char *argv, char **envp);

// exits.c
void	error_exit(void);
void	wrong_input(void);
void	free_command(char **cmd);

// parsing.c
char	**cmd_with_single_quotes_parser(char *argv);
char 	*extract_word(char *argv, int *index);
char	*extract_quoted_word(char *argv, int *index);
int		count_words(char *argv);
char	**parse_cmd_based_on_quotes(char *argv);

#endif