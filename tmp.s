.intel_syntax noprefix
.globl main
main:
  push rbp
  mov rbp, rsp
  sub rsp, 208
  push 5
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  push 0
  pop rax
  push 8
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  push 0
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  pop rax
  ret
