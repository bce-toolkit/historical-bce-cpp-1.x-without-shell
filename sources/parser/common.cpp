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

#include <string>
#include <iterator>
#include <cctype>
#include <libbceplus/standard.hpp>
#include <libbceplus/math/integer.hpp>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/parser/common.hpp>

using namespace std;
using namespace balancer::math;

namespace balancer {
namespace parser {

const string parseNumericPrefix(const string &src, \
                                string &remain) {
	string::const_iterator iterSRC;

	/*  Find the first non-digit character in source string  */
	for (iterSRC = src.cbegin(); iterSRC != src.cend(); ++iterSRC) {
		if (!isdigit(*iterSRC)) {
			break;
		}
	}

	/*  Cut the non-prefix part  */
	remain = src.substr(iterSRC - src.cbegin());

	/*  Return the prefix part  */
	return(src.substr(0, iterSRC - src.cbegin()));
}

const string parseNumericSuffix(const string &src, \
                                string &remain) {
	string::const_reverse_iterator iterSRC;

	/*  Find the last non-digit character in source string  */
	for (iterSRC = src.crbegin(); iterSRC != src.crend(); ++iterSRC) {
		if (!isdigit(*iterSRC)) {
			break;
		}
	}

	/*  Cut the non-suffix part  */
	remain = src.substr(0, src.crend() - iterSRC);

	/*  Return the suffix part  */
	return(src.substr(src.crend() - iterSRC));
}

bool parseFractionPrefix(const string &src, \
                         string &remain, \
                         fraction &result) {
	integer numberRead = 0;
	fraction build(0, 1);
	string tempExchange = "", \
	       tempRead = "", \
	       tempRemain = "";

	/*  Read the numerator part  */
	tempRead = parseNumericPrefix(src, tempRemain);

	/*  Convert string to integer  */
	if (tempRead.length() == 0) {
		if (tempRemain.length() > 0 && tempRemain[0] == STANDARD_FRACTION_SEMICOLON_CHAR) {
			return(false);
		}
		numberRead = 0;
	} else {
		stringToInteger(numberRead, tempRead);
	}

	/*  Set the numerator part  */
	build.setValue(numberRead, integer(1));

	while (tempRemain.length() > 0 && tempRemain[0] == STANDARD_FRACTION_SEMICOLON_CHAR) {
		/*  Read the denominator part  */
		tempRead = parseNumericPrefix(tempRemain.substr(1), tempExchange);

		/*  Convert string to integer  */
		if (tempRead.length() == 0) {
			return(false);
		} else {
			stringToInteger(numberRead, tempRead);
		}

		/*  Fail if divides with zero  */
		if (numberRead == 0) {
			return(false);
		}

		/*  Set the denominator part  */
		build /= fraction(numberRead, integer(1));
		tempRemain = tempExchange;
	}

	/*  Set remain part and the result  */
	remain = tempRemain;
	result = build;

	return(true);
}

bool parseFractionSuffix(const string &src, \
                         string &remain, \
                         fraction &result) {
	string dummyString;
	string::const_reverse_iterator iterSRC;

	/*  Find the last non-digit character in source string  */
	for (iterSRC = src.crbegin(); iterSRC != src.crend(); ++iterSRC) {
		if (*iterSRC != STANDARD_FRACTION_SEMICOLON_CHAR && !isdigit(*iterSRC)) {
			break;
		}
	}

	/*  Cut the non-suffix part  */
	remain = src.substr(0, src.crend() - iterSRC);

	/*  Return the suffix part  */
	return(parseFractionPrefix(src.substr(src.crend() - iterSRC), dummyString, result));
}

} /*  namespace parser  */
} /*  namespace balancer  */
