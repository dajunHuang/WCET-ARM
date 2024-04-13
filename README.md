编译器：[arm-none-eabi-gcc](https://developer.arm.com/downloads/-/gnu-rm)


<!-- 周永师兄虚拟机里`~/chronos-demo`里的原版工具demo是可以用的，我就照着这里的实现做了一个功能类似的arm版本，原`~/chronos-demo/est`目录我换成了`~/chronos/Desktop/WCET/est`,用Makefile编译。

考虑到benchmark程序都没有使用标准库，编译选项我加上了`-nostdlib`，这样编译后的程序更简洁，没有无关代码，便于分析，但程序入口函数变为`_start`。

本仓库下`est-arm`相比于师兄虚拟机里`~/chronos/Desktop/WCET/est`:

- 我在文件中加了很多函数的声明，防止编译警告或出错。

- 注释掉`main`函数中写死的部分。

- `cfg.h`中`prog_t`结构体的`main_addr`被我改为`entry_addr`，原意为`main`函数入口地址，但函数入口变成`_start`，不再有`main`函数。现意为程序入口地址即`_start`函数的地址。结构体的`main_proc`被我改为`start_proc`，原意为`main`函数在所有函数中的序号，现意为`_start`函数在所有函数中的序号。

- `infeasible.c`63行的`main_proc`改为`start_proc`，理由同上。

- 因为编译后的程序中不再有多余函数，故删除`ss/ss_readfile.c`中`is_lib_func()`函数，及其调用。

- `ss/ss_readfile.c`中`lookup_addr()`函数中设置`main_addr`的部分改为设置`entry_addr`。

- `ss/ss_readfile.c`中`read_text_head()`中`while ((section_index++) < nums_of_sections)`改为`while ((section_index++) < nums_of_sections - 1)`，此处循环次数错了，`sym_loadsyms__elf()`函数608行也是。

- `ss/symbol.c`中`sym_loadsyms__elf()`函数683加上判断`(symtb.st_info & 0xf) == 2`，意思是只有函数名才加入符号表，暂时我认为加上其它符号并没有作用。

ELF文件中的符号（函数地址，函数长度等）目前能被正确识别，但指令识别比较复杂，功能是否正确我还没验证。`read_code()`函数之后的部分我也还没开始验证。 -->

  

  







