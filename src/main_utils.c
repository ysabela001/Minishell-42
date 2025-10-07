/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggroff-d <ggroff-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:46:45 by ggroff-d          #+#    #+#             */
/*   Updated: 2025/03/19 12:06:13 by ggroff-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*process_input(char *input, t_shell *shell)
{
	t_token		*tokens;

	tokens = tokenize(shell, input);
	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens, shell))
	{
		free_token_list(tokens);
		shell->exit_status = 2;
		return (NULL);
	}
	if (shell->commands)
		free_command_list(shell->commands);
	shell->commands = parse_tokens(tokens, shell);
	free_token_list(tokens);
	if (!shell->commands)
		return (shell->exit_status = 2, NULL);
	execute_single_command(shell->commands, shell);
	free_command_list(shell->commands);
	shell->commands = NULL;
	return (NULL);
}

char	*get_user_input(void)
{
	char			*input;
	struct termios	old_term;
	struct termios	new_term;

	tcgetattr(STDIN_FILENO, &old_term);
	new_term = old_term;
	new_term.c_lflag |= ISIG;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	rl_on_new_line();
	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	return (input);
}

int	is_empty_input(const char *input)
{
	while (*input)
	{
		if (!is_whitespace(*input))
			return (0);
		input++;
	}
	return (1);
}

static void	handle_input(t_shell *shell, char *input)
{
	t_command	*commands;

	if (is_empty_input(input))
	{
		free(input);
		return ;
	}
	commands = process_input(input, shell);
	shell->expand[0] = '\0';
	free(input);
}

void	shell_loop(t_shell *shell)
{
	char	*input;
	int		exit_status;

	while (1)
	{
		set_signal_state(0);
		setup_signal_handlers();
		rl_on_new_line();
		input = get_user_input();
		if (!input)
		{
			write(1, "exit\n", 5);
			exit_status = shell->exit_status;
			cleanup_shell(shell);
			exit(exit_status);
		}
		if (get_signal_state() == SIGINT)
		{
			shell->exit_status = 130;
			free(input);
			continue ;
		}
		handle_input(shell, input);
	}
}
