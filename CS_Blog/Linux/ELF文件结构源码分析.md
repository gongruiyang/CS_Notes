## 一、ELF文件头结构体：Elf32_Ehdr

查看ELF文件的文件头：

```shell
[gongruiyang@localhost ws]$ readelf -h SimpleSection.o
ELF 头：
      Magic：  7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
      Class:                             ELF64
      Data:                             2 补码，小端序 (little endian)
      Version:                          1 (current)
      OS/ABI:                           UNIX - System V
      ABI Version:                      0
      Type:                             REL (可重定位文件)
      Machine:                          Advanced Micro Devices X86-64
      Version:                           0x1
      Entry point address：              0x0
      Start of program headers：         0 (bytes into file)
      Start of section headers:          1064 (bytes into file)
      Flags：                             0x0
      本头的大小：                        64 (字节)
      程序头大小：                        0 (字节)
      Number of program headers:         0
      节头大小：                          64 (字节)
      节头数量：                          13
      字符串表索引节头：                   12
```

> 可见该文件的`ELF Header`的大小为64字节（0x40）

与该文件头对应的内核源码中的结构体为：`include\linux\elf.h`

```c
typedef struct elf32_hdr{
    unsigned char	e_ident[EI_NIDENT /* 16 */];  /* ELF魔法数字 Magic */
    Elf32_Half/* unsigned short */	 e_type;     /* ELF文件类型 */
    Elf32_Half/* unsigned short */	 e_machine;  /* ELF文件的CPU平台属性，相关常量以EM_开头 */
    Elf32_Word/* unsigned int */	   	e_version;  /* ELF版本号，一般为常数 1 */
    Elf32_Addr/* unsigned int */	   e_entry;    /* ELF程序的入口地址，可重定位文件一般无入口地址设置为0 */
    Elf32_Off /* unsigned int */	   e_phoff;    /*  */
    Elf32_Off /* unsigned int */	   e_shoff;    /* 段表在文件中的偏移，也就是段表从文件的第e_shoff+1个字节开始 */
    Elf32_Word/* unsigned int */	   e_flags;    /* ELF标志位*/
    Elf32_Half/* unsigned short */	 e_ehsize;   /* ELF文件头本身大小 */
    Elf32_Half/* unsigned short */	 e_phentsize;/*  */
    Elf32_Half/* unsigned short */	 e_phnum;    /*  */
    Elf32_Half/* unsigned short */	 e_shentsize;/* 段表描述符的大小，等于 sizeof Elf32_Shdr */
    Elf32_Half/* unsigned short */	 e_shnum;    /* 段表描述符的数量，等于ELF文件中拥有段的数量 */
    Elf32_Half/* unsigned short */	 e_shstrndx; /* 段表字符串表 所在的段 在段表中的下标 */
} Elf32_Ehdr;
```

### e_ident

`e_ident`是一个16个字节大小的数组，正好与 `Magic`中16个字节的数据一一对应

> ELF标记：
>
> ```c
> #define	ELFMAG0		0x7f		/* EI_MAG */
> #define	ELFMAG1		'E'
> #define	ELFMAG2		'L'
> #define	ELFMAG3		'F'
> ```
>
> ELF文件类：
>
> ```c
> #define	ELFCLASSNONE	0		/* EI_CLASS */
> #define	ELFCLASS32	1
> #define	ELFCLASS64	2
> #define	ELFCLASSNUM	3
> ```
>
> 字节序：
>
> ```c
> #define ELFDATANONE	0		/* e_ident[EI_DATA] */
> #define ELFDATA2LSB	1
> #define ELFDATA2MSB	2
> ```
>
> ELF版本：
>
> ```c
> #define EV_NONE		0		/* e_version, EI_VERSION */
> #define EV_CURRENT	1
> #define EV_NUM		2
> ```

![image-20211219194616061](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112191946647.png)



### e_type

`e_type`表示ELF文件类型的，有以下常量表示

```c
/* These constants define the different elf file types */
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
```

| 常量    | 值   | 含义                          |
| ------- | ---- | ----------------------------- |
| ET_REL  | 1    | 可重定位文件，一般为 .o 文件  |
| ET_EXEC | 2    | 可执行文件，例如 a.out        |
| ET_DYN  | 3    | 共享目标文件，一般为 ,so 文件 |
| ET_CORE | 4    | core dump 文件                |



### e_machine

`e_machine`表示机器类型，ELF文件格式被设计成可以在多个平台下使用，e_machine 成员就表示该平台属性，比如 e_machine 值为 3 表示该ELF文件只能在 Intel x86 机器下使用

```c
/* These constants define the various ELF target machines */
#define EM_NONE		0
#define EM_M32		1
#define EM_SPARC	2
#define EM_386		3
#define EM_68K		4
#define EM_88K		5
#define EM_486		6	/* Perhaps disused */
#define EM_860		7
```

