/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:54:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:37:04 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "tokenizer/tokenizer.h"

typedef struct s_argument {
	enum e_token_type	next_token_type;
	char				**pa_argument;
} t_argument;

#endif
