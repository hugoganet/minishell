/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:01:21 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:12:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*empty_stash(char **stash, int j)
{
	char	*temp;

	if (!(*stash)[j])
	{
		free(*stash);
		return (NULL);
	}
	temp = ft_strdup(*stash + (j + 1));
	free (*stash);
	*stash = temp;
	return (temp);
}

static char	*set_line(char **stash)
{
	char	*line;
	int		j;

	j = 0;
	if (!*stash || !**stash)
		return (NULL);
	while ((*stash)[j] && (*stash)[j] != '\n')
		j++;
	if ((*stash)[j] == '\n')
		j++;
	line = ft_substr(*stash, 0, j);
	if (!line)
	{
		free(*stash);
		*stash = NULL;
		return (NULL);
	}
	*stash = empty_stash(stash, j - 1);
	return (line);
}

static char	*fill_buffer_and_line(int fd, char **stash)
{
	char	*buff;
	int		nb_bytes;
	int		i;

	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	nb_bytes = read(fd, buff, BUFFER_SIZE);
	if (nb_bytes == -1)
	{
		free(buff);
		return (NULL);
	}
	buff[nb_bytes] = '\0';
	*stash = free_strjoin(*stash, buff);
	free(buff);
	if (!*stash)
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	if ((*stash)[i] == '\n' || nb_bytes == 0)
		return (set_line(stash));
	return (fill_buffer_and_line(fd, stash));
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (!stash)
		stash = ft_strdup("\0");
	line = fill_buffer_and_line(fd, &stash);
	if (!line || !*stash)
	{
		free (stash);
		stash = NULL;
	}
	return (line);
}

// char	*get_next_line(int fd)
// {
// 	char		*line;
// 	static char	*stash;

// 	if (fd < 0 || BUFFER_SIZE < 0)
// 		return (NULL);
// 	if (!stash)
// 		stash = ft_strdup("\0");
// 	line = fill_buffer_and_line(fd, &stash);
// 	if (!line)
// 	{
// 		free (stash);
// 		stash = NULL;
// 	}
// 	return (line);
// }

// char	*fill_buffer_and_line(int fd, char **stash)
// {
// 	char	*buff;
// 	int		nb_bytes;
// 	int		i;

// 	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
// 	if (!buff)
// 		return (NULL);
// 	nb_bytes = read(fd, buff, BUFFER_SIZE);
// 	if (nb_bytes == -1)
// 	{
// 		free(buff);
// 		return (NULL);
// 	}
// 	buff[nb_bytes] = '\0';
// 	*stash = free_strjoin(*stash, buff);
// 	free(buff);
// 	if (!*stash)
// 		return (NULL);
// 	i = 0;
// 	while ((*stash)[i] && (*stash)[i] != '\n')
// 		i++;
// 	if ((*stash)[i] == '\n' || nb_bytes == 0)
// 		return (set_line(stash));
// 	return (fill_buffer_and_line(fd, stash));
// }

// char	*set_line(char **stash)
// {
// 	char	*line;
// 	int		j;

// 	j = 0;
// 	if (!*stash || !**stash)
// 		return (NULL);
// 	while ((*stash)[j] && (*stash)[j] != '\n')
// 		j++;
// 	if ((*stash)[j] == '\n')
// 		j++;
// 	line = ft_substr(*stash, 0, j);
// 	if (!line)
// 	{
// 		free(*stash);
// 		*stash = NULL;
// 		return (NULL);
// 	}
// 	*stash = empty_stash(stash, j - 1);
// 	return (line);
// }

// char	*empty_stash(char **stash, int j)
// {
// 	char	*temp;

// 	if (!(*stash)[j])
// 	{
// 		free(*stash);
// 		return (NULL);
// 	}
// 	temp = ft_strdup(*stash + (j + 1));
// 	free (*stash);
// 	*stash = temp;
// 	return (temp);
// }

// int main()
// {
//     int fd;
//     char    *next_line;

//     fd = open("exemple.txt", O_RDONLY);
//     if (fd == -1)
//         return (1);
//     while ((next_line = get_next_line(fd)) != NULL)
//     {
//         printf("%s", next_line);
//         free(next_line);
//     }
//     close(fd);
// }
