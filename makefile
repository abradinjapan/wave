debug:
	g++ ./source/main.cpp -g -fsanitize=address -Wall -Wextra -o ./../wave.elf

release:
	g++ ./source/main.cpp -Wall -Wextra -o ./../wave.elf
