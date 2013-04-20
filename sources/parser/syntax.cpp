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
#include <libbceplus/math/fraction.hpp>
#include <libbceplus/math/matrix.hpp>
#include <libbceplus/math/polynomial.hpp>
#include <libbceplus/math/equation.hpp>
#include <libbceplus/parser/common.hpp>
#include <libbceplus/parser/element.hpp>
#include <libbceplus/parser/molecule.hpp>
#include <libbceplus/parser/syntax.hpp>

using namespace std;
using namespace balancer::math;

namespace balancer {
namespace parser {

vector<cdec> syntaxPreparser(const string &formula, \
                             const bceSetup &settings) {
	size_t idxPosition = 0, \
	       idxLast     = 0, \
	       sizeLB      = 0, \
	       sizeRB      = 0;
	bool   readEqual    = false, \
	       readNegative = false;
	string molecularFM = "", \
	       symbolWrite = "";
	vector<cdec> result;
	cdec buildCDec;
	fraction dummyFraction;

	/*  Clear  */
	result.clear();

	for (idxPosition = 0; idxPosition <= formula.length(); ++idxPosition) {
		if (formula[idxPosition] == STANDARD_BRACKET_START_A || \
			formula[idxPosition] == STANDARD_BRACKET_START_B || \
			formula[idxPosition] == STANDARD_BRACKET_START_C) {
			sizeLB++;
			continue;
		}

		if (formula[idxPosition] == STANDARD_BRACKET_END_A || \
			formula[idxPosition] == STANDARD_BRACKET_END_B || \
			formula[idxPosition] == STANDARD_BRACKET_END_C) {
			sizeRB++;
			continue;
		}

		if (sizeLB != sizeRB) {
			continue;
		}

		/*  Analyze the chemical formula of the chemical equation  */
		if (idxPosition == formula.length() || \
			formula[idxPosition] == STANDARD_PLUS_CHAR || \
			formula[idxPosition] == STANDARD_MINUS_CHAR || \
			formula[idxPosition] == STANDARD_SYNTAX_SEPARATOR_CHAR || \
			formula[idxPosition] == STANDARD_SYNTAX_EQUAL_CHAR) {
			/*  Get the chemical formula  */
			molecularFM = formula.substr(idxLast, idxPosition - idxLast);

			if (molecularFM.length() == 0) {
				if (formula[idxPosition] == STANDARD_SYNTAX_EQUAL_CHAR) {
					readEqual = true;
				}

				idxLast = idxPosition + 1;

				continue;
			}

			/*  Decide whether the connector before a chemical is STANDARD_MINUS_CHAR  */
			readNegative = ((idxLast == 0) ? \
						   false : \
						   (formula[idxPosition - 1] == STANDARD_MINUS_CHAR));

			/*  Process for hydrate dot.  */
			if (settings.parser.useMolecularPrefix == false && \
				findHydrateDot(molecularFM, NULL) == false) {
				if (parseFractionPrefix(molecularFM, symbolWrite, dummyFraction) == false) {
					/*  We can't process failure here, so just remove results and it will cause an error when
						build the balance matrix.  */
					result.clear();
					return(result);
				}
			} else {
				symbolWrite = molecularFM;
			}

			/*  Insert the parsed one to result vector  */
			buildCDec.symbol = symbolWrite;
			buildCDec.negative = readNegative;
			buildCDec.right = readEqual;
			result.insert(result.end(), buildCDec);

			if (formula[idxPosition] == STANDARD_SYNTAX_EQUAL_CHAR) {
				readEqual = true;
			}

			idxLast = idxPosition + 1;
		}
	}

	if (sizeLB != sizeRB) {
		result.clear();
	}

	return(result);
}

bool syntaxDecodedToMatrix(vector<cdec> &cdecTable, \
                           matrix &target, \
                           size_t &mx, \
                           size_t &my, \
                           const bceSetup &settings) {
	size_t px = 0, \
	       py = 0;
	vector<cdec>::iterator iterTable;
	vector<element> sampleElements, \
	                parsedElements;
	vector<element>::iterator iterElement1, \
	                          iterElement2, \
	                          findEM;
	element build;

	/*  Clear  */
	sampleElements.clear();

	/*  Pre-parse existed elements  */
	for (iterTable = cdecTable.begin(); iterTable != cdecTable.end(); ++iterTable) {
		parsedElements.clear();

		if (parseMolecule(iterTable->symbol, NULL, fraction(1, 1), parsedElements, settings) == false) {
			return(false);
		}

		if (parsedElements.size() == 0) {
			iterTable = cdecTable.erase(iterTable) - 1;
		} else {
			for (iterElement1 = parsedElements.begin(); iterElement1 != parsedElements.end(); ++iterElement1) {
				build.setValue(iterElement1->symbol, fraction(0, 1));
				findEM = lower_bound(sampleElements.begin(), sampleElements.end(), build);
				if (findEM == sampleElements.end() || *findEM != build) {
					sampleElements.insert(findEM, build);
				}
			}
		}
	}

	/*  Decide and reset the matrix size  */
	mx = cdecTable.size() + 1;
	my = sampleElements.size();

	if (mx <= 1 || my == 0) {
		return(false);
	}

	target.resizeMatrix(mx, my);
	target.clear();

	/*  Re-parse and write data to the matrix  */
	for (iterTable = cdecTable.begin(), px = 0; iterTable != cdecTable.end(); ++iterTable, ++px) {
		parsedElements.clear();

		if (parseMolecule(iterTable->symbol, NULL, fraction((iterTable->negative == iterTable->right ? 1 : -1), 1), parsedElements, settings) == false) {
			return(false);
		}

		for (iterElement1 = parsedElements.begin(); iterElement1 != parsedElements.end(); ++iterElement1) {
			for (iterElement2 = sampleElements.begin(), py = 0; iterElement2 != sampleElements.end(); ++iterElement2, ++py) {
				if (*iterElement1 == *iterElement2) {
					target.write(px, py, iterElement1->count);
					break;
				}
			}
		}
	}

	return(true);
}

} /*  namespace parser  */
} /*  namespace balancer  */
