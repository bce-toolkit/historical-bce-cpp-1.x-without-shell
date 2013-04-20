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

#include <cctype>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <libbceplus/standard.hpp>
#include <libbceplus/setup.hpp>
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/parser/common.hpp>
#include <libbceplus/parser/element.hpp>
#include <libbceplus/parser/rpt.hpp>
#include <libbceplus/parser/molecule.hpp>

using namespace std;
using namespace balancer::math;

namespace balancer {
namespace parser {

bool findHydrateDot(const string &formula, \
                    size_t *position) {
	size_t sizeLB  = 0, \
	       sizeRB = 0;

	string::const_iterator iterPosition;

	for (iterPosition = formula.cbegin(); \
	     iterPosition != formula.cend(); \
	     ++iterPosition) {
		if (*iterPosition == STANDARD_BRACKET_START_A || \
		    *iterPosition == STANDARD_BRACKET_START_B || \
		    *iterPosition == STANDARD_BRACKET_START_C) {
			sizeLB++;

			continue;
		}

		if (*iterPosition == STANDARD_BRACKET_END_A || \
		    *iterPosition == STANDARD_BRACKET_END_B || \
		    *iterPosition == STANDARD_BRACKET_END_C) {
			sizeRB++;

			continue;
		}

		if (sizeLB == sizeRB && \
		    *iterPosition == STANDARD_PARSER_HYDRATE_DOT_CHAR) {
			if (position != NULL) {
				*position = iterPosition - formula.cbegin();
			}

			return(true);
		}
	}

	return(false);
}

bool parseRPT(const string &name, rpt *result, const bceSetup &setup) {
	if (setup.parser.externalRPT != NULL && \
	    setup.parser.externalRPT(name, result) == true) {
		return(true);
	}

	if (setup.parser.useInternalRPT == true && \
	    rptQueryInternal(name, result) == true) {
		return(true);
	}

	return(false);
}

bool parseMolecule(const string &formula, \
                   int *status, \
                   const fraction &suffix, \
                   vector<element> &result, \
                   const bceSetup &setup) {
	size_t ofxPosition   = 0, \
	       ofxHydrateDot = 0, \
	       ofxBBegin     = 0, \
		   ofxBEnd       = 0, \
	       ofxLast       = 0, \
	       sizeLB        = 0, \
	       sizeRB        = 0;
	fraction suffixRead(0, 1), \
	         suffixBracket(0, 1);
	string formulaRead   = "", \
	       formulaTemp   = "", \
	       bracketLeft   = "", \
	       bracketMid    = "", \
	       bracketRight  = "";
	element elementBuild;
	vector<element>::iterator elementData;
	rpt rptData;

	/*  Find the hydrate dot and split this molecular into two parts.  */
	if (findHydrateDot(formula, &ofxHydrateDot) == true) {
		return(parseMolecule(formula.substr(0, ofxHydrateDot), status, suffix, result, setup) == true && \
			   parseMolecule(formula.substr(ofxHydrateDot + 1), status, suffix, result, setup) == true);
	}

	/*  Get the prefix number before the molecular.  */
	if (parseFractionPrefix(formula, formulaRead, suffixRead) == false) {
		return(false);
	}

	if (formulaRead.length() == formula.length()) {
		suffixRead.setValue(1, 1);
	}

	suffixRead *= suffix;
	if (formulaRead.length() == 0) {
		return(true);
	}

	if (parseRPT(formulaRead, &rptData, setup) == true) {
		if (status != NULL && rptData.status != STANDARD_STATUS_NONE) {
			*status = rptData.status;
		}
		return(parseMolecule(rptData.target, status, suffixRead * rptData.suffix, result, setup));
	}

	/*  Parse bracket part  */
	for (ofxBBegin = 0; ofxBBegin < formulaRead.length(); ++ofxBBegin) {
		if (formulaRead[ofxBBegin] == STANDARD_BRACKET_START_A || \
			formulaRead[ofxBBegin] == STANDARD_BRACKET_START_B || \
			formulaRead[ofxBBegin] == STANDARD_BRACKET_START_C) {
			for (ofxPosition = ofxBBegin; ofxPosition < formulaRead.length(); ++ofxPosition) {
				if (formulaRead[ofxPosition] == STANDARD_BRACKET_START_A || \
					formulaRead[ofxPosition] == STANDARD_BRACKET_START_B || \
					formulaRead[ofxPosition] == STANDARD_BRACKET_START_C) {
					sizeLB++;
					continue;
				}

				if (formulaRead[ofxPosition] == STANDARD_BRACKET_END_A || \
					formulaRead[ofxPosition] == STANDARD_BRACKET_END_B || \
					formulaRead[ofxPosition] == STANDARD_BRACKET_END_C) {
					sizeRB++;

					if (sizeLB == sizeRB) {
						ofxBEnd = ofxPosition;
						goto __bracket_analyzed;
					}

					continue;
				}
			}

			return(false);
__bracket_analyzed:
			/*  Get the expressions on the left, middle and right.  */
			bracketLeft  = formulaRead.substr(0, ofxBBegin);
			bracketMid   = formulaRead.substr(ofxBBegin + 1, \
													ofxBEnd - ofxBBegin - 1);
			bracketRight = formulaRead.substr(ofxBEnd + 1);

			/*  Get the suffix of the bracket  */
			if (parseFractionPrefix(bracketRight, formulaTemp, suffixBracket) == false) {
				return(false);
			}
			if (formulaTemp.length() == bracketRight.length()) {
				suffixBracket.setValue(1, 1);
			}
			bracketRight = formulaTemp;

			/*  Parse three parts  */
			return(parseMolecule(bracketLeft, status, suffixRead, result, setup) == true && \
				   parseMolecule(bracketMid, status, suffixRead * suffixBracket, result, setup) == true && \
				   parseMolecule(bracketRight, status, suffixRead, result, setup) == true);
		}
	}

	/*  Analyze elements  */
	ofxLast = 0;
	for (ofxPosition = 1; ofxPosition <= formulaRead.length(); ++ofxPosition) {
		if (ofxPosition == formulaRead.length() || isupper(formulaRead.at(ofxPosition))) {
			if (elementBuild.parseString(formulaRead.substr(ofxLast, ofxPosition - ofxLast)) == false) {
				return(false);
			}

			elementBuild.count *= suffixRead;

			if (elementBuild.count.isZero() == false) {
				/*  Insert the element  */
				elementData = lower_bound(result.begin(), result.end(), elementBuild);
				if (elementData != result.end() && *elementData == elementBuild) {
					elementData->count += elementBuild.count;
					if (elementData->count.isZero() == true) {
						result.erase(elementData);
					}
				} else {
					result.insert(elementData, elementBuild);
				}
			}

			ofxLast = ofxPosition;
		}
	}

	return(true);
}

} /*  namespace parser  */
} /*  namespace balancer  */
