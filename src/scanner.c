//
// Created by vasniktel on 15.04.19.
//

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../include/scanner.h"
#include "../include/util.h"

Token* token_alloc(TokType type, const char* val) {
  Token* token = malloc(sizeof(Token));
  if (token == NULL) return NULL;
  token->type = type;
  token->val = val;
  token->next = NULL;
  return token;
}

void token_free(Token* token) {
  if (token == NULL) return;
  free((void*) token->val);
  free(token);
}

void tokens_free(Token* tokens) {
  while (tokens != NULL) {
    Token* old = tokens;
    tokens = tokens->next;
    token_free(old);
  }
}

static const char* num(const char* arr) {
  if (arr == NULL) return NULL;
  while (isdigit(*arr)) ++arr;
  return !isalpha(*arr) ? arr : NULL;
}

static const char* var(const char* arr) {
  if (arr == NULL) return NULL;
  while (isalpha(*arr)) ++arr;
  return !isdigit(*arr) ? arr : NULL;
}

Token* tokenize(const char* program) {
  if (program == NULL) return NULL;

  Token tokens = {.next = NULL}; // dummy
  Token* curr = &tokens;

  for (char c = *program; c != '\0'; c = *(++program)) {
    switch (c) {
      case LPAREN:
      case RPAREN:
      case LBRA:
      case RBRA:
      case ADD:
      case SUB:
      case MUL:
      case DIV: {
        ALLOC_CHECK(curr->next, token_alloc(c, NULL), goto clear)
        curr = curr->next;
        break;
      }

      case ' ':
      case '\t':
      case '\n': break;

      default: {
        const char* after = NULL;
        TokType type = -1;

        if (isdigit(c)) {
          after = num(program);
          type = NUM;
        } else if (isalpha(c)) {
          after = var(program);
          type = VAR;
        }

        if (after == NULL) {
          fprintf(stderr, "Invalid syntax\n");
          goto clear;
        }

        size_t size = after - program;
        char* val;
        ALLOC_CHECK(val, malloc((size + 1) * sizeof(char)), goto clear)
        strncpy(val, program, size);
        val[size] = '\0';

        assert(type != -1);
        ALLOC_CHECK(curr->next, token_alloc(type, val), goto clear_val)
        curr = curr->next;
        program = after - 1;
        break;

      clear_val:
        free(val);
        goto clear;
      }
    }
  }

  return tokens.next;
clear:
  tokens_free(tokens.next);
  return NULL;
}

void tokens_print(const Token* tokens) {
  for (; tokens != NULL; tokens = tokens->next) {
    switch (tokens->type) {
    case LPAREN:
    case RPAREN:
    case LBRA:
    case RBRA:
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      printf("<%c> ", tokens->type);
      break;
    case NUM:
    case VAR:
      printf("<%c : %s> ", tokens->type, tokens->val);
      break;
    }
  }
}