| 常量     | 值   | 含义           |
| -------- | ---- | -------------- |
| EM_M32   | 1    | AT&T WE 32100  |
| EM_SPARC | 2    | SPARC          |
| EM_386   | 3    | Intel x86      |
| EM_68K   | 4    | Motorola 68000 |
| EM_88K   | 5    | Motorola 88000 |
| EM_869   | 7    | Intel 80860    |

## 二、段表描述符结构体：Elf32_Shdr

使用`objdump -h`命令看到的是 ELF 文件中包含的关键段显示出来，而省略了其他辅助性的段，比如：符号表、字符串表、段名字符串表、重定位表等，我们可以使用`readelf -S`命令来看完整的段表结构：

```shell
[gongruiyang@localhost ws]$ readelf -S SimpleSection.o
共有 13 个节头，从偏移量 0x428 开始：

节头：
[号] 名称              类型     地址              偏移量    大小                全体大小          旗标 链接 信息 对齐
[ 0]                   NULL     0000000000000000  00000000  0000000000000000  0000000000000000         0  0    0
[ 1] .text             PROGBITS 0000000000000000  00000040  0000000000000055  0000000000000000  AX     0  0    1
[ 2] .rela.text        RELA     0000000000000000  00000318  0000000000000078  0000000000000018   I    10  1    8
[ 3] .data             PROGBITS 0000000000000000  00000098  0000000000000008  0000000000000000  WA     0  0    4
[ 4] .bss              NOBITS   0000000000000000  000000a0  0000000000000004  0000000000000000  WA     0  0    4
[ 5] .rodata           PROGBITS 0000000000000000  000000a0  0000000000000004  0000000000000000   A     0  0    1
[ 6] .comment          PROGBITS 0000000000000000  000000a4  000000000000002d  0000000000000001  MS     0  0    1
[ 7] .note.GNU-stack   PROGBITS 0000000000000000  000000d1  0000000000000000  0000000000000000         0  0    1
[ 8] .eh_frame         PROGBITS 0000000000000000  000000d8  0000000000000058  0000000000000000   A     0  0    8
[ 9] .rela.eh_frame    RELA     0000000000000000  00000390  0000000000000030  0000000000000018   I    10  8    8
[10] .symtab           SYMTAB   0000000000000000  00000130  0000000000000180  0000000000000018        11  11   8
[11] .strtab           STRTAB   0000000000000000  000002b0  0000000000000067  0000000000000000         0  0    1
[12] .shstrtab         STRTAB   0000000000000000  000003c0  0000000000000061  0000000000000000         0  0    1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
```

根据偏移量可以画出如下所示图解：

![image-20211219222459589](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112192224655.png)

与之相对应内核源码中的结构体为：`include\linux\elf.h`

```c
typedef struct {
    Elf32_Word/*unsigned int*/	sh_name;
    Elf32_Word/*unsigned int*/	sh_type;
    Elf32_Word/*unsigned int*/	sh_flags;
    Elf32_Addr/*unsigned int*/	sh_addr;
    Elf32_Off/*unsigned int*/	sh_offset;
    Elf32_Word/*unsigned int*/	sh_size;
    Elf32_Word/*unsigned int*/	sh_link;
    Elf32_Word/*unsigned int*/	sh_info;
    Elf32_Word/*unsigned int*/	sh_addralign;
    Elf32_Word/*unsigned int*/	sh_entsize;
} Elf32_Shdr;

typedef struct elf64_shdr {
    Elf64_Word/*unsigned int*/ sh_name;		/* Section name, index in string tbl */
    Elf64_Word/*unsigned int*/ sh_type;		/* Type of section */
    Elf64_Xword/*unsigned long long*/ sh_flags;		/* Miscellaneous section attributes */
    Elf64_Addr/*unsigned long long*/ sh_addr;		/* Section virtual addr at execution */
    Elf64_Off/*unsigned long long*/ sh_offset;		/* Section file offset */
    Elf64_Xword/*unsigned long long*/ sh_size;		/* Size of section in bytes */
    Elf64_Word/*unsigned int*/ sh_link;		/* Index of another section */
    Elf64_Word/*unsigned int*/ sh_info;		/* Additional section information */
    Elf64_Xword/*unsigned long long*/ sh_addralign;	/* Section alignment */
    Elf64_Xword/*unsigned long long*/ sh_entsize;	/* Entry size if section holds table */
} Elf64_Shdr;
```

结构体中成员变量含义如表所示：

