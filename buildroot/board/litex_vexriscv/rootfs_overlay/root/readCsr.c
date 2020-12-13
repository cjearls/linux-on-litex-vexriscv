#include <stdio.h>
#include "VexRiscv/src/main/c/emulator/src/riscv.h"

int main() {
	int csrValue = csr_read(0xCBF);
	printf("Hello, World%d!\n", csrValue);
	return 0;
}
