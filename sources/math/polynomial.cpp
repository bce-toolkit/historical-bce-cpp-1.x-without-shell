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
#include <vector>
#include <iterator>
#include <algorithm>
#include <libbceplus/standard.hpp>
#include <libbceplus/math/integer.hpp>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/math/polynomial_unknown.hpp>
#include <libbceplus/math/polynomial.hpp>

using namespace std;

namespace balancer {
namespace math {

polynomial::polynomial() {
	this->unknowns.clear();
	this->constant.setValue(0, 1);
}

polynomial::polynomial(const polynomial &src) {
	this->unknowns = src.unknowns;
	this->constant = src.constant;
}

polynomial::polynomial(const fraction &constant) {
	this->unknowns.clear();
	this->constant = constant;
}

polynomial::~polynomial() {
	/*  Dummy destructor  */
}

bool polynomial::isNoUnknown() {
	return(this->unknowns.size() == 0);
}

bool polynomial::isNoUnknown() const {
	return(const_cast<polynomial*>(this)->isNoUnknown());
}

bool polynomial::isAllNegative() {
	vector<polynomialUnknown>::iterator iterUnknown;

	for (iterUnknown = this->unknowns.begin(); iterUnknown != this->unknowns.end(); iterUnknown++) {
		if (iterUnknown->prefix.isNegative() == false) {
			return(false);
		}
	}

	if (this->constant.isNegative() == false && this->constant.isZero() == false) {
		return(false);
	} else {
		return(true);
	}
}

bool polynomial::isAllNegative() const {
	return(const_cast<polynomial*>(this)->isAllNegative());
}

void polynomial::clear() {
	this->unknowns.clear();
}

void polynomial::addUnknown(const size_t &srcID, const fraction &srcPrefix) {
	polynomialUnknown itemNew;
	vector<polynomialUnknown>::iterator iterFind;

	/*  If the prefix equals to zero, just exit this function.  */
	if (srcPrefix.isZero() == true) {
		return;
	}

	/*  Make temporary node  */
	itemNew.setValue(srcID, srcPrefix);

	/*  Find the insert position  */
	iterFind = lower_bound(this->unknowns.begin(), this->unknowns.end(), itemNew);

	if (iterFind != this->unknowns.end() && *iterFind == itemNew) {
		/*  Add the prefix to the existed node  */
		iterFind->prefix += itemNew.prefix;

		/*  If the prefix equals to zero, remove it  */
		if (iterFind->prefix.isZero() == true) {
			this->unknowns.erase(iterFind);
		}

		return;
	}

	/*  Insert the node to the container  */
	this->unknowns.insert(iterFind, itemNew);
}

const fraction& polynomial::getConstant() {
	return(this->constant);
}

const fraction& polynomial::getConstant() const {
	return(const_cast<polynomial*>(this)->getConstant());
}

void polynomial::setConstant(const fraction &src) {
	this->constant = src;
}

void polynomial::multiply(const fraction &src) {
	vector<polynomialUnknown>::iterator iterUnknown;

	this->constant *= src;

	for (iterUnknown = this->unknowns.begin(); iterUnknown != this->unknowns.end(); iterUnknown++) {
		iterUnknown->prefix *= src;
		if (iterUnknown->prefix.isZero() == true) {
			iterUnknown = this->unknowns.erase(iterUnknown) - 1;
		}
	}
}

void polynomial::multiply(const int &src) {
	multiply(fraction(src, 1));
}

void polynomial::merge(const polynomial &src) {
	vector<polynomialUnknown>::const_iterator iterUnknown;

	this->constant += src.constant;

	for (iterUnknown = src.unknowns.cbegin(); iterUnknown != src.unknowns.cend(); iterUnknown++) {
		this->addUnknown(iterUnknown->id, iterUnknown->prefix);
	}
}

void polynomial::mergeCost(const polynomial &src) {
	vector<polynomialUnknown>::const_iterator iterUnknown;

	this->constant -= src.constant;

	for (iterUnknown = src.unknowns.cbegin(); iterUnknown != src.unknowns.cend(); iterUnknown++) {
		this->addUnknown(iterUnknown->id, fraction(0, 1) - iterUnknown->prefix);
	}
}

const string polynomial::toString() {
	string build = "", \
	       buildTemp = "", \
	       buildConstant = "";
	vector<polynomialUnknown>::iterator iterUnknown;

	/*  Print all unknowns to the result string  */
	for (iterUnknown = this->unknowns.begin(); iterUnknown != this->unknowns.end(); iterUnknown++) {
		buildTemp = iterUnknown->toString();

		if (build.length() != 0 && \
		    build[build.length() - 1] == STANDARD_PLUS_CHAR && \
		    buildTemp[0] == STANDARD_MINUS_CHAR) {
			build.erase(build.length() - 1, 1);
		}

		if (buildTemp.length() != 0) {
			if (iterUnknown + 1 == this->unknowns.end()) {
				build += buildTemp;
			} else {
				build += buildTemp + STANDARD_PLUS_STRING;
			}
		}
	}

	/*  Get the string of constant  */
	buildConstant = this->constant.toString();

	/*  Print the constant to the result string  */
	if (this->constant.isZero() == false) {
		if (buildConstant[0] == STANDARD_MINUS_CHAR) {
			build += buildConstant;
		} else {
			if (build.length() == 0) {
				build += buildConstant;
			} else {
				build += STANDARD_PLUS_STRING + buildConstant;
			}
		}
	}

	if (build.length() == 0) {
		build = STANDARD_ZERO_STRING;
	}

	return(build);
}

const string polynomial::toString() const {
	return(const_cast<polynomial*>(this)->toString());
}

void polynomialVectorAllToInteger(vector<polynomial> &dest) {
	integer multiplyLCM = 1;
	fraction multiplyFC(0, 1);
	vector<polynomial>::iterator iterDest;
	vector<polynomialUnknown>::iterator iterUnknown;

	/*  Get the least common multiplination  */
	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		for (iterUnknown = iterDest->unknowns.begin(); iterUnknown != iterDest->unknowns.end(); iterUnknown++) {
			multiplyLCM = lcm(multiplyLCM, iterUnknown->prefix.getDenominator());
		}
		multiplyLCM = lcm(multiplyLCM, iterDest->getConstant().getDenominator());
	}

