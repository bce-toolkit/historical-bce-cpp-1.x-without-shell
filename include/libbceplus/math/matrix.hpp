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

#ifndef __MATRIX_HPP__
	#define __MATRIX_HPP__

	#include "fraction.hpp"

	namespace balancer {
	namespace math {

	class matrix {
	public:
		/*  Public declares  */
		matrix();
		matrix(const matrix&);
		matrix(const size_t, const size_t);
		~matrix();
		void clear();
		void resizeMatrix(const size_t, \
		                  const size_t);
		const fraction& read(const size_t, \
		                     const size_t);
		const fraction& read(const size_t, \
		                     const size_t) const;
		void write(const size_t, \
		           const size_t, \
		           const fraction&);
		void swapRow(const size_t, \
		             const size_t);
		void swapColumn(const size_t, \
		                const size_t);
		void print();
		void print() const;
		matrix& operator =(const matrix &src) {
			size_t px = 0, \
			       py = 0;
			if (this == &src) {
				return(*this);
			}
			this->resizeMatrix(src.max_x, src.max_y);
			for (py = 0; py < max_y; ++py) {
				for (px = 0; px < max_x; ++px) {
					mtx[py][px] = src.mtx[py][px];
				}
			}
			return(*this);
		}
	private:
		size_t max_x, max_y;
		fraction **mtx;

		/*  Private declares  */
		void destroyMatrix();
	};

	} /*  namespace math  */
	} /*  namespace balancer  */
#endif
