/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:54:36 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/04 00:23:15 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/*
 * 파서는 주로 문자로 된 입력 데이터를 받아 자료 구조를 만들어 내는 컴포넌트이다.
 * 해당 자료 구조의 형태 다양하게 존재하는데 parse tree, abstract syntax tree, 이외의 다른 계층 구조로 일 수 있다.
 * 파서는 자료 구조를 만들면서 구조화된 표현을 더하기도, 구문이 올바른지 검사하기도 한다.
 * 보통은 파서 앞에 어휘 분석기(Lexer)를 따로 둔다.
 */

/*
 * 추상 구문 트리를 통해 파서를 작성할 예정이다.
 * 추상 구문 트리라는 표현은 소스코드에서 보이는 세부 정보가 생략되기 때문이다. (ex: ", ', (, ), ...)
 * 이런 세부 정보는 파서가 AST를 구성할 때 구성 형태를 알려주는 용도로만 사용한다.
 * 주목할 점은 보편적은 AST 형식은 없다는 점이다.
 * 각각의 구현체는 모두 비슷하고 개념도 같지만, 세부적인 면에서는 파싱할 대상에 따라 구체적인 구현이 달라진다.
 */
# include "token/token.h"

enum e_command {
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ILLEGAL
};

/*
 * $> <command> <option> <arguments>
 * Command {
 * 		cmd: echo,
 * 		option: -n,
 * 		argument: "hello world!"
 * }
 */

typedef struct s_parser {
	struct s_lexer_lst	*l;
	struct s_token_lst	*cur_t;
} t_parser;

typedef struct s_command {
	enum e_command		cmd;
	char				*option;
	char				*argument;
} t_command;

#endif
