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

#ifndef __FRACTION_HPP__
	#define __FRACTION_HPP__

	#include <string>
	#include "../standard.hpp"
	#include "integer.hpp"

	namespace balancer {
	namespace math {

	class fraction {
	public:
		/*  Public declares  */
		fraction();
		fraction(const fraction&);
		fraction(const integer&, \
		         const integer&);
		fraction(const int&, \
		         const int&);
		~fraction();
		const std::string toString();
		const std::string toString() const;
		const integer& getNumerator();
		const integer& getNumerator() const;
		const integer& getDenominator();
		const integer& getDenominator() const;
		void setValue(const fraction&);
		void setValue(const integer&, \
		              const integer&);
		void setValue(const int&, \
		              const int&);
		bool isZero();
		bool isZero() const;
		bool isNegative();
		bool isNegative() const;
		fraction& operator =(const fraction &rhs) {
			if (this == &rhs) {
				return(*this);
			}
			this->setValue(rhs);
			return(*this);
		}
		bool operator ==(const fraction &rhs) {
			return(this->compare(*this, rhs) == STANDARD_FRACTION_COMPARE_EQUAL);
		}
		bool operator ==(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator ==(rhs));
		}
		bool operator !=(const fraction &rhs) {
			return(this->compare(*this, rhs) != STANDARD_FRACTION_COMPARE_EQUAL);
		}
		bool operator !=(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator !=(rhs));
		}
		bool operator <(const fraction &rhs) {
			return(this->compare(*this, rhs) == STANDARD_FRACTION_COMPARE_A_MIN);
		}
		bool operator <(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator <(rhs));
		}
		bool operator <=(const fraction &rhs) {
			return(this->compare(*this, rhs) != STANDARD_FRACTION_COMPARE_B_MIN);
		}
		bool operator <=(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator <=(rhs));
		}
		bool operator >(const fraction &rhs) {
			return(this->compare(*this, rhs) == STANDARD_FRACTION_COMPARE_B_MIN);
		}
		bool operator >(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator >(rhs));
		}
		bool operator >=(const fraction &rhs) {
			return(this->compare(*this, rhs) != STANDARD_FRACTION_COMPARE_A_MIN);
		}
		bool operator >=(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator >=(rhs));
		}
		fraction operator +(const fraction &rhs) {
			return(this->plus(*this, rhs));
		}
		const fraction operator +(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator +(rhs));
		}
		fraction operator -(const fraction &rhs) {
			return(this->minus(*this, rhs));
		}
		const fraction operator -(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator -(rhs));
		}
		fraction operator *(const fraction &rhs) {
			return(this->multiply(*this, rhs));
		}
		const fraction operator *(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator *(rhs));
		}
		fraction operator /(const fraction &rhs) {
			return(this->divide(*this, rhs));
		}
		const fraction operator /(const fraction &rhs) const {
			return(const_cast<fraction*>(this)->operator /(rhs));
		}
		fraction& operator +=(const fraction &rhs) {
			return(*this = this->plus(*this, rhs));
		}
		fraction& operator -=(const fraction &rhs) {
			return(*this = this->minus(*this, rhs));
		}
		fraction& operator *=(const fraction &rhs) {
			return(*this = this->multiply(*this, rhs));
		}
		fraction& operator /=(const fraction &rhs) {
			return(*this = this->divide(*this, rhs));
		}
	private:
		integer numerator, denominator;

		/*  Private declares  */
		void simplify();
		fraction plus(const fraction&, \
		              const fraction&);
		const fraction plus(const fraction&, \
		                    const fraction&) const;
		fraction minus(const fraction&, \
		               const fraction&);
		const fraction minus(const fraction&, \
		                     const fraction&) const;
		fraction multiply(const fraction&, \
		                  const fraction&);
		const fraction multiply(const fraction&, \
		                        const fraction&) const;
		fraction divide(const fraction&, \
		                const fraction&);
		const fraction divide(const fraction&, \
		                      const fraction&) const;
		int compare(const fraction&, \
		                  const fraction&);
		int compare(const fraction&, \
		                  const fraction&) const;
	};

	} /*  namespace math  */
	} /*  namespace balancer  */
#endif
