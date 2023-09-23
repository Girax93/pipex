/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrawe <agrawe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 16:34:20 by agrawe            #+#    #+#             */
/*   Updated: 2023/09/23 17:54:38 by agrawe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Counts the number of space-separated words in argv
int count_words(char *argv)
{
    int word_count;
    int i;

	i = 0;
	word_count = 1;
    while (argv[i])
    {
        if (argv[i] == ' ')
            word_count++;
        i++;
    }
    return (word_count);
}

// Extract a word bounded by single quotes
char *extract_quoted_word(char *argv, int *index)
{
    int start;

	start = *index + 1;
    *index = start;
    while (argv[*index] && argv[*index] != '\'')
        (*index)++;
    return (ft_substr(argv, start, *index - start));
}

// Extract a word not bounded by quotes
char *extract_word(char *argv, int *index)
{
    int start;

	start = *index;
    while (argv[*index] && argv[*index] != ' ')
        (*index)++;
    return (ft_substr(argv, start, *index - start));
}

// 1. Counts the number of words in the argument, separated by a " " character.
// 2. Extracts the command(s) if they are inside single quotation marks
//    - NB! Bash handles double quotation marks by itself.
// 3. Extracts the command itself if there are no single quotation marks.
// Returns an array of strings with the commands to be executed.
char **cmd_with_single_quotes_parser(char *argv)
{
    char **cmd;
    int i;
    int j;
    int word_count;

    i = 0;
    j = 0;
    word_count = count_words(argv);
    cmd = malloc(sizeof(char *) * (word_count + 1));
    if (!cmd)
        return (NULL);
    while (argv[i])
    {
        if (argv[i] == '\'')
            cmd[j] = extract_quoted_word(argv, &i);
        else
            cmd[j] = extract_word(argv, &i);
        j++;
        if (argv[i] == ' ')
            i++;
    }
    cmd[j] = 0;
    return (cmd);
}

// 1. Checks if the argument is wrapped in single quotation marks.
// 2. If it is, then it runs the "cmd_with_single_quotes_parser" command on it
// 3. If it is NOT, then it splits the argument based on spaces.
// Returns the parsed command as an array of strings. (**cmd).
char **parse_cmd_based_on_quotes(char *argv)
{
    char **cmd;
    int i;

    i = 0;
    while (argv[i] != '\'' && argv[i])
        i++;
    if (argv[i] == '\'' && argv[ft_strlen(argv) - 1] == '\'')
        cmd = cmd_with_single_quotes_parser(argv);
    else
        cmd = ft_split(argv, ' ');
    return cmd;
}