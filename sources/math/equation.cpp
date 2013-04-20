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

#include <vector>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/math/matrix.hpp>
#include <libbceplus/math/polynomial.hpp>
#include <libbceplus/math/equation.hpp>

using namespace std;

namespace balancer {
namespace math {

bool equationSolve(matrix &mtx, size_t ofx, size_t ofy, size_t mx, size_t my, vector<polynomial> &result) {
	size_t px = 0, \
	       py = 0;
	polynomial buildResult, \
	           buildTemp;
	vector<polynomial> rsvResult;

	/*  Check matrix size  */
	if (mx < 2 || my == 0) {
		return(false);
	}

	/*
	 *  Reroute to make the top-left element of the matrix a non-zero value, eg:
	 *  > 0 1 3   =>  > 1 2 2
	 *  > 1 2 2        > 0 1 3
	 *    1 0 -4           1 0 -4
	 */
	for (py = 0; py < my; ++py) {
		if (mtx.read(ofx, ofy + py).isZero() == false) {
			mtx.swapRow(ofy, ofy + py);
			break;
		}
	}

	if (py == my) {
		/*  If elements in the first column are all zero, remove the first column and continue solving recursively.  */
		rsvResult.clear();

		/*  Continue solving the matrix without the first column  */
		if (mx > 2 && equationSolve(mtx, ofx + 1, ofy, mx - 1, my, rsvResult) == false) {
			return(false);
		}

		/*  Add the unknown corresponding to the first column of the matrix to the unknown table of the result  */
		buildResult.clear();
		buildResult.setConstant(fraction(0, 1));
		buildResult.addUnknown(ofx, fraction(1, 1));
		rsvResult.insert(rsvResult.begin(), buildResult);
		result = rsvResult;

		return(true);
	}

	if (mx == 2 || my == 1) {
		/*  AC  */
		rsvResult.clear();
		buildResult.clear();
		buildResult.setConstant(mtx.read(ofx + mx - 1, ofy));

		/*  Back substitute  */
		for (px = 1; px <= mx - 2; ++px) {
			buildTemp.clear();
			buildTemp.setConstant(fraction(0, 1));
			buildTemp.addUnknown(ofx + px, fraction(1, 1));
			rsvResult.insert(rsvResult.end(), buildTemp);
			buildResult.addUnknown(ofx + px, fraction(0, 1) - mtx.read(ofx + px, ofy));
		}
		buildResult.multiply(fraction(1, 1) / mtx.read(ofx, ofy));

		/*  Push the back-substitution value to the result  */
		rsvResult.insert(rsvResult.begin(), buildResult);
		result = rsvResult;
	} else {
		/*  Eliminate  */
		for (py = 0; py < my; ++py) {
			if (mtx.read(ofx, ofy + py).isZero() == false) {
				for (px = 0; px < mx; ++px) {
					if (py == 0) {
						mtx.write(ofx + mx - px - 1, \
						          ofy + py, \
						          mtx.read(ofx + mx - px - 1, ofy + py) / mtx.read(ofx, ofy + py));
					} else {
						mtx.write(ofx + mx - px - 1, \
						          ofy + py, \
						          mtx.read(ofx + mx - px - 1, ofy) - mtx.read(ofx + mx - px - 1, ofy + py) / mtx.read(ofx, ofy + py));
					}
				}
			}
		}

		/*  Calculate the value of eliminated equations  */
		rsvResult.clear();
		if (equationSolve(mtx, ofx + 1, ofy + 1, mx - 1, my - 1, rsvResult) == false) {
			return(false);
		}

		/*  AC & Back substitution  */
		buildResult.clear();
		buildResult.setConstant(mtx.read(ofx + mx - 1, ofy));

		for (px = 1; px < mx - 1; ++px) {
			buildTemp = rsvResult[px - 1];
			buildTemp.multiply(mtx.read(ofx + px, ofy));
			buildResult.mergeCost(buildTemp);
		}

		/*  Push the back-substitution value to the result  */
		rsvResult.insert(rsvResult.begin(), buildResult);
		result = rsvResult;
	}

	return(true);
}

bool equationCheckResult(const matrix &mtx, size_t mx, size_t my, vector<polynomial> &result) {
	size_t px = 0, \
	       py = 0;
	polynomial checker, \
	           buildTemp;

	for (py = 0; py < my; ++py) {
		/*  AC  */
		checker.clear();
		checker.setConstant(fraction(0, 1));

		/*  Calculate the constant item  */
		for (px = 0; px < mx - 1; ++px) {
			buildTemp = result[px];
			buildTemp.multiply(mtx.read(px, py));
			checker.merge(buildTemp);
		}

		/*  Check the result  */
		if (checker.isNoUnknown() == false || checker.getConstant() != mtx.read(mx - 1, py)) {
			return(false);
		}
	}

	return(true);
}

} /*  namespace math  */
} /*  namespace balancer  */
