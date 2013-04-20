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
#include <sstream>
#include <libbceplus/standard.hpp>
#include <libbceplus/math/integer.hpp>
#include <libbceplus/math/fraction.hpp>

using namespace std;

namespace balancer {
namespace math {

fraction::fraction() {
	this->numerator = 0;
	this->denominator = 1;
}

fraction::fraction(const fraction &src) {
	this->numerator = src.numerator;
	this->denominator = src.denominator;
}

fraction::fraction(const integer &numer, const integer &denomin) {
	this->setValue(numer, denomin);
}

fraction::fraction(const int &numer, const int &denomin) {
	this->setValue(numer, denomin);
}

fraction::~fraction() {
	/*  Dummy destructor  */
}

const string fraction::toString() {
	stringstream ret;

	ret.str("");

	if (this->numerator % this->denominator == 0) {
		ret << this->numerator / this->denominator;
	} else {
		ret << this->numerator << STANDARD_FRACTION_SEMICOLON << this->denominator;
	}

	return(ret.str());
}

const string fraction::toString() const {
	return(const_cast<fraction*>(this)->toString());
}

void fraction::simplify() {
	integer gnd;

	if (this->numerator == 0) {
		this->denominator = 1;
		return;
	}

	gnd = gcd(numerator, denominator);
	this->numerator /= gnd;
	this->denominator /= gnd;

	if (this->denominator < 0) {
		this->numerator = -(this->numerator);
		this->denominator = -(this->denominator);
	}
}

const integer& fraction::getNumerator() {
	return(this->numerator);
}

const integer& fraction::getNumerator() const {
	return(const_cast<fraction*>(this)->getNumerator());
}

const integer& fraction::getDenominator() {
	return(this->denominator);
}

const integer& fraction::getDenominator() const {
	return(const_cast<fraction*>(this)->getDenominator());
}

void fraction::setValue(const fraction &src) {
	this->numerator = src.numerator;
	this->denominator = src.denominator;
}

void fraction::setValue(const integer &numer, const integer &denomin) {
	this->numerator = numer;
	this->denominator = denomin;
	this->simplify();
}

void fraction::setValue(const int &numer, const int &denomin) {
	this->numerator = numer;
	this->denominator = denomin;
	this->simplify();
}

fraction fraction::plus(const fraction &na, const fraction &nb) {
	return(fraction(na.numerator * nb.denominator + na.denominator * nb.numerator, na.denominator * nb.denominator));
}

const fraction fraction::plus(const fraction &na, const fraction &nb) const {
	return(const_cast<fraction*>(this)->plus(na, nb));
}

fraction fraction::minus(const fraction &na, const fraction &nb) {
	return(fraction(na.numerator * nb.denominator - na.denominator * nb.numerator, na.denominator * nb.denominator));
}

const fraction fraction::minus(const fraction &na, const fraction &nb) const {
	return(const_cast<fraction*>(this)->minus(na, nb));
}

fraction fraction::multiply(const fraction &na, const fraction &nb) {
	return(fraction(na.numerator * nb.numerator, na.denominator * nb.denominator));
}

const fraction fraction::multiply(const fraction &na, const fraction &nb) const {
	return(const_cast<fraction*>(this)->multiply(na, nb));
}

fraction fraction::divide(const fraction &na, const fraction &nb) {
	return(fraction(na.numerator * nb.denominator, na.denominator * nb.numerator));
}

const fraction fraction::divide(const fraction &na, const fraction &nb) const {
	return(const_cast<fraction*>(this)->divide(na, nb));
}

int fraction::compare(const fraction &na, const fraction &nb) {
	integer delta;

	delta = na.denominator * nb.numerator - na.numerator * nb.denominator;

	if (delta == 0) {
		return(STANDARD_FRACTION_COMPARE_EQUAL);
	}

	if (delta < 0) {
		return(STANDARD_FRACTION_COMPARE_B_MIN);
	} else {
		return(STANDARD_FRACTION_COMPARE_A_MIN);
	}
}

int fraction::compare(const fraction &na, const fraction &nb) const {
	return(const_cast<fraction*>(this)->compare(na, nb));
}

bool fraction::isZero() {
	return (this->numerator == 0);
}

bool fraction::isZero() const {
	return(const_cast<fraction*>(this)->isZero());
}

bool fraction::isNegative() {
	return(this->numerator < 0);
}

bool fraction::isNegative() const {
	return(const_cast<fraction*>(this)->isNegative());
}

} /*  namespace math  */
} /*  namespace balancer  */
