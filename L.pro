TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/parser/parser.cpp \
    src/common/token.cpp \
    src/parser/lexer.cpp \
    src/common/code.cpp \
    src/common/error_handler.cpp \
    src/common/error_producer.cpp \
    src/parser/lexer_error_producer.cpp \
    src/common/code_point.cpp

HEADERS += \
    src/parser/ast.h \
    src/parser/parser.h \
    src/common/token.h \
    src/common/operators.h \
    src/parser/lexer.h \
    src/common/code_point.h \
    src/common/string_view.h \
    src/common/code.h \
    src/common/error_handler.h \
    src/common/error_producer.h \
    src/common/error_codes.h \
    src/parser/lexer_error_producer.h \
    src/parser/lexer_settings.h

INCLUDEPATH += D:/Programming/Libs/boost_1_59_0

QMAKE_CXXFLAGS += -std=c++14
INCLUDEPATH += "."

