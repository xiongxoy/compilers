#include "../compilercompiler/haneda_cc.h"
#include "../lex/haneda_lex.h"
#include "haneda_env.h"

#ifndef HANEDA_ANALYSIS_SYN_H
#define HANEDA_ANALYSIS_SYN_H

//typedef enum {
//	S_ID = 1,
//	S_FUNC,
//	S_STR,
//	S_INT,
//	S_FLOAT,
//} sym_type;


void syntax_init();
void syntax( string filename );

#endif
