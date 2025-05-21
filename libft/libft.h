/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:49:07 by elaudrez          #+#    #+#             */
/*   Updated: 2025/04/07 14:01:00 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
// # include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <string.h>
# include <stdarg.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 12
# endif

// size_t	ft_strlen(const char *str);
void	ft_bzero(void *s, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
char	*ft_itoa(int n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memset(void *b, int c, size_t len);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(const char *s, char c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
int		ft_tolower(int c);
int		ft_toupper(int c);
// int		ft_strlen(const char *str);
// char	*ft_strjoin(const char *s1, char *s2);
char	*free_strjoin(char *s1, char *s2);
// char	*ft_strdup(const char *s1);
// char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*get_next_line(int fd);
// char	*fill_buffer_and_line(int fd, char **stash);
// char	*set_line(char **stash);
// char	*empty_stash(char **stash, int j);
int		ft_putnbr_base1(unsigned long nb, char *base);
int		ft_putnbr_base2(long nb, char *base);
int		ft_printf(const char *format, ...);
int		ft_printa(unsigned long nb, char *base);
int		ft_putchar(char c);
int		ft_putstr(char *str);

#endif