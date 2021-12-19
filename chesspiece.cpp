#include "chesspiece.hpp"

namespace chess {

int get_pos(const std::string &str)
{
	if (str.size() != 2)
		throw std::invalid_argument("Invalid argument. A position is "
									"described with 2 letters. i.e. b4");
	int row = str[0] - 'a';
	int col = str[1] - '1';

	if (row > 8 or row < 0 or col > 8 or col < 0)
		throw std::invalid_argument("Invalid argument. The positions must be "
									"within the range a1 to h8");

	return row * 8 + col;
}

std::string get_str(int pos)
{
	int row = pos / 8;
	int col = pos % 8;
	return std::string(1, 'a' + row) + std::string(1, '1' + col);
}

bool king::legal_move(int to) const
{
	const int diff = to - pos;
	return to >= 0 and to < 64 and
	(diff == 1 or diff == -1 or diff == 8 or diff== -8 or
	 diff == 7 or diff == -7 or diff == 9 or diff == -9)
}

bool rook::legal_move(int to) const
{
	const int diff_col = to % 8 - pos % 8;
	const int diff_row = to / 8 - pos / 8;
	return to >= 0 and to < 64 and (diff_col == 0 or diff_row == 0);
}

bool queen::legal_move(int to) const
{
	const int diff_col = to % 8 - pos % 8;
	const int diff_row = to / 8 - pos / 8;
	return to >= 0 and to < 64 and (diff_col == 0 or diff_row == 0 or
									diff_col == diff_row or
									diff_col == -diff_row);
}

bool bishop::legal_move(int to) const
{
	const int diff_col = to % 8 - pos % 8;
	const int diff_row = to / 8 - pos / 8;
	return to >= 0 and to < 64 and (diff_col == diff_row or
									diff_col == -diff_row);
}

bool knight::legal_move(int to) const
{
	const int diff_col = to % 8 - pos % 8;
	const int diff_row = to / 8 - pos / 8;
	return to >= 0 and to < 64 and
	      (diff_col == 2  and diff_row == 1 or
		   diff_col == 1  and diff_row == 2 or
		   diff_col == 2  and diff_row == -1 or
		   diff_col == 1  and diff_row == -2 or
		   diff_col == -2 and diff_row == 1 or
		   diff_col == -1 and diff_row == 2 or
		   diff_col == -2 and diff_row == -1 or
		   diff_col == -1 and diff_row == -2);
}

bool pawn::legal_move(int to) const
{
	const int diff = to - pos;

	return to >= 0 and to < 64 and (is_white ? (
		   (pos % 8 == 1 and diff == 2) or
		   (diff == 1 or diff - 8 == 1 or diff + 8 == 1)
			)
		   : (
		   (pos % 8 == 6 and diff == 2) or
		   (diff == -1 or diff - 8 == -1 or diff + 8 == -1)
		   ));
}
}
