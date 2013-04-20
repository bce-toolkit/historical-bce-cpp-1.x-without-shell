#  Source codes
SET(BCEPLUS_TESTS_SRCS
	"tests_runner.cpp"
	"sources/balancer_tests.cpp"
	"sources/math/fraction_tests.cpp"
	"sources/math/equation_tests.cpp"
	"sources/parser/molecule_tests.cpp"
	"sources/parser/preparser_tests.cpp"
)

ADD_EXECUTABLE(bceplus_tests ${BCEPLUS_TESTS_SRCS})
SET_TARGET_PROPERTIES(bceplus_tests PROPERTIES OUTPUT_NAME "bceplus_tests")
TARGET_LINK_LIBRARIES(bceplus_tests bceplus)
