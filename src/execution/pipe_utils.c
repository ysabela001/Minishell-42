/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggroff-d <ggroff-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:08:57 by ggroff-d          #+#    #+#             */
/*   Updated: 2025/03/19 14:52:34 by ggroff-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_heredoc(t_command *cmd)
{
	if (cmd->is_heredoc && cmd->heredoc_fd != -1 && !cmd->input_file)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
		{
			perror("heredoc dup2 failed\n");
			exit(1);
		}
		close(cmd->heredoc_fd);
	}
}

static void	setup_pipes(t_command *cmd, int *fd_in, int *pipe_fd)
{
	if (*fd_in != -1 && !cmd->is_heredoc)
	{
		dup2(*fd_in, STDIN_FILENO);
		close(*fd_in);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

void	exit_process(t_shell *shell, int exit_code)
{
	cleanup_shell(shell);
	exit(exit_code);
}

void	child_process(t_command *cmd, int *fd_in, int *pipe_fd,
			t_shell *shell)
{
	int	i;

	if (handle_redirections(cmd, shell) < 0)
		exit_process(shell, 1);
	setup_heredoc(cmd);
	setup_pipes(cmd, fd_in, pipe_fd);
	i = 3;
	while (i < 256)
	{
		if ((fd_in && *fd_in == i) || (pipe_fd && pipe_fd[1] == i)
			|| (cmd->is_heredoc && cmd->heredoc_fd == i)
			|| (cmd->input_file && cmd->input_fd == i)
			|| (cmd->output_file && cmd->output_fd == i))
		{
			i++;
			continue ;
		}
		close(i);
		i++;
	}
	if (!cmd->args || !cmd->args[0])
		exit_process(shell, 0);
	exit_process(shell, execute_command(cmd, shell));
}

void	parent_process(int *fd_in, int *pipe_fd, t_shell *shell)
{
	if (*fd_in != -1)
		close_and_untrack_fd(shell, fd_in);
	if (pipe_fd[1] != -1)
		close_and_untrack_fd(shell, &pipe_fd[1]);
	if (pipe_fd[0] != -1)
		*fd_in = pipe_fd[0];
	else
		*fd_in = -1;
}
