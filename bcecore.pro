#
#	XiaoJSoft BCE++ Core Library for QT
#	(C) 2013 XiaoJSoft Studio. All rights reserved.
#

QT       -= core gui

TARGET = bcecore
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += include
QMAKE_CXXFLAGS += -std=c++0x -std=c++11

SOURCES += \
    sources/balancer.cpp \
    sources/setup.cpp \
    sources/math/polynomial.cpp \
    sources/math/polynomial_unknown.cpp \
    sources/math/matrix.cpp \
    sources/math/integer.cpp \
    sources/math/fraction.cpp \
    sources/math/equation.cpp \
    sources/parser/syntax.cpp \
    sources/parser/rpt.cpp \
    sources/parser/molecule.cpp \
    sources/parser/element.cpp \
    sources/parser/common.cpp

HEADERS += \
    include/libbceplus/math/polynomial.hpp \
    include/libbceplus/math/polynomial_unknown.hpp \
    include/libbceplus/math/matrix.hpp \
    include/libbceplus/math/integer.hpp \
    include/libbceplus/math/fraction.hpp \
    include/libbceplus/math/equation.hpp \
    include/libbceplus/version.hpp \
    include/libbceplus/standard.hpp \
    include/libbceplus/setup.hpp \
    include/libbceplus/balancer.hpp \
    include/libbceplus/parser/syntax.hpp \
    include/libbceplus/parser/rpt.hpp \
    include/libbceplus/parser/molecule.hpp \
    include/libbceplus/parser/element.hpp \
    include/libbceplus/parser/common.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix|win32: LIBS += -lgmpxx
unix|win32: LIBS += -lgmp
