#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン(のポインタ)
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
};

// 現在着目しているトークン
Token *token;

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char op) { // opは'+'なので、足し算ならtrue
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("'%c'ではありません", op);
  token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
  if (token->kind != TK_NUM)
    error("数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token)); // 32byte確保

  tok->kind = kind; // トークンの型(記号 or 数字 or 終端)
  tok->str = str; // strは文字列のポインタ(char *str)

  cur->next = tok; // 引数で渡されたトークンポインタ(cur)のnextを定義

  return tok; // 作ったトークンのポインタを返す
}

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      // p++は、pを渡す(p++を渡さない)が、その後にインクリメントされる
      cur = new_token(TK_RESERVED, cur, p++); // Tokenポインタ(TK_RESERVEDで)を作成してcur->nextに代入。作成したTokenポインタを返す
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);

      // 数値に変換できない最初の文字のポインタをpに代入する
      cur->val = strtol(p, &p, 10); // ここでpがインクリメントされる
      continue;
    }

    error("トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("引数の個数が正しくありません");
    return 1;
  }

  token = tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // 式の最初は数でなければならない
  printf("  mov rax, %d\n", expect_number());

  // `+ <数>`あるいは`- <数>`というトークンの並びを消費しつつアセンブリを出力
  while (!at_eof()) {
    if (consume('+')) { // +ならtokenを進めてtrueを返す。-ならfalseを返す
      printf("  add rax, %d\n", expect_number()); // expect_numberは現在の数字を返して、tokenを進める
      continue;
    }

    expect('-'); // consumeと違い、boolではなくerrorを出す。tokenは進める
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
