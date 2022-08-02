# Go 语言 plan 9汇编

## plan9 的常见指令

## 寄存器

有4个核心的伪寄存器，这4个寄存器是编译器用来维护上下文、特殊标识等作用的：
FP(Frame pointer): arguments and locals
PC(Program counter): jumps and branches
SB(Static base pointer): global symbols
SP(Stack pointer): top of stack
还有很多其他的用法，就先不摘抄了，后面用到再去查吧。

### plan9 中栈操作并没有使用push，pop，而是采用sub 跟add SP。

SUBQ $0x18, SP // 对 SP 做减法，为函数分配函数栈帧
ADDQ $0x18, SP // 对 SP 做加法，清除函数栈帧
数据copy

MOVB $1, DI      // 1 byte 
MOVW $0x10, BX   // 2 bytes
MOVD $1, DX      // 4 bytes
MOVQ $-10, AX     // 8 bytes

### 计算指令

ADDQ  AX, BX   // BX += AX
SUBQ  AX, BX   // BX -= AX
IMULQ AX, BX   // BX *= AX


### 跳转

1. 无条件跳转
   JMP addr   // 跳转到地址，地址可为代码中的地址，不过实际上手写不会出现这种东西
   JMP label  // 跳转到标签，可以跳转到同一函数内的标签位置
   JMP 2(PC)  // 以当前指令为基础，向前/后跳转 x 行
   JMP -2(PC) // 同上

2. 有条件跳转
   JNZ target // 如果 zero flag 被 set 过，则跳转
   

### 变量声明

在汇编里所谓的变量，一般是存储在 .rodata 或者 .data 段中的只读值。对应到应用层的话，就是已初始化过的全局的 const、var、static 变量/常量。
DATA    symbol+offset(SB)/width, value
使用 DATA 结合 GLOBL 来定义一个变量
GLOBL 必须跟在 DATA 指令之后:
DATA age+0x00(SB)/4, $18  // forever 18
GLOBL age(SB), RODATA, $4

DATA pi+0(SB)/8, $3.1415926
GLOBL pi(SB), RODATA, $8

DATA birthYear+0(SB)/4, $1988
GLOBL birthYear(SB), RODATA, $4

### 函数声明

先看一个定义：
// func add(a, b int) int
//   => 该声明定义在同一个 package 下的任意 .go 文件中
//   => 只有函数头，没有实现
TEXT pkgname·add(SB), NOSPLIT, $0-8
    MOVQ a+0(FP), AX
    MOVQ a+8(FP), BX
    ADDQ AX, BX
    MOVQ BX, ret+16(FP)
    RET
代码存储在TEXT段中。
pkgname 可以省略。
比如你的方法是 runtime·main，在编译之后的程序里的符号则是 runtime.main。
                             参数及返回值大小
                                  | 
 TEXT pkgname·add(SB),NOSPLIT,$32-32
       |        |               |
      包名     函数名         栈帧大小(局部变量+可能需要的额外调用函数的参数空间的总大小，但不包括调用其它函数时的 ret address 的大小)

### go程序如何转换为plan9 ?

// 编译
go build -gcflags="-S"
go tool compile -S hello.go
go tool compile -N -S hello.go // 禁止优化
// 反编译
go tool objdump <binary>

