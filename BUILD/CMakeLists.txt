PROJECT (Compiler)
SET(SRC_LIST ../compilercompiler/haneda_cc;
             ../include/haneda_util;
             ../lex/haneda_lex;
             ../syntax_and_semantics/haneda_syntax;
             ../syntax_and_semantics/haneda_env;
             ../syntax_and_semantics/syntax_test.c)
ADD_EXECUTABLE(test_compiler ${SRC_LIST})
