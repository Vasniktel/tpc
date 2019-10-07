//
// Created by vasniktel on 15.04.19.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../include/parser.h"
#include "../include/scanner.h"

typedef struct {
  const Token* curr;
  const Token* args_begin;
  const Token* args_end;
} Parser;

Parser parser = {NULL, NULL, NULL};

static AST* ast_alloc(Op op, AST* a, AST* b, int n) {
  AST* node = malloc(sizeof(AST));
  if (node == NULL) return NULL;
  node->op = op;
  node->a = a;
  node->b = b;
  node->n = n;
  return node;
}

void ast_free(AST* node) {
  free(node);
}

void asts_free(AST* tree) {
  if (tree == NULL) return;
  asts_free(tree->a);
  asts_free(tree->b);
  ast_free(tree);
}

AST* ast_arg(int n) {
  return ast_alloc(arg, NULL, NULL, n);
}

AST* ast_imm(int n) {
  return ast_alloc(imm, NULL, NULL, n);
}

AST* ast_bin(Op op, AST* a, AST* b) {
  return ast_alloc(op, a, b, 0);
}

static const Token* advance() {
  return parser.curr ? parser.curr = parser.curr->next : NULL;
}

static const Token* peek() {
  return parser.curr;
}

static const Token* next() {
  return parser.curr ? parser.curr->next : NULL;
}

static const Token* arg_list_end() {
  if (peek() == NULL) return NULL;

  if (peek()->type != LBRA) {
    fprintf(stderr, "Left bracket missing\n");
    return NULL;
  }

  while (advance() && peek()->type != RBRA) {
    if (peek()->type != VAR) {
      fprintf(stderr, "Invalid token in argument list\n");
      return NULL;
    }
  }

  if (peek() == NULL) {
    fprintf(stderr, "Right bracket missing\n");
    return NULL;
  }

  return peek();
}

static int find_arg(const char* arg) {
  int i = 0;
  for (const Token* t = parser.args_begin->next; t != parser.args_end; t = t->next) {
    if (strcmp(t->val, arg) == 0) return i;
    ++i;
  }

  fprintf(stderr, "Unknown parameter name used\n");
  return -1;
}

static AST* expression();

static AST* factor() {
  if (next() == NULL) return NULL;

  switch (next()->type) {
    case NUM: return ast_imm(atoi(advance()->val));
    case VAR: {
      int pos = find_arg(advance()->val);
      return pos == -1 ? NULL : ast_arg(pos);
    }

    case LPAREN: {
      advance();
      AST* expr = expression();
      if (expr == NULL) return NULL;

      if (!next() || next()->type != RPAREN) {
        fprintf(stderr, "Missing right parentheses\n");
        asts_free(expr);
        return NULL;
      }

      advance();
      return expr;
    }

    default:
      fprintf(stderr, "Syntax error\n");
      return NULL;
  }
}

static AST* term() {
  if (next() == NULL) return NULL;

  AST* expr = factor();
  if (expr == NULL) return NULL;

  while (next() && (next()->type == MUL || next()->type == DIV)) {
    Op op = advance()->type == MUL ? mul : div;
    AST* right = factor();
    if (right == NULL) goto error;
    expr = ast_bin(op, expr, right);
  }

  return expr;

error:
  asts_free(expr);
  return NULL;
}

static AST* expression() {
  if (next() == NULL) return NULL;

  AST* expr = term();
  if (expr == NULL) return NULL;

  while (next() && (next()->type == ADD || next()->type == SUB)) {
    Op op = advance()->type == ADD ? plus : min;
    AST* right = term();
    if (right == NULL) goto error;
    expr = ast_bin(op, expr, right);
  }

  return expr;

error:
  asts_free(expr);
  return NULL;
}

AST* parse(const Token* tokens) {
  if (tokens == NULL) return NULL;

  parser.curr = parser.args_begin = tokens;
  if ((parser.args_end = arg_list_end()) == NULL)
    return NULL;
  AST* tree = expression();

  parser = (Parser) {NULL, NULL, NULL};
  return tree;
}

void asts_print(const AST* tree) {
  if (tree == NULL) return;

  switch (tree->op) {
    case imm:  printf("(imm %d)", tree->n); return;
    case arg:  printf("(arg %d)", tree->n); return;
    case plus: printf("(plus "); break;
    case min:  printf("(min "); break;
    case mul:  printf("(mul "); break;
    case div:  printf("(div "); break;
  }

  asts_print(tree->a);
  printf(" ");
  asts_print(tree->b);
  printf(")");
}
