#include "9cc.h"

LVar *locals;

Token *consume_ident() {
  if (token->kind == TK_IDENT) {
    Token *tok = token;
    token = token->next;

    return tok;
  }

  return false;
}

bool consume_return() {
  if (token->kind == TK_RETURN) {
    token = token->next;

    return true;
  }

  return false;
}

LVar *find_lvar(Token *tok) {
  for (LVar *var = locals; var; var = var->next)
    if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
      return var;
  return NULL;
}

Node *new_node(NodeKind kind) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;

  return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  node->lhs = lhs;
  node->rhs = rhs;

  return node;
}

Node *new_num(int val) {
  Node *node = new_node(ND_NUM);

  node->val = val;
  return node;
}

Node *code[100]; // 100個のステートメント情報を格納する

Node *stmt() {
  Node *node;

  if (consume_return()) {
    node = new_node(ND_RETURN);
    node->lhs = expr();
  } else {
    node = expr();
  }

  expect(";");

  return node;
}

Node *expr() {
  return assign();
}

Node *assign() {
  Node *node = equality();

  for (;;) {
    if (consume("="))
      node = new_binary(ND_ASSIGN, node, assign());
    else
      return node;
  }
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_binary(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_binary(ND_NE, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<"))
      node = new_binary(ND_LT, node, add());
    else if (consume("<="))
      node = new_binary(ND_LE, node, add());
    else if (consume(">"))
      node = new_binary(ND_LT, add(), node);
    else if (consume(">="))
      node = new_binary(ND_LE, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_binary(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_binary(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_binary(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_binary(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_binary(ND_SUB, new_num(0), unary());
  return primary();
}

Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  Token *tok = consume_ident();

  if (tok) {
    Node *node = new_node(ND_LVAR);

    LVar *lvar = find_lvar(tok); // 名前で検索

    if (lvar) {
      node->offset = lvar->offset;
    } else {
      // lvarを作成する
      lvar = calloc(1, sizeof(LVar));
      lvar->next = locals;
      lvar->name = tok->str;
      lvar->len = tok->len;

      if (!locals) {
        lvar->offset = 8;
      } else {
        lvar->offset = locals->offset + 8;
      }

      locals = lvar;

      node->offset = lvar->offset;
    }

    return node;
  }

  return new_num(expect_number());
}

void program() {
  int i = 0;

  while(!at_eof())
    code[i++] = stmt();

  code[i] = NULL;
};
