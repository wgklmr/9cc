.intel_syntax noprefix
.globl main
main:
  push 0
  push 1
  pop rdi
  pop rax
  pop rdi
  pop rax
  cmp rax, rdi
  sete al
  movzb rax, al
  push rax
  pop rax
  ret
