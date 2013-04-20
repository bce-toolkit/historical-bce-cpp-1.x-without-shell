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

#include <cctype>
#include <string>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/parser/common.hpp>
#include <libbceplus/parser/element.hpp>

using namespace std;
using namespace balancer::math;

namespace balancer {
namespace parser {

t_element::t_element() {
	symbol.clear();
	count.setValue(0, 1);
}

t_element::t_element(const t_element &src) {
	setValue(src.symbol, src.count);
}

t_element::t_element(const string &src_symbol, const fraction &src_count) {
	symbol = src_symbol;
	count = src_count;
}

t_element::~t_element() {
	/*  Dummy destructor  */
}

void t_element::setValue(const string &src_symbol, const fraction &src_count) {
	symbol = src_symbol;
	count = src_count;
}

bool t_element::parseString(const string &src) {
	fraction tempCount(0, 1);

	if (parseFractionSuffix(src, this->symbol, this->count) == false) {
		return(false);
	}

	if (count.isZero() == true) {
		if (this->symbol.length() != src.length()) {
			return(false);
		} else {
			count.setValue(1, 1);
		}
	}

	return(true);
}

} /*  namespace parser  */
} /*  namespace balancer  */
