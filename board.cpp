#include "board.hpp"

namespace chess {
board::board()
: cur_player(WHITE), pieces()
{
	pieces[WHITE][A1] = piece::rook;
	pieces[WHITE][B1] = piece::knight;
	pieces[WHITE][C1] = piece::bishop;
	pieces[WHITE][D1] = piece::queen;
	pieces[WHITE][E1] = piece::king;
	pieces[WHITE][F1] = piece::bishop;
	pieces[WHITE][G1] = piece::knight;
	pieces[WHITE][H1] = piece::rook;

	for (int i = A2; i <= H2; i += 8)
		pieces[WHITE][i] = piece::pawn;

	pieces[BLACK][A8] = piece::rook;
	pieces[BLACK][B8] = piece::knight;
	pieces[BLACK][C8] = piece::bishop;
	pieces[BLACK][D8] = piece::queen;
	pieces[BLACK][E8] = piece::king;
	pieces[BLACK][F8] = piece::bishop;
	pieces[BLACK][G8] = piece::knight;
	pieces[BLACK][H8] = piece::rook;

	for (int i = A7; i <= H7; i += 8)
		pieces[BLACK][i] = piece::pawn;

}

std::ostream &operator<<(std::ostream &os, const board &b)
{
	for(int j = 0; j < 8; ++j)
	{
		for(int i = 0; i < 64; i+=8)
		{
			int pos = i + 7 - j;
			os << b[pos] << ' ';
		}
		os << "| " << 8 - j << std::endl;
	}
	os << "----------------|--\n"
		  "A B C D E F G H |  \n";

	return os;
}

char board::operator[](int pos) const
{
	if (pieces[WHITE][pos] != piece::empty)
	{
		switch(pieces[WHITE][pos])
		{
		case piece::empty: return EMPTY_SQUARE;
		case piece::king: return 'K';
		case piece::queen: return 'Q';
		case piece::rook: return 'R';
		case piece::bishop: return 'B';
		case piece::knight: return 'N';
		case piece::pawn: return 'P';
		default: throw std::invalid_argument("Invalid piece");
		}
	}
	else
	{
		switch(pieces[BLACK][pos])
		{
		case piece::empty: return EMPTY_SQUARE;
		case piece::king: return 'k';
		case piece::queen: return 'q';
		case piece::rook: return 'r';
		case piece::bishop: return 'b';
		case piece::knight: return 'n';
		case piece::pawn: return 'p';
		default: throw std::invalid_argument("Invalid piece");
		}
	}
}

bool board::bishop_legal_move(int pos, int to) const
{
	const int diff_col = to / 8 - pos / 8;
	const int diff_row = to % 8 - pos % 8;
	if (to < 0 or to >= 64 or !is(pos,cur_player) or is(to,cur_player))
		return false;

	if (diff_col==0 and abs(diff_col) != abs(diff_row))
		return false;

	if (diff_col > 0 and diff_row > 0)
	{
		for (int i = 1; i < diff_col; i++) // while moving columns to 'to'
		{
			if ((*this)[pos + i * 8 + i] != EMPTY_SQUARE)
				return false;
		}
	}
	if (diff_col > 0 and diff_row < 0)
	{
		for (int i = 1; i < diff_col; i++) // while moving columns to 'to'
		{
			if ((*this)[pos + i * 8 - i] != EMPTY_SQUARE)
				return false;
		}
	}
	if (diff_col < 0 and diff_row > 0)
	{
		for (int i = -1; i > diff_row; i--) // while moving rows to 'to'
		{
			if ((*this)[pos + i * 8 - i] != EMPTY_SQUARE)
				return false;
		}
	}
	if (diff_col < 0 and diff_row < 0)
	{
		for (int i = -1; i > diff_col; i--) // while moving columns to 'to'
		{
			if ((*this)[pos + i * 8 + i] != EMPTY_SQUARE)
				return false;
		}
	}

	return is_valid(pos, to);
}

bool board::rook_legal_move(int pos, int to) const
{
	const int diff_col = to / 8 - pos / 8;
	const int diff_row = to % 8 - pos % 8;

	if ((bool)diff_col == (bool)diff_row) // make sure rook is moving in a straight line
		return false;

	if (diff_col > 0)
	{
		for (int i = 1; i < diff_col; i++) // while moving columns to 'to'
		{
			if ((*this)[pos + i*8] != EMPTY_SQUARE)
				return false;
		}
	}
	if (diff_col < 0)
	{
		for (int i = -1; i > diff_col; i--) // while moving columns to 'to'
		{
			if ((*this)[pos + i*8] != EMPTY_SQUARE)
				return false;
		}
	}
	if (diff_row > 0)
	{
		for (int i = 1; i < diff_row; i++) // while moving rows to 'to'
		{
			if ((*this)[pos + i] != EMPTY_SQUARE)
				return false;
		}
	}
	if (diff_row < 0)
	{
		for (int i= -1 ; i > diff_col; i--) // while moving columns to 'to'
		{
			if ((*this)[pos + i] != EMPTY_SQUARE)
				return false;
		}
	}

	return is_valid(pos, to);
}

bool board::queen_legal_move(int pos, int to) const
{
	return rook_legal_move(pos, to) or bishop_legal_move(pos, to);
}

bool board::knight_legal_move(int pos, int to) const
{
	const int diff_col = to / 8 - pos / 8;
	const int diff_row = to % 8 - pos % 8;
	return is_valid(pos, to) and
		   (diff_col == 2  and diff_row == 1  or
		    diff_col == 1  and diff_row == 2  or
			diff_col == 2  and diff_row == -1 or
			diff_col == 1  and diff_row == -2 or
			diff_col == -2 and diff_row == 1  or
			diff_col == -1 and diff_row == 2  or
			diff_col == -2 and diff_row == -1 or
			diff_col == -1 and diff_row == -2   );
}

bool board::king_legal_move(int pos, int to) const
{
	const int diff_col = to / 8 - pos / 8;
	const int diff_row = to % 8 - pos % 8;
	return is_valid(pos, to) and abs(diff_col) == 1 or abs(diff_row) == 1;
}

bool board::pawn_legal_move(int pos, int to) const
{
	const int diff_col = to / 8 - pos / 8;
	const int diff_row = to % 8 - pos % 8;
	bool move = !is(to, !cur_player) and diff_col == 0;
	bool capture;
	if (cur_player == WHITE)
	{
		const bool advance = !is(pos+1, WHITE) and !is(pos+1, BLACK);
		const bool one_square = diff_row == 1;
		const bool two_square = pos % 8==1 and diff_row==2;
		move = move and ( (advance and two_square) or one_square );
		capture = diff_row == 1 and abs(diff_col) == 1 and is(to,!cur_player);
	}
	else
	{
		const bool advance = !is(pos-1, WHITE) and !is(pos-1, BLACK);
		const bool one_square = diff_row == -1;
		const bool two_square = pos % 8==6 and diff_row==-2;
		move = move and ( (advance and two_square) or one_square );
		capture = diff_row==-1 and abs(diff_col)==1 and is(to,!cur_player);
	}

	return is_valid(pos, to) and (move or capture);
}

}