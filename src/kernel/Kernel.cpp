#define SPACE 0x0020
#define UNDERSCORE 0x005f
#define PERIOD 0x002e
#define TWO 0x0032
#define C 0x0043
#define M 0x004D
#define F 0x0046
#define O 0x004F
#define S 0x0053
#define m 0x006d
#define NUL 0x1F201f20

#define COLOR 0x1F00

int TITLE [11] = {C, M, F, UNDERSCORE, O, S, SPACE, m, PERIOD, TWO, NUL};

extern "C" void _start() {
	int mem_addr = 0xb8000;
	int character;

	for (int i = 0; i <= 10; i++) {
		int data = TITLE[i];
		character = NUL;

		if (data != NUL) {
			character = COLOR + data;
		}

		int* ptr = (int*)mem_addr;
		*ptr = character;

		if (data != NUL) mem_addr += 0x00002;
		else mem_addr += 0x00004;
	}

	return;
}
