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
#include <libbceplus/standard.hpp>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/parser/rpt.hpp>

using namespace std;
using namespace balancer::math;

namespace balancer {
namespace parser {

bool rptQueryInternal(const string &name, rpt *result) {
	rpt *p;

	rpt __replace_table[] = {
		{
			.name = "l",
			.target = "",
			.suffix = fraction(1, 1),
			.status = STANDARD_STATUS_LIQUID
		},
		{
			.name = "g",
			.target = "",
			.suffix = fraction(1, 1),
			.status = STANDARD_STATUS_GAS
		},
		{
			.name = "s",
			.target = "",
			.suffix = fraction(1, 1),
			.status = STANDARD_STATUS_SOLID
		},
		{
			.name = "aq",
			.target = "",
			.suffix = fraction(1, 1),
			.status = STANDARD_STATUS_AQUEOUS
		},
		{
			.name = "e+",
			.target = "e",
			.suffix = fraction(1, 1),
			.status = STANDARD_STATUS_NONE
		},
		{
			.name = "e-",
			.target = "e",
			.suffix = fraction(-1, 1),
			.status = STANDARD_STATUS_NONE
		},
		{
			.name = "",
			.target = "",
			.suffix = fraction(0, 1),
			.status = STANDARD_STATUS_NONE
		}
	};

	for (p = __replace_table; p->name.length() != 0; ++p) {
		if (p->name == name) {
			*result = *p;
			return(true);
		}
	}

	return(false);
}

} /*  namespace parser  */
} /*  namespace balancer  */
