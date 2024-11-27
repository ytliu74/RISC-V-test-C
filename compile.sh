include="-I . -I ./sw/libs/uart_lib/include"
ld_flag="-T linker.ld"
flag="-mabi=lp64 -march=rv64g -mcmodel=medany -Os -static -fvisibility=hidden -nostdlib -nostartfiles -fomit-frame-pointer"
files="$target.c message.S ./sw/libs/uart_lib/uart.c ./sw/libs/uart_lib/conio.c ./sw/libs/uart_lib/console.c ./sw/libs/uart_lib/vsprintf.c ./sw/libs/uart_lib/skip_spaces.c ./sw/libs/uart_lib/ctype.c ./sw/libs/uart_lib/strnlen.c"


riscv64-unknown-elf-gcc $include $ld_flag $flag $files -DCONFIG_DEV_CON_UART_MMIO_BASE=0x10000000 -DPERI_CORE_CLOCK_DIV=54 -DCONFIG_DEV_CON_UART_BPS=115200 -o ./out_files/$target.elf