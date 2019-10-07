//
// Created by vasniktel on 15.04.19.
//

#include <stdio.h>
#include <assert.h>

#include "../include/tpc.h"
#include "../include/scanner.h"
#include "../include/util.h"
#include "../include/generator.h"

char* compile(const char* program) {
  return pass3(pass2(pass1(program)));
}

AST* pass1(const char* program) {
  Token* tokens = tokenize(program);
  AST* tree = parse(tokens);
  tokens_free(tokens);
  return tree;
}

AST* pass2(AST* ast) {
  if (ast == NULL || ast->op == imm || ast->op == arg)
    return ast;

  ast->a = pass2(ast->a);
  ast->b = pass2(ast->b);

  if (ast->a->op != imm || ast->b->op != imm) return ast;

  int result;
  switch (ast->op) {
    case plus: result = ast->a->n + ast->b->n; break;
    case min:  result = ast->a->n - ast->b->n; break;
    case mul:  result = ast->a->n * ast->b->n; break;
    case div:  result = ast->a->n / ast->b->n; break;
    default:   assert(0);
  }

  asts_free(ast);
  ALLOC_CHECK(ast, ast_imm(result), ast = NULL)
  return ast;
}

char* pass3(AST* ast) {
  char* result = generate(ast);
  asts_free(ast);
  return result;
}
