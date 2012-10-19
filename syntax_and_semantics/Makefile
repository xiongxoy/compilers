all: test_env
	echo "done"

test_env: haneda_env.c haneda_env.h test_haneda_env.c ../include/*
			make -Wall -w -C ../include/
			cc -Wall -g -c haneda_env.c haneda_env.h 
			cc -Wall -g -o test_env ../include/haneda_util.o haneda_env.o test_haneda_env.c