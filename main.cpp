#include <iostream>
#include "board.hpp"
#include <fstream>

void test_print_board()
{
	chess::board board;
	std::cout << board;

}

void play_cmd()
{
	std::ifstream file;
	file.open("/home/hina/Code/chess/test.txt");
	if (!file.is_open())
	{
		std::cout << "file not found" << std::endl;
		return;
	}
	chess::board board;
	std::string cmd;
	int from, to;
	while (file.is_open())
	{
		file >> cmd;
		if (cmd == "quit")
			break;
		try
		{
			from = chess::board::get_pos(cmd.substr(0, 2));
			to = chess::board::get_pos(cmd.substr(2, 2));
		}
		catch (std::invalid_argument &e)
		{
			std::cout << "invalid argument" << e.what() << std::endl;
			continue;
		}

		if (!board.move(from, to))
			std::cout << "Invalid move" << std::endl;

		std::cout << board() << std::endl;
		std::cout << board << std::endl;
	}
}

int main()
{
	play_cmd();

	return 0;
}
