/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:53:29 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
#define SYNTAX_H

#include "minishell.h"

// !===========================================================================
// !                           VALIDATION MODULE                             =
// !===========================================================================

// validation/input_validation.c
int is_line_empty(char *input);
int has_unclosed_quotes(char *input);

// validation/pipe_validation.c
int has_invalid_pipes(char *input);

// validation/redirection_validation.c
int has_invalid_redirections(char *input);

// validation/parentheses_validation.c
int has_unmatched_parentheses(char *input);

// validation/braces_validation.c
int has_unclosed_braces(char *input);

// validation/validation.c
int is_syntax_valid(char *input, t_shell *shell);

// !===========================================================================
// !                         TOKENIZATION MODULE                             =
// !===========================================================================

// tokenization/token_parsing.c
void skip_spaces(char *input, int *i);
bool is_token_delim(char c);
char *read_word_segment(char *input, int *i);
char *join_and_free(char *result, char *segment);

// tokenization/token_reading.c
char *read_next_segment(char *input, int *i);
char *read_complete_token(char *input, int *i);
char *read_operator(char *input, int *i);
char *read_redir_and_file(char *input, int *i);

// tokenization/tokenize_core.c
t_token *tokenize(char *input);

// tokenization/token_type_utils.c
t_token_type get_token_type(char *str);
bool is_redirection(t_token_type type);
bool is_logical_operator(t_token_type type);

// tokenization/token_validation.c
char *parse_quoted_token(char *input, int *i);
int print_syntax_error(char *token);
int validate_token_sequence(t_token *head);

// tokenization/build_token_list.c
t_token *token_new(char *str, t_token_type type);
void append_token(t_token **head, t_token **last, t_token *new);

// !===========================================================================
// !                           PARSING MODULE                                =
// !===========================================================================

// parsing/refine_token_type.c
void refine_token_types(t_token *head);

// parsing/logic_groups.c
t_token *is_logic(t_token **node);

// !===========================================================================
// !                             CORE MODULE                                 =
// !===========================================================================

// core/syntax_utils.c
void update_quote_state(char *quote_state, char c);
int is_parenthesis_empty(char *input, int i);

#endif
