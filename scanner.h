#ifndef SOLARIA_SCANNER_H
#define SOLARIA_SCANNER_H

typedef struct scanner_t {
  const char* start;
  const char* current;
  int line;
} scanner_t;

typedef enum token_type_t {
  // Single-character tokens.
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_DOT,
  TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
  // One or two character tokens.
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,
  // Literals.
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
  // Keywords.
  TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
  TOKEN_FOR, TOKEN_FUNCTION, TOKEN_IF, TOKEN_NULL, TOKEN_OR,
  TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
  TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
  // TOKEN_ERROR is reserved for errors detected during scanning. I.e.:
  // unterminated strings, unrecognised characters, etc.
  TOKEN_ERROR,
  TOKEN_EOF
} token_type_t;


typedef struct token_t {
  token_type_t type;
  const char* start;
  int length;
  int line;
} token_t;


void init_scanner(const char* source);
token_t scan_token();

#endif
