#include "test.h"
#define OPCODE_OFFSET 26
#define OPCODE_SIZE 6 // 6 bits to encode an opcode (26-31)

int opcode;
opcode = get_piece_of_a_word(25, OPCODE_OFFSET, OPCODE_SIZE);
printf("%d\n", x);
