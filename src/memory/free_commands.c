/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggroff-d <ggroff-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:42:58 by ggroff-d          #+#    #+#             */
/*   Updated: 2025/03/18 17:50:46 by ggroff-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	int	i;

	if (!commands)
		return ;
	if (commands->args)
	{
		i = -1;
		while (commands->args[++i])
			free(commands->args[i]);
		free(commands->args);
	}
	free(commands->input_file);
	free(commands->output_file);
	free(commands->heredoc_delim);
	free(commands);
}

void	free_command_list(t_command *cmd_list)
{
	t_command	*current;
	t_command	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_commands(current);
		current = next;
	}
}
