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
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio>
#include "sources/math/fraction_tests.hpp"
#include "sources/math/equation_tests.hpp"
#include "sources/parser/molecule_tests.hpp"
#include "sources/parser/preparser_tests.hpp"
#include "sources/balancer_tests.hpp"
#include "tests_runner.hpp"

using namespace std;

struct t_unit_test_table tests[] = {
	{
		.name = "sources/math/fraction.cpp",
		.path = "data/math/fraction/fraction",
		.lower = 1,
		.higher = 100,
		.test = fraction_unit_test
	},
	{
		.name = "sources/math/equation.cpp",
		.path = "data/math/equation/equation",
		.lower = 1,
		.higher = 434,
		.test = equation_unit_test
	},
	{
		.name = "sources/parser/molecule.cpp",
		.path = "data/parser/molecule/molecule",
		.lower = 1,
		.higher = 461,
		.test = molecule_unit_test
	},
	{
		.name = "sources/parser/molecule.cpp",
		.path = "data/bugs/parser/molecule/molecule",
		.lower = 1,
		.higher = 1,
		.test = molecule_unit_test
	},
	{
		.name = "sources/parser/syntax.cpp",
		.path = "data/parser/preparser/preparser",
		.lower = 1,
		.higher = 434,
		.test = preparser_unit_test
	},
	{
		.name = "sources/balancer.cpp",
		.path = "data/balancer/balancer",
		.lower = 1,
		.higher = 434,
		.test = balancer_unit_test
	},
	{
		.name = "sources/balancer.cpp",
		.path = "data/bugs/balancer/balancer",
		.lower = 1,
		.higher = 4,
		.test = balancer_unit_test
	},
	{
		.name = "sources/balancer.cpp",
		.path = "data/features/fraction_suffix/fsf",
		.lower = 1,
		.higher = 2,
		.test = balancer_unit_test
	},
	{
		.name = "(None)",
		.path = "",
		.lower = 1,
		.higher = 1,
		.test = NULL
	},
};

bool tester_check_result(const string &path, const string &sample) {
	ifstream fa, fb;
	string ia, ib;

	fa.open(path.c_str(), ifstream::in);
	fb.open(sample.c_str(), ifstream::in);

	while(!fa.eof() && !fb.eof()) {
		fa >> ia;
		fb >> ib;
		if (ia != ib) {
			fa.close();
			fb.close();
			return(false);
		}
	}

	if (fa.eof() == false || fb.eof() == false) {
		return(false);
	}

	fa.close();
	fb.close();

	return(true);
}

int main(int argc, char **argv) {
	string pathA, pathB;
	stringstream tmpA, tmpB;
	struct t_unit_test_table *ptrTest;
	int idx;
	const string tmpFile = "answer.tmp";

	for (ptrTest = tests; ptrTest->test != NULL; ptrTest++) {
		for (idx = ptrTest->lower; idx <= ptrTest->higher; idx++) {
			tmpA.str("");
			tmpA << ptrTest->path << idx << ".in";
			pathA = tmpA.str();

			tmpB.str("");
			tmpB << ptrTest->path << idx << ".out";
			pathB = tmpB.str();

			cout << "[" << "TEST: " << ptrTest->name << ": " << pathA << "]" << endl;
			ptrTest->test(pathA, tmpFile);
			if (tester_check_result(pathB, tmpFile) == false) {
				cout << "[ERROR] Failed: " << pathA << endl;
				return(1);
			}
		}
	}

	if (remove(tmpFile.c_str()) != 0) {
		cout << "[ERROR] Cannot remove '" << tmpFile << "'." << endl;
	}

	cout << "[SUCCEED] Passed all test cases." << endl;

	return(0);
}
