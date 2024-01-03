debug:
	g++ ./source/main.cpp -g -fsanitize=address -Wall -Wextra -o ./binary/wave.elf

release:
	g++ ./source/main.cpp -Wall -Wextra -o ./binary/wave.elf
