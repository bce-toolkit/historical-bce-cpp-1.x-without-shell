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

#ifndef __POLYNOMIAL_H__
	#define __POLYNOMIAL_H__

	#include <string>
	#include <vector>
	#include "fraction.hpp"
	#include "polynomial_unknown.hpp"

	namespace balancer {
	namespace math {

	class polynomial {
		/*  Friend declares  */
		friend void polynomialVectorAllToInteger(std::vector<polynomial> &dest);
		friend void polynomialVectorRemoveOnlyUnknown(std::vector<polynomial> &dest);
		friend void polynomialVectorReindex(std::vector<polynomial> &dest);
	public:
		/*  Public declares  */
		polynomial();
		polynomial(const polynomial &src);
		polynomial(const fraction &constant);
		~polynomial();
		bool isNoUnknown();
		bool isNoUnknown() const;
		bool isAllNegative();
		bool isAllNegative() const;
		void clear();
		void addUnknown(const size_t&, \
		                const fraction&);
		const fraction& getConstant();
		const fraction& getConstant() const;
		void setConstant(const fraction&);
		void multiply(const fraction&);
		void multiply(const int&);
		void merge(const polynomial&);
		void mergeCost(const polynomial&);
		const std::string toString();
		const std::string toString() const;
		polynomial& operator =(const polynomial &src) {
			if (this == &src) {
				return(*this);
			}
			this->unknowns = src.unknowns;
			this->constant = src.constant;
			return(*this);
		}
	private:
		std::vector<polynomialUnknown> unknowns;
		fraction constant;
	};

	/*  External declares  */
	void polynomialVectorAllToInteger(std::vector<polynomial> &dest);
	void polynomialVectorRemoveOnlyUnknown(std::vector<polynomial> &dest);
	void polynomialVectorReindex(std::vector<polynomial> &dest);

	} /*  namespace math  */
	} /*  namespace balancer  */
#endif
