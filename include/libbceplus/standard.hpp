/*
 *  Copyright (C) 2012-2013, XiaoJSoft Studio. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */

#ifndef __STANDARD_HPP__
	#define __STANDARD_HPP__

	/*  Standards for common modules  */
	#define STANDARD_ZERO_CHAR                      '0'
	#define STANDARD_ZERO_STRING                    "0"
	#define STANDARD_PLUS_CHAR                      '+'
	#define STANDARD_PLUS_STRING                    "+"
	#define STANDARD_MINUS_CHAR                     '-'
	#define STANDARD_MINUS_STRING                   "-"

	/*  Standards for brackets  */
	#define STANDARD_BRACKET_BUILD_START            "("
	#define STANDARD_BRACKET_BUILD_END              ")"
	#define STANDARD_BRACKET_START_A                '('
	#define STANDARD_BRACKET_START_B                '['
	#define STANDARD_BRACKET_START_C                '{'
	#define STANDARD_BRACKET_END_A                  ')'
	#define STANDARD_BRACKET_END_B                  ']'
	#define STANDARD_BRACKET_END_C                  '}'

	/*  Standards for 'polynm_unknown' class  */
	#define STANDARD_PUK_SYMBOL_PREFIX              "X"
	#define STANDARD_PUK_SYMBOL_DICTIONARY          "abcdefghijklmnopqrstuvwxyz"
	#define STANDARD_PUK_SYMBOL_DICTIONARY_LENGTH   26

	/*  Standards for replacement-table  */
	#define STANDARD_STATUS_NONE                    -1
	#define STANDARD_STATUS_GAS                     0
	#define STANDARD_STATUS_LIQUID                  1
	#define STANDARD_STATUS_SOLID                   2
	#define STANDARD_STATUS_AQUEOUS                 3

	/*  Standards for fraction module  */
	#define STANDARD_FRACTION_COMPARE_A_MIN         -1
	#define STANDARD_FRACTION_COMPARE_EQUAL         0
	#define STANDARD_FRACTION_COMPARE_B_MIN         1
	#define STANDARD_FRACTION_SEMICOLON             "/"
	#define STANDARD_FRACTION_SEMICOLON_CHAR        '/'
	#define STANDARD_FRACTION_NEGATIVE              "-"

	/*  Standards for molecular parser  */
	#define STANDARD_PARSER_HYDRATE_DOT_STRING      "."
	#define STANDARD_PARSER_HYDRATE_DOT_CHAR        '.'

	/*  Standards for syntax parser  */
	#define STANDARD_SYNTAX_SEPARATOR_CHAR          ';'
	#define STANDARD_SYNTAX_SEPARATOR_STRING        ";"
	#define STANDARD_SYNTAX_EQUAL_CHAR              '='
	#define STANDARD_SYNTAX_EQUAL_STRING            "="



#define STANDARD_RPT_ELECTRONIC_POSITIVE "e+"
#define STANDARD_RPT_ELECTRONIC_POSITIVE_LENGTH 2
#define STANDARD_RPT_ELECTRONIC_NEGATIVE "e-"
#define STANDARD_RPT_ELECTRONIC_NEGATIVE_LENGTH 2
#endif
