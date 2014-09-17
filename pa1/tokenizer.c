/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef unsigned char bool;

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char *delimiters;
	char *tokenstream;
	int tkstart;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

char* escapeKeys(char *token);

TokenizerT *TKCreate(char *separators, char *ts) {
	TokenizerT *Tokimonsta;

	Tokimonsta = (TokenizerT*)malloc(sizeof(TokenizerT));
	Tokimonsta->delimiters = (char*)malloc(sizeof(separators));
	Tokimonsta->tokenstream = (char*)malloc(sizeof(ts));

	/* copy strings into TKinstance so they are immutable */
	strcpy(Tokimonsta->delimiters, separators);
	strcpy(Tokimonsta->tokenstream, ts);
	Tokimonsta->tkstart = 0;
	Tokimonsta->tokenstream = escapeKeys(Tokimonsta->tokenstream);

	if (Tokimonsta != NULL)
		return Tokimonsta;
	else
		return NULL;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
	free(tk->delimiters);
	free(tk->tokenstream);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
char *TKGetNextToken(TokenizerT *tk) {
	char *nextToken;
	bool found_delim;
	int tkptr = tk->tkstart, i;

	do {
		for (i = 0; i < strlen(tk->delimiters); i++) {
			/* finds delimiter match */
			if (tk->delimiters[i] == tk->tokenstream[tkptr]) {
				nextToken = malloc(sizeof(char) * 100);
				strncpy(nextToken, tk->tokenstream, tkptr - tk->tkstart);
				found_delim = true;
			}
		}
		tkptr++;
	} while (found_delim == false);

	tk->tkstart = tkptr;
	if (nextToken != NULL)	{
		return nextToken;
	}
	else {
		return 0;
	}
}

char *escapeKeys(char *token) {
	char* esc_token = (char*)malloc(sizeof(token));
	char temp;
	int i = 0, j = 0;

	for (i = 0; i < strlen(token); i++) {
		if (token[i] == '\\') {
			if (token[i+1] == '\\') {
				temp = 0x5c;
			}
			else if (token[i+1] == 'n') {
				temp = 0x0a;
				i++;
			}
			else if (token[i+1] == 't') {
				temp = 0x09;
				i++;
			}
			else if (token[i+1] == 'v') {
				temp = 0x0b;
				i++;
			}
			else if (token[i+1] == 'b') {
				temp = 0x08;
				i++;
			}
			else if (token[i+1] == 'r') {
				temp = 0x0d;
				i++;
			}
			else if (token[i+1] == 'f') {
				temp = 0x0c;
				i++;
			}
			else if (token[i+1] == 'a') {
				temp = 0x07;
				i++;
			}
			else if (token[i+1] == '\"') {
				temp = 0x22;
				i++;
			}
		}
		else {
			temp = token[i];
		}
		
		esc_token[j] = temp;
		j++;
	}
	return esc_token;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	TokenizerT *TokiMonsta = TKCreate(argv[1], argv[2]);
	while (TokiMonsta->tkstart < strlen(TokiMonsta->tokenstream)) {
		char * delim_token = (char*)TKGetNextToken(TokiMonsta);
		if (!strcmp(delim_token,"0"))	{
			printf("%s\n", delim_token);
		}
	}
	TKDestroy(TokiMonsta);
	return 0;
}
