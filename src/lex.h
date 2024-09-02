#ifndef LEX_H
#define LEX_H

enum TokenType {
	NUMBER,
	LPAR,
	RPAR,
	PLUS,
	MINUS,
	TIMES,
	DIVIDE,
	POW,
	SIN,
	COS,
	TOKEN_EOF,
	LINE_END
};

static const char *TokenStrings[] = {
	"Number",
	"(",
	")",
	"+",
	"-",
	"*",
	"/",
	"^",
	"sin",
	"cos",
	"EOF",
	"\\n"
};

struct Token {
	enum TokenType type;
	char *spelling;
};

int isInfix(enum TokenType);
struct Token *peek(void);
void acceptIt(void);
void accept(enum TokenType);

#endif
