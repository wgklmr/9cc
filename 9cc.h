#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_RETURN,
  TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

typedef struct LVar LVar;
struct LVar {
  LVar *next; // 次の変数かNULL
  char *name; // 変数の名前
  int len;    // 名前の長さ
  int offset; // RBPからのオフセット
};

typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_NUM, // 整数
  ND_ASSIGN, // =
  ND_LVAR, // ローカル変数
  ND_RETURN, // 右辺なし
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
  int offset;    // kindがND_LVARの場合のみ使う
};

int expect_number();
bool at_eof();
bool startswith(char *p, char *q);
bool consume(char *op);
bool consume_return();
int is_alnum(char c);
void expect(char *op);
void error(char *fmt, ...);
void gen(Node *node);
void program();

Token *tokenize(char *p);
Token *consume_ident();
Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_num(int val);
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
LVar *find_lvar(Token *tok);

extern Token *token;
extern Node *code[];
extern char *user_input;
extern LVar *locals;
