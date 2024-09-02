#include "lex.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_P 3

double fold(double left, enum TokenType type, double right);
double parsePriority(int);
double parsePrimaryExpr();
int isPriority(enum TokenType type, int priority);

double fold(double left, enum TokenType type, double right)
{
	switch (type) {
	case PLUS:
		return left + right;
	case MINUS:
		return left - right;
	case TIMES:
		return left * right;
	case DIVIDE:
		return left / right;
	case POW:
		return pow(left, right);
	default:
		fprintf(stderr, "idk how to fold in %s\n", TokenStrings[type]);
		return left;
	}
}

double parse()
{
	double ans = parsePriority(MAX_P);
	accept(LINE_END);
	return ans;
}

double parsePriority(int priority)
{
	if (priority <= 0) {
		return parsePrimaryExpr();
	}
	double left = parsePriority(priority - 1);
	struct Token *next = peek();

	while (isPriority(next->type, priority)) {
		acceptIt();
		left = fold(left, next->type, parsePriority(priority - 1));
		next = peek();
	}
	return left;
}

double parsePrimaryExpr()
{
	struct Token *next = peek();
	double ans;
	switch (next->type) {
	case NUMBER:
		acceptIt();
		return strtof(next->spelling, NULL);
	case LPAR:
		acceptIt();
		ans = parsePriority(MAX_P);
		accept(RPAR);
		return ans;
	case MINUS:
		acceptIt();
		return -parsePrimaryExpr();
	case SIN:
		acceptIt();
		accept(LPAR);
		ans = sin(parsePriority(MAX_P));
		accept(RPAR);
		return ans;
	case COS:
		acceptIt();
		accept(LPAR);
		ans = cos(parsePriority(MAX_P));
		accept(RPAR);
		return ans;
	default:
		fprintf(stderr, "Unexpected: `%s`\n", next->spelling);
		exit(1);
	}
}

int isPriority(enum TokenType type, int priority)
{
	if (!isInfix(type)) {
		return 0;
	}
	switch (priority) {
	case 3:
		return type == PLUS || type == MINUS;
	case 2:
		return type == TIMES || type == DIVIDE;
	case 1:
		return type == POW;
	default:
		return 0;
	}
}
