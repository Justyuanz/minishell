/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:16:45 by jinzhang          #+#    #+#             */
/*   Updated: 2025/05/02 16:48:14 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	size_t	len;
	size_t	i;
	size_t	word;

	i = 0;
	word = 0;
	len = ft_strlen(s);
	while (i < len)
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			word++;
		i++;
	}
	return (word);
}

static size_t	word_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] != c && s[len] != '\0')
	{
		len++;
	}
	return (len);
}

static void	*ft_free(char **arr, int index)
{
	while (index >= 0)
	{
		free(arr[index]);
		arr[index] = NULL;
		index--;
	}
	free(arr);
	arr = NULL;
	return (NULL);
}

static char	**make_arr(char **arr, size_t totalwords, const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	wordlen;

	i = 0;
	j = 0;
	wordlen = 0;
	while (i < totalwords)
	{
		while (s[j] == c)
			j++;
		wordlen = word_len(s + j, c);
		arr[i] = ft_substr(s + j, 0, wordlen);
		j += wordlen;
		if (!arr[i])
			return (ft_free(arr, i));
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	totalwords;

	if (!s)
		return (NULL);
	totalwords = count_words(s, c);
	arr = malloc((totalwords + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	return (make_arr(arr, totalwords, s, c));
}
