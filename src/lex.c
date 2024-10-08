#include "lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static struct Token *next = NULL;
struct Token *searchForNext(void);
struct Token *lexRestNumber(char);
struct Token *lexRestPrefix(char);

int isInfix(enum TokenType type)
{
	return PLUS <= type && type <= POW;
}

struct Token *peek()
{
	if (next == NULL) {
		next = searchForNext();
	}
	return next;
}

void acceptIt()
{
	next = NULL;
}

void accept(enum TokenType type)
{
	peek();
	if (next->type != type) {
		fprintf(stderr, "Expected `%s` but got `%s`\n", TokenStrings[type], next->spelling);
	}
	acceptIt();
}

struct Token *searchForNext()
{
	int nextChar;
	struct Token *ans = malloc(sizeof(struct Token));
	while ((nextChar = getc(stdin)) == ' ' || nextChar == '\t');
	if (nextChar == EOF) {
		ans->type = TOKEN_EOF;
		ans->spelling = "EOF";
		return ans;
	}
	if (nextChar == '\n') {
		ans->type = LINE_END;
		ans->spelling = "\\n";
		return ans;
	}
	switch (nextChar) {
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return lexRestNumber(nextChar);
	case '(':
		ans->type = LPAR;
		ans->spelling = "(";
		return ans;
	case ')':
		ans->type = RPAR;
		ans->spelling = ")";
		return ans;
	case '-':
		ans->type = MINUS;
		ans->spelling = "-";
		return ans;
	case '+':
		ans->type = PLUS;
		ans->spelling = "+";
		return ans;
	case '*':
		ans->type = TIMES;
		ans->spelling = "*";
		return ans;
	case '/':
		ans->type = DIVIDE;
		ans->spelling = "/";
		return ans;
	case '^':
		ans->type = POW;
		ans->spelling = "^";
		return ans;
	default:
		return lexRestPrefix(nextChar);
	}
}

struct Token *lexRestNumber(char first)
{
	struct Token *ans = malloc(sizeof(struct Token));
	int nextChar = '\0';
	ans->type = NUMBER;
	ans->spelling = malloc(sizeof(char)*64);
	ans->spelling[0] = first;
	int i = 1;
	while (i < 64 && (isdigit((nextChar = getc(stdin))))) {
		ans->spelling[i++] = nextChar;
	}
	if (nextChar != '.') {
		ungetc(nextChar, stdin);
		return ans;
	}
	ans->spelling[i++] = nextChar;
	while (i < 64 && (isdigit((nextChar = getc(stdin))))) {
		ans->spelling[i++] = nextChar;
	}
	if (nextChar != '\0') {
		ungetc(nextChar, stdin);
	}
	return ans;
}

struct Token *lexRestPrefix(char first)
{
	struct Token *ans = malloc(sizeof(struct Token));
	int next;
	if (first == 's') {
		if ((next = getchar()) == 'i') {
			if ((next = getchar()) == 'n') {
				ans->type = SIN;
				ans->spelling = "sin";
				return ans;
			}
			fprintf(stderr, "Unrecognized token: si%c\n", next);
			exit(1);
		}
		fprintf(stderr, "Unrecognized token: s%c\n", next);
		exit(1);
	} else if (first == 'c') {
		if ((next = getchar()) == 'o') {
			if ((next = getchar()) == 's') {
				ans->type = COS;
				ans->spelling = "cos";
				return ans;
			}
			fprintf(stderr, "Unrecognized token: co%c\n", next);
			exit(1);
		}
		fprintf(stderr, "Unrecognized token: c%c\n", next);
		exit(1);
	}
	fprintf(stderr, "Unrecognized token: %c\n", first);
	exit(1);
}
