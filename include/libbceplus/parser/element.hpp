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

#ifndef __ELEMENT_HPP__
	#define __ELEMENT_HPP__

	#include "../math/fraction.hpp"

	namespace balancer {
	namespace parser {

	/*  Internal ADT  */
	typedef struct t_element {
		std::string symbol;
		balancer::math::fraction count;

		/*  Public declares  */
		t_element();
		t_element(const t_element&);
		t_element(const std::string&, \
		          const balancer::math::fraction&);
		~t_element();
		void setValue(const std::string&, \
		              const balancer::math::fraction&);
		bool parseString(const std::string&);
		bool operator ==(const struct t_element &rhs) {
			return(this->symbol == rhs.symbol);
		}
		bool operator ==(const struct t_element &rhs) const {
			return(const_cast<t_element*>(this)->operator ==(rhs));
		}
		bool operator !=(const struct t_element &rhs) {
			return(this->symbol != rhs.symbol);
		}
		bool operator !=(const struct t_element &rhs) const {
			return(const_cast<t_element*>(this)->operator !=(rhs));
		}
		bool operator <(const struct t_element &rhs) {
			return(this->symbol < rhs.symbol);
		}
		bool operator <(const struct t_element &rhs) const {
			return(const_cast<t_element*>(this)->operator <(rhs));
		}
		bool operator <=(const struct t_element &rhs) {
			return(this->symbol <= rhs.symbol);
		}
		bool operator <=(const struct t_element &rhs) const {
			return(const_cast<t_element*>(this)->operator <=(rhs));
		}
		bool operator >(const struct t_element &rhs) {
			return(this->symbol > rhs.symbol);
		}
		bool operator >(const struct t_element &rhs) const {
			return(const_cast<t_element*>(this)->operator >(rhs));
		}
		bool operator >=(const struct t_element &rhs) {
			return(this->symbol >= rhs.symbol);
		}
		bool operator >=(const struct t_element &rhs) const {
			return(const_cast<t_element*>(this)->operator >=(rhs));
		}
	} element;

	} /*  namespace parser  */
	} /*  namespace balancer  */
#endif
