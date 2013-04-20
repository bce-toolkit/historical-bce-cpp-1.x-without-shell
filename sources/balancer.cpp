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

#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <iterator>
#include <libbceplus/setup.hpp>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/math/matrix.hpp>
#include <libbceplus/math/polynomial.hpp>
#include <libbceplus/math/equation.hpp>
#include <libbceplus/parser/common.hpp>
#include <libbceplus/parser/element.hpp>
#include <libbceplus/parser/molecule.hpp>
#include <libbceplus/parser/syntax.hpp>
#include <libbceplus/parser/rpt.hpp>
#include <libbceplus/balancer.hpp>

using namespace std;
using namespace balancer::math;
using namespace balancer::parser;

namespace balancer {

void balancerArrangeSide(vector<cdec> &parsed, \
                         vector<polynomial> &result, \
                         const bceSetup &settings) {
	vector<cdec> procParsed;
	vector<polynomial> procSolved;
	vector<cdec>::iterator iterParsed;
	vector<polynomial>::iterator iterEqResult;
	cdec popupParsed;
	polynomial popupEqResult;
	bool negative = false;

	if (settings.features.fixArrangeSide == false) {
		return;
	}

	/*  Clear  */
	procParsed.clear();
	procSolved.clear();

	/*  Analyze which element should be put to left side (before "=")  */
	for (iterParsed = parsed.begin(), iterEqResult = result.begin(); \
		 iterParsed != parsed.end(); \
		 iterParsed++, iterEqResult++) {
		negative = iterEqResult->isAllNegative();

		if (iterParsed->right == false && negative == false) {
			procParsed.push_back(*iterParsed);
			procSolved.push_back(*iterEqResult);
		}

		if (iterParsed->right == true && negative == true) {
			popupParsed = *iterParsed;
			popupEqResult = *iterEqResult;
			popupParsed.right = false;
			popupEqResult.multiply(fraction(-1, 1));
			procParsed.push_back(popupParsed);
			procSolved.push_back(popupEqResult);
		}
	}

	/*  Analyze which element should be put to right side (after "=")  */
	for (iterParsed = parsed.begin(), iterEqResult = result.begin(); \
		 iterParsed != parsed.end(); \
		 iterParsed++, iterEqResult++) {
		negative = iterEqResult->isAllNegative();

		if (iterParsed->right == true && negative == false) {
			procParsed.push_back(*iterParsed);
			procSolved.push_back(*iterEqResult);
		}

		if (iterParsed->right == false && negative == true) {
			popupParsed = *iterParsed;
			popupEqResult = *iterEqResult;
			popupParsed.right = true;
			popupEqResult.multiply(fraction(-1, 1));
			procParsed.push_back(popupParsed);
			procSolved.push_back(popupEqResult);
		}
	}

	parsed = procParsed;
	result = procSolved;
}

bool balancerProcess(const string &formula, \
                     vector<cdec> &parsed, \
                     vector<polynomial> &result, \
                     const bceSetup &settings) {
	size_t mx = 0, \
	       my = 0;
	matrix eq;
	vector<cdec> preparsed;
	vector<polynomial> eqResult;
	vector<cdec>::iterator iterParsed;
	vector<polynomial>::iterator iterEqResult;

	/*  Pre-parse the chemical equation  */
	preparsed = syntaxPreparser(formula, settings);

	/*  Convert to matrix  */
	if (syntaxDecodedToMatrix(preparsed, eq, mx, my, settings) == false) {
		return(false);
	}

	/*  Solve the matrix as linear equations  */
	if (equationSolve(eq, 0, 0, mx, my, eqResult) == false) {
		return(false);
	}

	/*  Re-index unknowns in the result vector  */
	polynomialVectorReindex(eqResult);

	/*  Check the result we had got  */
	if (equationCheckResult(eq, mx, my, eqResult) == false) {
		return(false);
	}

	/*  Remove zero-prefixed elements in the result vector  */
	for (iterParsed = preparsed.begin(), iterEqResult = eqResult.begin(); \
		 iterParsed != preparsed.end(); \
		 iterParsed++, iterEqResult++) {
		if (iterEqResult->isNoUnknown() == true && \
			iterEqResult->getConstant().isZero() == true) {
			if (settings.features.fixRemoveZero == false) {
				return(false);
			}
			iterParsed = preparsed.erase(iterParsed) - 1;
			iterEqResult = eqResult.erase(iterEqResult) - 1;
		}
	}

	if (eqResult.size() < 2) {
		return(false);
	}

	/*  Finalize  */
	polynomialVectorAllToInteger(eqResult);
	polynomialVectorRemoveOnlyUnknown(eqResult);

	/*  Arrange elements  */
	balancerArrangeSide(preparsed, eqResult, settings);

	parsed = preparsed;
	result = eqResult;

	return(true);
}

bool balancerBuildAnswer(vector<cdec> &parsed, \
                         vector<polynomial> &result, \
                         string &built) {
	bool lastSide = false, \
	     requireEqual = false;
	string connector = "", \
	       tmpBuild = "", \
	       tmpR = "", \
	       tmpSymbol = "";
	vector<cdec>::iterator iterParsed;
	vector<polynomial>::iterator iterEqResult;

	/*  Initialize  */
	tmpBuild.clear();

	for (iterParsed = parsed.begin(), iterEqResult = result.begin(); \
		 iterParsed != parsed.end(); \
		 ++iterParsed, ++iterEqResult) {
		/*
		 *  Convert the result to string
		 *
		 *  P.S: If the prefix-number is 1, we can ignore it, e.g:
		 *       > [1]C+[1]O2=[1]CO2
		 *            C+   O2=   CO2
		 */
		if (iterEqResult->isNoUnknown() == true && \
			iterEqResult->getConstant() == fraction(1, 1)) {
			tmpR.clear();
		} else {
			tmpR = iterEqResult->toString();
		}

		/*  Decide the connector  */
		if (iterParsed == parsed.begin()) {
			if (lastSide != iterParsed->right) {
				requireEqual = true;
			}
			if (iterParsed->negative == true) {
				connector = STANDARD_MINUS_CHAR;
			} else {
				connector.clear();
			}
		} else {
			if (lastSide != iterParsed->right) {
				connector = STANDARD_SYNTAX_EQUAL_CHAR;
				lastSide = iterParsed->right;
			} else {
				connector = (iterParsed->negative == true ? \
						 STANDARD_MINUS_CHAR : \
						 STANDARD_PLUS_CHAR);
			}
		}

		/*  Add bracket for hydrate molecule formula  */
		if (findHydrateDot(iterParsed->symbol, NULL) == true && \
			iterEqResult->getConstant() != fraction(1, 1)) {
			tmpSymbol = STANDARD_BRACKET_BUILD_START + \
						iterParsed->symbol + \
						STANDARD_BRACKET_BUILD_END;
		} else {
			tmpSymbol = iterParsed->symbol;
		}

		/*  Rebuild  */
		if (iterEqResult->isNoUnknown() == true && iterEqResult->isAllNegative() == false) {
			tmpBuild += connector + \
				 tmpR + \
				 tmpSymbol;
		} else {
			tmpBuild += connector + \
						STANDARD_BRACKET_BUILD_START + \
						tmpR + \
						STANDARD_BRACKET_BUILD_END + \
						tmpSymbol;
		}
	}

	/*  If no "=" exists, exit  */
	if (lastSide == false || requireEqual == true) {
		return(false);
	}

	built = tmpBuild;

	return(true);
}

bool balancerAuto(const string &formula, \
                  string &result, \
                  const bceSetup &settings) {
	vector<cdec> procParsed;
	vector<polynomial> procSolved;

	/*  Cleanup  */
	result.clear();

	if (balancerProcess(formula, procParsed, procSolved, settings) == false) {
		return(false);
	}

	if (balancerBuildAnswer(procParsed, procSolved, result) == false) {
		return(false);
	}

	return(true);
}

} /*  namespace balancer  */
