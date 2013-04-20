#  Source codes
SET(LIBBCEPLUS_SRCS
	"balancer.cpp"
	"setup.cpp"
	"math/equation.cpp"
	"math/fraction.cpp"
	"math/integer.cpp"
	"math/matrix.cpp"
	"math/polynomial.cpp"
	"math/polynomial_unknown.cpp"
	"parser/common.cpp"
	"parser/element.cpp"
	"parser/molecule.cpp"
	"parser/rpt.cpp"
	"parser/syntax.cpp"
)

#  Build libbceplus to bceplus.a statically
ADD_LIBRARY(bceplus STATIC ${LIBBCEPLUS_SRCS})
SET_TARGET_PROPERTIES(bceplus PROPERTIES OUTPUT_NAME "bceplus")

#  Add dependencies
TARGET_LINK_LIBRARIES(bceplus gmpxx)
TARGET_LINK_LIBRARIES(bceplus gmp)
