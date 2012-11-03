#include "haneda_lex.h"

static FILE * file ;
static char buffer[1025];
static int count  = 0;

int start(const string file_name) {
	return (int)(file = fopen(file_name, "r"));
}

void printbuffer() {
	printf("%s\n", buffer);
}
/* deep copy struct lexval */
void val_cpy(struct lexval * dest, const struct lexval * src) {
	assert(dest && src);
	dest->d = src->d;
	dest->f = src->f;
	dest->s = strdup(src->s);
	dest->num_t = src->num_t;
}
void lex_init() {
	static bool done = FALSE;
	if(done) return;
	val = checked_malloc(sizeof *val);
	val->d = 0;
	val->f = 0.0;
	val->s = NULL;
	val->num_t = NM_F;
	done = TRUE;
}
int lex() {
	char c;
	count = 0;
	begin:
	while ( (c = getc(file)) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n') {
			continue;
		}
		else if (c == '#') {
			while( TRUE ) {
				c = getc(file);
				if( c == '\n' )
					goto begin;
			}
		}
		else if (isalpha(c))
		{ /* NAME begins */
		    while( TRUE ) {
		        if( isalpha(c) || isdigit(c) || c == '_') {
		        	assert( count < 1024 );
		        	buffer[ count++ ] = c;
		        }
				else
				{
					ungetc(c,file);
					buffer[ count ] = '\0';
					if( strcmp("int", buffer) == 0) {
						return L_INT;
					}
					if( strcmp("float", buffer) == 0) {
						return L_FLOAT;
					}
					if( strcmp("if", buffer) == 0) {
						return L_IF;
					}
					if( strcmp("for", buffer) == 0) {
						return L_FOR;
					}
					if( strcmp("else", buffer) == 0) {
						return L_ELSE;
					}
					if( strcmp("return", buffer) == 0) {
						return L_RETURN;
					}

					string result = malloc(count+1);
					strcpy(result, buffer);
					val->s = result;
					return L_IDENTIFIER;
				}
		        c = getc(file);
		    }
		}/* NAME ends */
		else if (isdigit(c))
		{ /* NUM begins */
			bool isfloat = FALSE;
			int state = 0;

			do {
				if(c == EOF) {
					state = 3;
				}
				switch(state) {
				case 0:
					buffer[count++] = c;
					if (isdigit(c))
						state = 0;
					else if (c=='.') {
						isfloat = TRUE;
						state = 1;
					}
					else {
						ungetc(c,file);
						count--;
						state=3;
					}
					break;
				case 1:
					assert(isdigit(c));
					buffer[count++] = c;
					state = 2;
					break;
				case 2:
					if(isdigit(c)) {
						buffer[count++] = c;
						state = 2;
					}
					else {
						count--;
						ungetc(c,file);
						state = 3;
					}
					break;
				case 3:
					ungetc(c,file);
					buffer[count] = '\0';
					if(!isfloat) {
						val->num_t = NM_I;
						sscanf(buffer, "%d", &val->d);
						val->s = checked_malloc( strlen(buffer)+1);
						strcpy(val->s, buffer);
						return L_CONSTANT;
					}
					else {
						val->num_t = NM_F;
						sscanf(buffer, "%lf", &val->f);
						val->s = checked_malloc( strlen(buffer)+1);
						strcpy(val->s, buffer);
						return L_CONSTANT;
					}
					break;
				}
				c = getc(file);
			} while(TRUE);

		} /* NUM ends */
		else if (c == '\"')
		{
			while(TRUE) {
				c = getc(file);
				if ( c != '"' ) {
					assert(count < 1024);
					buffer[count++] = c;
				}
				else {
					string result = malloc(count+1);
					buffer[count] = '\0';
					strcpy(result, buffer);
					val->s = result;
					return L_STRING_LITERAL;
				}
			 }
		}
		else if (issingle(c)) {
			buffer[0] = c;
			buffer[1] = '\0';
			return c;
		}
		else if (c == '+' ) {
			c = getc(file);
			if ( c == '+' ) {
				buffer[0] = '+';
				buffer[1] = '+';
				buffer[2] = '\0';
				return L_INCREMENT;
			}
			else {
				ungetc(c,file);
				return '+';
			}
		}
		else if (c == '-') {
			c = getc(file);
			if ( c=='-' ) {
				return L_DECREMENT;
			}
			else {
				ungetc(c,file);
				return '-';
			}
		}
		else if (c == '<') {
			c = getc(file);
			if ( c=='=') {
				return L_LS_EQ;
			}
			else {
				ungetc(c,file);
				return '<';
			}
		}
		else if (c == '>') {
			c = getc(file);
			if ( c=='=' ) {
				return L_BG_EQ;
			}
			else {
				ungetc(c,file);
				return '>';
			}
		}
		else if ( c == EOF ) {
			return '$';
		}
		else {
			return 0;
		}
	}

	return '$';
}
