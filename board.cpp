#include "board.hpp"

namespace chess {
board::board()
: pieces(), cur_player(WHITE), white_long(true), white_short(true),
black_long(true), black_short(true), en_passant_square(-1)
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

	if (diff_col==0 or abs(diff_col) != abs(diff_row))
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
		for (int i = -1; i > diff_col; i--) // while moving rows to 'to'
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
		for (int i= -1 ; i > diff_row; i--) // while moving columns to 'to'
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
	return is_valid(pos, to) and (abs(diff_col) <= 1 and abs(diff_row) <= 1)
	and (abs(diff_col));
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

bool board::is_legal(int from, int to) const
{
	auto selected_piece = pieces[cur_player][from];

	switch (selected_piece)
	{
	case piece::pawn:
		if (!pawn_legal_move(from, to))
			return false;
		break;
	case piece::knight:
		if (!knight_legal_move(from, to))
			return false;
		break;
	case piece::bishop:
		if (!bishop_legal_move(from, to))
			return false;
		break;
	case piece::rook:
		if (!rook_legal_move(from, to))
			return false;
		break;
	case piece::queen:
		if (!queen_legal_move(from, to))
			return false;
		break;
	case piece::king:
		if(!king_legal_move(from, to))
			return false;
		break;
	default:
		return false;
	}

	return true;
}

bool board::move(int from, int to)
{
	std::cout << (cur_player == WHITE ? "white" : "black") << " tried to make"
															  " a move";
	if ((from == E1 or from == E8) and castle(from, to))
		return true;
	if (!is_legal(from, to))
		return false;

	piece opp_to = pieces[!cur_player][to];
	piece cp_to = pieces[cur_player][to];
	pieces[!cur_player][to] = piece::empty;
	pieces[cur_player][to] = pieces[cur_player][from];
	pieces[cur_player][from] = piece::empty;

	if (is_check(cur_player))
	{
		pieces[cur_player][from] = pieces[cur_player][to];
		pieces[!cur_player][to] = opp_to;
		pieces[cur_player][to] = cp_to;
		std::cout << "you are in check\n";
		return false;
	}

	cur_player = !cur_player;

	update_castle_rights(from);

	return true;
}

bool board::is_check(bool king_color) const
{
	// find where the king is
	int king_pos {-1};
	bool _cur_player = cur_player;
	cur_player = !king_color; // temporarily assign the player to the current
	// player

	for (int i=0; i<64 and king_pos<0; i++)
		if (pieces[king_color][i]==piece::king)
			king_pos = i;

	for (int i=0; i<64; i++)
	{
		if (is_legal(i, king_pos))
		{
			cur_player = _cur_player;
			return true;
		}
	}
	cur_player = _cur_player;
	return false;
}

bool board::castle(int from, int to)
{
	if (!is_valid(from, to) or is_check(cur_player))
		return false;

	int king_from, king_to;
	int rook_from, rook_to;

	if (from == E1 and cur_player == WHITE)
	{
		king_from = E1;
		if (to==G1 and white_short)
		{
			king_to = G1;
			rook_from = H1;
			rook_to = F1;
		}
		else if(to==C1 and white_long)
		{
			king_to = C1;
			rook_from = A1;
			rook_to = D1;
		}
        else
            return false;
	}
	if (from == E8 and cur_player == BLACK)
	{
		king_from = E8;
		if (to==G8 and black_short)
		{
			king_to = G8;
			rook_from = H8;
			rook_to = F8;
		}
        else if(to==C8 and black_long)
        {
            king_to = C8;
            rook_from = A8;
            rook_to = D8;
        }
        else
            return false;
	}
    if (pieces[WHITE][rook_to] != piece::empty or
        pieces[BLACK][rook_to] != piece::empty)
        return false;

    pieces[cur_player][king_from] = piece::empty;
    pieces[cur_player][rook_to] = piece::king;

    if (is_check(cur_player))
    {
        pieces[cur_player][king_from] = piece::king;
        pieces[cur_player][rook_to] = piece::empty;
        return false;
    }
    
    pieces[cur_player][rook_to] = piece::rook;
    pieces[cur_player][king_to] = piece::king;
    pieces[cur_player][rook_from] = piece::empty;
    return true;
}

void board::update_castle_rights(int from)
{
	switch(from)
	{
	case E1:
		white_short = false;
		white_long = false;
		break;
	case E8:
		black_short = false;
		black_long = false;
		break;
	case A1:
		white_long = false;
		break;
	case A8:
		black_long = false;
		break;
	case H1:
		white_long = false;
		break;
	case H8:
		white_short = false;
		break;
	default:
		break;
	}
}

}