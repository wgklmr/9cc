#include "9cc.h"

void gen(Node *node) {
  switch (node->kind) {
  case ND_NUM:
    // 数値をスタックに積む
    printf("  push %d\n", node->val);
    return;
  case ND_LVAR:
    // 変数のアドレスをスタックに積む
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");

    // 変数の値をスタックに積む
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    // 変数(左辺)のアドレスをスタックに積む
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->lhs->offset);
    printf("  push rax\n");

    // 右辺が数値ならスタックに積む
    gen(node->rhs);

    // 右辺の値を変数にストアする
    printf("  pop rdi\n"); // 右辺
    printf("  pop rax\n"); // 左辺
    printf("  mov [rax], rdi\n");
    printf("  push rax\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}
