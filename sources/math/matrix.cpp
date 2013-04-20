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

#include <iostream>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/math/matrix.hpp>

using namespace std;

namespace balancer {
namespace math {

matrix::matrix() {
	mtx = NULL;
	this->resizeMatrix(1, 1);
}

matrix::matrix(const matrix &src) {
	size_t px = 0, \
	       py = 0;

	mtx = NULL;
	this->resizeMatrix(src.max_x, src.max_y);

	for (py = 0; py < max_y; ++py) {
		for (px = 0; px < max_x; ++px) {
			mtx[py][px] = src.mtx[py][px];
		}
	}
}

matrix::matrix(const size_t mx, const size_t my) {
	mtx = NULL;
	this->resizeMatrix(mx, my);
}

matrix::~matrix() {
	this->destroyMatrix();
}

void matrix::clear() {
	size_t px = 0, \
	       py = 0;

	for (py = 0; py < max_y; ++py) {
		for (px = 0; px < max_x; ++px) {
			mtx[py][px].setValue(0, 1);
		}
	}
}

void matrix::resizeMatrix(const size_t mx, const size_t my) {
	size_t py = 0;

	if (mtx != NULL) {
		destroyMatrix();
	}

	max_x = mx;
	max_y = my;

	do {
		mtx = new fraction*[my];
	} while(mtx == NULL);

	for (py = 0; py < max_y; ++py) {
		do {
			mtx[py] = new fraction[mx]();
		} while(mtx[py] == NULL);
	}
}

void matrix::destroyMatrix() {
	size_t py = 0;

	if (mtx != NULL) {
		for (py = 0; py < max_y; ++py) {
			delete [](mtx[py]);
		}
		delete []mtx;
	}
}

const fraction& matrix::read(const size_t ofx, const size_t ofy) {
	return(mtx[ofy][ofx]);
}

const fraction& matrix::read(const size_t ofx, const size_t ofy) const {
	return(const_cast<matrix*>(this)->read(ofx, ofy));
}

void matrix::write(const size_t ofx, const size_t ofy, const fraction &data) {
	mtx[ofy][ofx] = data;
}

void matrix::swapRow(const size_t src, const size_t dest) {
	fraction *t = mtx[src];
	mtx[src] = mtx[dest];
	mtx[dest] = t;
}

void matrix::swapColumn(const size_t src, const size_t dest) {
	size_t py = 0;
	fraction t;

	for (py = 0; py < max_y; ++py) {
		t = mtx[py][src];
		mtx[py][src] = mtx[py][dest];
		mtx[py][dest] = t;
	}
}

void matrix::print() {
	size_t px = 0, \
	       py = 0;

	for (py = 0; py < max_y; ++py) {
		for (px = 0; px < max_x; ++px) {
			cout<<mtx[py][px].toString()<<'\t';
		}
		cout<<endl;
	}
}

void matrix::print() const {
	return(const_cast<matrix*>(this)->print());
}

} /*  namespace math  */
} /*  namespace balancer  */