| 成员变量           | 含义       |
| ------------------ | ---------- |
| sh_name            | 段名       |
| sh_type            | 段的类型   |
| sh_flags           | 段的标志位 |
| sh_addr            | 段虚地址   |
| sh_offset          | 段偏移量   |
| sh_size            | 段的长度   |
| sh_link 和 sh_info | 段链接信息 |
| sh_addralign       | 段地址对齐 |
| sh_entsize         | 项的长度   |

* `sh_name`：段名是一个字符串，位于一个叫做 ".shstrtab" 的字符串表，sh_name中保存的是一个数值表示段名字符串在 ”.shstrtab“ 中的偏移
* `sh_addr`：如果该段可以被加载，则 sh_addr 为该段被加载后在进程地址空间中的虚拟地址，否则为0
* `sh_offset`：若该段位于文件中，则表示该段在文件中的偏移；否则无意义
* `sh_addralign`：若该值为 0 或 1 表示无对齐要求，其他值则需按照对应值对齐
* `sh_entsize`：有些段（例如：符号表）包含一些固定大小的项，对于这种段，sh_entsize 表示每个项的大小，如果该值为 0 表示该段不包含固定大小的项

### sh_type

`sh_type`表示段的类型，使用不同的常量来区分不同的类型

```c
/* sh_type */
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff
```

| 常量         | 值   | 含义                   |
| ------------ | ---- | ---------------------- |
| SHT_NULL     | 0    | 无效段                 |
| SHT_PROGBITS | 1    | 程序段、代码段、数据段 |
| SHT_SYMTAB   | 2    | 符号表                 |
| SHT_STRTAB   | 3    | 字符串表               |
| SHT_RELA     | 4    | 重定位表               |
| SHT_HASH     | 5    | 符号表的哈希表         |
| SHT_DYNAMIC  | 6    | 动态连接信息           |
| SHT_NOTE     | 7    | 提示性信息             |
| SHT_NOBITS   | 8    | 表示该段在文件中没内容 |
| SHT_REL      | 9    | 该段包含重定位信息     |
| SHT_SHLIB    | 10   | 保留段                 |
| SHT_DYNSYM   | 11   | 动态链接的符号表       |

### sh_flag

`sh_flag`：段的标志位表示该段在进程虚拟地址空间中的属性，比如是否可写、是否可执行等

```c
/* sh_flags */
#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000
```

| 常量          | 值   | 含义                                      |
| ------------- | ---- | ----------------------------------------- |
| SHF_WRITE     | 1    | 表示该段在进程虚拟地址空间中 可写         |
| SHF_ALLOC     | 2    | 表示该段在进程虚拟地址空间中 需要分配空间 |
| SHF_EXECINSTR | 4    | 表示该段在进程虚拟地址空间中 可以被执行   |



## 三、常见系统保留段的属性表

由以下的表我们可以知道：`一个段是否是可读写或执行，是取决于该段对应内核中的结构体的一个成员变量决定的`

| Name     | sh_type                                | sh_flag             |
| -------- | -------------------------------------- | ------------------- |
| .bss     | SHT_NOBITS (表示该段无内容)            | 需分配空间 + 可写   |
| .comment | SHT_PROGBITS（程序段、代码段、数据段） | none                |
| .data    | SHT_PROGBITS（数据段）                 | 需分配空间 + 可写   |
| .rodata  | SHT_PROGBITS（数据段）                 | 需分配空间          |
| .symtab  | SHT_SYMTAB（符号表）                   | 需分配空间          |
| .text    | SHT_PROGBITS（程序段）                 | 需分配空间 + 可执行 |
| .strtab  | SHT_STRTAB（字符串表）                 | 需分配空间          |

## 四、重定位表和字符串表

### 重定位表

`.rela.text`该段的类型为`SHT_RELA`，即该段是一个重定位表。

重定位：链接器在处理目标文件时，需要对代码段和数据段中一些对绝对地址进行引用的位置进行重定位

该段的`sh_link`表示符号表的下标，`sh_info`表示作用于哪个段

### 字符串表

ELF 文件中用到了字符串，例如：段名称、变量名等。

因为字符串的长度是不定的，所以用固定结构表示较为困难，一种常见的存储方法是：把字符串集中起来存放到一个表中，然后使用字符串在表中的偏移量来引用字符串，如表所示：

![image-20211220100527352](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201005737.png)

那么偏移量与字符串对应关系如下表所示：

![image-20211220100608248](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201006304.png)

段中最常见的两个字符串表分别是 `.strtab(字符串表 String Table)`和`.shstrtab(段表字符串表 Section Header String Table)`

* 字符串表：用来保存普通的字符串，例如：符号名称
* 段表字符串表：用来保存段表中用到的字符串，例如：段名

通过`readelf -h SimpleSection.o`能够看到最后一个`     字符串表索引节头：12`，表示`.shstrtab`在段表中的下标，即`[12] .shstrtab`
