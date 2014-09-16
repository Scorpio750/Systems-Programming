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

TokenizerT *TKCreate(char *separators, char *ts) {
	TokenizerT *Tokimonsta;

	Tokimonsta = (TokenizerT*)malloc(sizeof(TokenizerT));
	Tokimonsta->delimiters = (char*)malloc(sizeof(separators));
	Tokimonsta->tokenstream = (char*)malloc(sizeof(ts));

	// copy strings into TKinstance so they are immutable
	strcpy(Tokimonsta->delimiters, separators);
	strcpy(Tokimonsta->tokenstream, ts);
	Tokimonsta->tkstart = 0;

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
	free(tk->tkstart);
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
	int tkptr = tk->tkstart;

	do {
		for (int i = 0; i < strlen(tk->delimiters); i++) {
			if (tk->delimiters[i] == tk->tokenstream[tkptr]) {
				strncpy(nextToken, tk->tokenstream, tkptr - tk->tkstart);
				found_delim = true;
			}
		}
		tkptr++;
	} while (found_delim == false);

	nextToken = escapeKeys(nextToken);

	tk->tkstart = tkptr;
	return nextToken;
}

char *escapeKeys(char *token) {
	for (int i = 0; i < strlen(token); i++) {
		if (token[i] == "\n"	
	}
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

	TKDestroy(TokiMonsta);
	return 0;
}
