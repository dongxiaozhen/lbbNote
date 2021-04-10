
    FP: Frame pointer: arguments and locals.
    PC: Program counter: jumps and branches.
    SB: Static base pointer: global symbols.
    SP: Stack pointer: top of stack.

BP 存放当前函数的栈底位置,函数调用时，要保存到栈底

  |
  |-----------24(SP)
  | BP(原来函数的栈底)
  |-----------16(SP) -------------新BP
  | 返回值2
高|-----------12(SP)
  | 返回值1
  |-----------8(SP)
  | 参数2
  |-----------4(SP)
  | 参数1
  |-----------SP1(自己算出来的栈) ---- 新SP
  |call命令下一句的地址(8字节)
  |-----------SP(CALL指令会再在原来的栈顶再-8，存放return-address，所以函数里看到堆栈要在原来的堆栈上-8)
  V

函数
   1. 取参数
   2. 运行代码
   3. 放返回值
   4. RET

调用函数
   1. 先设置栈空间(参数+返回值+SP(8))
   2. call 
   3. 取返回值
   4. 放回原来的栈顶位置到BP
   5. 回退栈空间
   

0x0000 00000 (/work/src/test/strings/main.go:)a(SB), NOSPLIT, $0-16     16标示参数大小
函数调用需要恢复两个段：指令段和栈段，标示下一条指令的地址和上一会函数的堆栈
CALL前需要保存原函数的栈底BP到SP，保存当前函数的栈底到BP，
CALL 指令会在sp的基础上向下移动8字节0(sp)存放下调指令，当函数返回是调用RET指令，实际上去除0(sp)的地址返回

bp 栈底(高地址)
sp 栈顶(底地址)

1计算空间： num = arg_ret(参数+返回值)+8(old_BP)
2计算新sp:  sp = sp - num (栈从高地址往地址)
3保存old_BP: sp + arg_ret =  old_BP
4设置参数
5调用函数 CALL xxx (会操作IP)
6取返回值 RET (会操作IP)
7恢复BP BP = old_BP
8恢复sp sp = sp + num
