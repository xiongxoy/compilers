all: env_test syntax_test
	 echo "done"

lex: ../lex/*
	make -Wall -w -C ../lex/
compilercompiler: ../compilercompiler/*
	make -Wall -w -C ../compilercompiler/
util: ../include/*
	make -Wall -w -C ../include/
syntax_test: haneda_syntax.h haneda_syntax.c syntax_test.c util lex compilercompiler env_test
	cc -Wall -std=c99 -g -c haneda_syntax.h haneda_syntax.c
	cc -Wall -std=c99 -g -o syntax_test syntax_test.c haneda_syntax.o haneda_env.o ../include/haneda_util.o ../lex/haneda_lex.o ../compilercompiler/haneda_cc.o

env_test: haneda_env.c haneda_env.h env_test.c util
			cc -Wall -std=c99 -g -c haneda_env.c haneda_env.h 
			cc -Wall -std=c99 -g -o env_test  ../include/haneda_util.o haneda_env.o env_test.c
