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

#include <libbceplus/standard.hpp>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/math/polynomial_unknown.hpp>

using namespace std;

namespace balancer {
namespace math {

adtPolynomialUnknown::adtPolynomialUnknown() {
	this->id = 0;
	this->prefix.setValue(0, 1);
}

adtPolynomialUnknown::adtPolynomialUnknown(const struct adtPolynomialUnknown &src) {
	this->setValue(src.id, src.prefix);
}

adtPolynomialUnknown::~adtPolynomialUnknown() {
	/*  Dummy destructor  */
}

void adtPolynomialUnknown::setValue(const size_t &srcID, const fraction &srcPrefix) {
	this->id = srcID;
	this->prefix = srcPrefix;
}

const string adtPolynomialUnknown::toString() {
	string build = this->prefix.toString();

	if (this->prefix.isZero() == true) {
		return("");
	}

	if (this->prefix == fraction(1, 1)) {
		build.clear();
	}

	if (this->prefix == fraction(-1, 1)) {
		build = STANDARD_FRACTION_NEGATIVE;
	}

	build += STANDARD_PUK_SYMBOL_PREFIX + \
	         this->getUnknownName(this->id);

	return(build);
}

const string adtPolynomialUnknown::toString() const {
	return(const_cast<adtPolynomialUnknown*>(this)->toString());
}

const string adtPolynomialUnknown::getUnknownName(const size_t &srcID) {
	string build = "";
	size_t idTemp = srcID;

	do {
		build.insert(0, \
		             1, \
		                STANDARD_PUK_SYMBOL_DICTIONARY[idTemp % STANDARD_PUK_SYMBOL_DICTIONARY_LENGTH]);
		idTemp /= STANDARD_PUK_SYMBOL_DICTIONARY_LENGTH;
	} while (idTemp != 0);

	return(build);
}

const string adtPolynomialUnknown::getUnknownName(const size_t &srcID) const {
	return(const_cast<adtPolynomialUnknown*>(this)->getUnknownName(srcID));
}

} /*  namespace math  */
} /*  namespace balancer  */
