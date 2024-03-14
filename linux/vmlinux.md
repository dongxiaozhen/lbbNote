### vmlinux

- `vmlinux` 是未压缩的 Linux 内核映像。它是在 Linux 内核编译过程中生成的一个文件，包含了整个内核的代码（包括内核模式和用户模式的代码）。 是未压缩的 Linux 内核映像。它是在 Linux 内核编译过程中生成的一个文件，包含了整个内核的代码（包括内核模式和用户模式的代码）。`vmlinux` 包含了内核的符号表，因此它经常被用于调试目的。 包含了内核的符号表，因此它经常被用于调试目的。

- `vmlinux.h` 是一个由 BPF CO-RE（Compile Once, Run Everywhere）框架自动生成的头文件。它包含了内核的数据结构定义，这些定义是通过解析  是一个由 BPF CO-RE（Compile Once, Run Everywhere）框架自动生成的头文件。它包含了内核的数据结构定义，这些定义是通过解析  是一个由 BPF CO-RE（Compile Once, Run Everywhere）框架自动生成的头文件。它包含了内核的数据结构定义，这些定义是通过解析 `vmlinux` 中的 BTF（BPF Type Format）信息自动生成的。 中的 BTF（BPF Type Format）信息自动生成的。
- BTF 是一种新的内核数据类型格式，它为内核提供了一种描述数据类型的方法。通过 BTF，BPF CO-RE 可以解析 BTF 是一种新的内核数据类型格式，它为内核提供了一种描述数据类型的方法。通过 BTF，BPF CO-RE 可以解析 `vmlinux` 中的数据结构定义，并将这些定义写入  中的数据结构定义，并将这些定义写入  中的数据结构定义，并将这些定义写入 `vmlinux.h` 文件。 文件。
- `vmlinux` 和  和  和 `vmlinux.h` 之间的关系是： 之间的关系是：`vmlinux.h` 是通过解析  是通过解析  是通过解析 `vmlinux` 中的 BTF 信息生成的。 中的 BTF 信息生成的。`vmlinux` 包含了内核的代码和数据结构定义，而  包含了内核的代码和数据结构定义，而  包含了内核的代码和数据结构定义，而 `vmlinux.h` 包含了这些数据结构定义的副本，可以在编写 eBPF 程序时使用。 包含了这些数据结构定义的副本，可以在编写 eBPF 程序时使用。
- 链接