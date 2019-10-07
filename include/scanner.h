//
// Created by vasniktel on 15.04.19.
//

#ifndef TPC__SCANNER_H
#define TPC__SCANNER_H

typedef enum {
  LPAREN = '(',
  RPAREN = ')',
  LBRA   = '[',
  RBRA   = ']',
  ADD    = '+',
  SUB    = '-',
  MUL    = '*',
  DIV    = '/',
  NUM    = 'N',
  VAR    = 'V'
} TokType;

typedef struct Token {
  TokType type;
  struct Token* next;
  const char* val;
} Token;

Token* token_alloc(TokType type, const char* val);

void token_free(Token* token);
void tokens_free(Token* tokens);

void tokens_print(const Token* tokens);

Token* tokenize(const char* program);

#endif //TPC__SCANNER_H
