debug:
	g++ ./source/main.cpp -g -fsanitize=address -Wall -Wextra -o ./../wave.elf

release:
	g++ ./source/main.cpp -Wall -Wextra -o ./../wave.elf

test:
	./../wave.elf programs/booleans.wave
	./../wave.elf programs/loop.wave
	./../wave.elf programs/codegen.wave
	./../wave.elf programs/context_io.wave
	./../wave.elf programs/files.wave
	./../wave.elf programs/math.wave
	./../wave.elf programs/literals.wave
