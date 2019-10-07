//
// Created by vasniktel on 17.04.19.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/generator.h"
#include "../include/util.h"
#include "../include/parser.h"

#define INIT_SIZE 256

typedef struct {
  char* buff;
  char* curr;
  size_t size;
} Buffer;

Buffer buffer = {NULL, NULL, 0};

static int buffer_init(size_t size) {
  ALLOC_CHECK(buffer.buff, malloc(size * sizeof(char)), return 1)
  buffer.curr = buffer.buff;
  buffer.size = size;
  return 0;
}

static int buffer_resize(size_t size) {
  size_t pos = buffer.curr - buffer.buff;
  Buffer tmp = buffer;
  buffer = (Buffer) {NULL, NULL, 0};
  ALLOC_CHECK(tmp.buff, realloc(tmp.buff, size * sizeof(char)), return 1)
  tmp.curr = tmp.buff + (pos < size ? pos : size);
  tmp.size = size;
  buffer = tmp;
  return 0;
}

static int buffer_write(const char* str, size_t size) {
  assert(buffer.curr != NULL);

  if (buffer.curr + size >= buffer.buff + buffer.size)
    EXPR_CHECK_NE(buffer_resize(buffer.size * 2 + size), 0, return 1)

  strncpy(buffer.curr, str, size);
  buffer.curr += size;
  return 0;
}

static int is_unary(const AST* ast) {
  assert(ast != NULL);
  return ast->op == imm || ast->op == arg;
}

static int generate_recur(const AST* ast) {
  if (ast == NULL) return 0;

  char command[16] = {0};
  switch (ast->op) {
    case arg: sprintf(command, "AR %d\n", ast->n); break;
    case imm: sprintf(command, "IM %d\n", ast->n); break;
    case plus: strcpy(command, "AD\n"); break;
    case min:  strcpy(command, "SU\n"); break;
    case mul:  strcpy(command, "MU\n"); break;
    case div:  strcpy(command, "DI\n"); break;
  }

  if (!is_unary(ast)) {
    EXPR_CHECK_NE(generate_recur(ast->a), 0, return 1)

    if (is_unary(ast->b)) {
      EXPR_CHECK_NE(buffer_write("SW\n", 3), 0, return 1)
    } else {
      EXPR_CHECK_NE(buffer_write("PU\n", 3), 0, return 1)
    }

    EXPR_CHECK_NE(generate_recur(ast->b), 0, return 1)

    if (is_unary(ast->b)) {
      if (ast->op == div || ast->op == min) {
        EXPR_CHECK_NE(buffer_write("SW\n", 3), 0, return 1)
      }
    } else {
      EXPR_CHECK_NE(buffer_write("SW\nPO\n", 6), 0, return 1)
    }
  }

  EXPR_CHECK_NE(buffer_write(command, strlen(command)), 0, return 1)
  return 0;
}

char* generate(const AST* ast) {
  if (ast == NULL) return NULL;

  EXPR_CHECK_NE(buffer_init(INIT_SIZE), 0, return NULL)
  EXPR_CHECK_NE(generate_recur(ast), 0, return NULL)

  size_t pos = buffer.curr - buffer.buff;
  EXPR_CHECK_NE(buffer_resize(pos + 1), 0, return NULL)
  char* result = buffer.buff;
  result[pos] = '\0';

  buffer = (Buffer) {NULL, NULL, 0};
  return result;
}
