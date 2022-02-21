/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   puts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Вывод символа (используется только для вывода скрытого 127-го символа)
void	ft_putchar(int c)
{
	write(1, &c, 1);
}

//Вывод строки
void	ft_putstr(char *str, int std_var)
{
	int			i;
	static int	len_red;
	static int	len_reset;

	if (str == NULL)
		str = "(null)";
	i = 0;
	while (str[i])
		i++;
	if (!len_red || !len_reset)
	{
		len_red = ft_strlen(COLOR_RED);
		len_reset = ft_strlen(COLOR_RESET);
	}
	if (std_var == 2)
		write(std_var, COLOR_RED, len_red);
	write(std_var, str, i);
	if (std_var == 2)
		write(std_var, COLOR_RESET, len_reset);
}

//Вывод ошибок с выходом из программы
void	ft_puterror(t_shell *shell, int code, char *name)
{
	ft_putstr("Error: ", 2);
	if (code == 1)
		ft_putstr("Programmu neobhodimo zapuskat bez parametrov\n", 2);
	if (code == 2)
	{
		ft_putstr("Ne udalos vydelit pamyat. ", 2);
		ft_putstr(name, 2);
	}
	if (code == 3)
		ft_putstr("excve\n", 2);
	if (code == 4)
	{
		ft_putstr("incorrect variable. ", 2);
		ft_putstr(name, 2);
	}
	ms_shell_destroy(shell);
	exit(1);
}

//Вывод ошибок без выхода из программы
void	ft_puterror_noexit(t_shell *shell, int code)
{
	ft_putstr("Error: ", 2);
	if (code == 1)
		ft_putstr("Ne udalos voyti v ukazannuyu directoriyu\n", 2);
	shell->output_error = 1;
}

//Вывод сообщения о выходе
void	ft_putexit(t_shell *shell)
{
	ft_putstr(COLOR_GREEN, 1);
	ft_putstr("Exit: Vi pokinuli programmu. Prihodite eshe!!!\n", 1);
	ft_putstr(COLOR_RESET, 1);
	ms_shell_destroy(shell);
	exit(0);
}