	multiplyFC.setValue(multiplyLCM, 1);

	/*  Multiply each element with the l.c.m.  */
	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		for (iterUnknown = iterDest->unknowns.begin(); iterUnknown != iterDest->unknowns.end(); iterUnknown++) {
			iterUnknown->prefix *= multiplyFC;
		}
		iterDest->setConstant(iterDest->getConstant() * multiplyFC);
	}
}

void polynomialVectorRemoveOnlyUnknown(vector<polynomial> &dest) {
	vector<polynomial>::iterator iterDest;
	vector<polynomialUnknown>::iterator iterUnknown;

	/*  Scan all unknowns  */
	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		if (iterDest->unknowns.size() != 1 || iterDest->unknowns[0].id != 0) {
			return;
		}
	}

	/*  Work for each polynomial  */
	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		iterDest->setConstant(iterDest->getConstant() + iterDest->unknowns[0].prefix);
		iterDest->unknowns.clear();
	}
}

void polynomialVectorReindex(vector<polynomial> &dest) {
	size_t idx = 0, \
	       idAllocated = 0, \
	       idMin = 0, \
	       *next = NULL;
	bool idLoaded = false;

	/*  Allocate spaces to contain next-pointers  */
	do {
		next = new size_t[dest.size()]();
	} while(next == NULL);

	while(true) {
		idLoaded = false;

		/*  Get the minimum ID of the front of each next-queue  */
		for (idx = 0; idx < dest.size(); idx++) {
			if (next[idx] != dest[idx].unknowns.size()) {
				if (idLoaded == false) {
					idMin = dest[idx].unknowns[next[idx]].id;
				} else {
					if (dest[idx].unknowns[next[idx]].id < idMin) {
						idMin = dest[idx].unknowns[next[idx]].id;
					}
				}

				idLoaded = true;
			}
		}

		/*  If no ID was loaded, break the function.  */
		if (idLoaded == false) {
			break;
		}

		/*  Reset the ID  */
		for (idx = 0; idx < dest.size(); idx++) {
			if (next[idx] != dest[idx].unknowns.size()) {
				if (dest[idx].unknowns[next[idx]].id == idMin) {
					dest[idx].unknowns[next[idx]].id = idAllocated;
					next[idx]++;
				}
			}
		}

		/*  Allocate a new ID  */
		idAllocated++;
	}

	delete []next;
}

} /*  namespace math  */
} /*  namespace balancer  */
