#pragma once
#include <array>
#include <iostream>

namespace chess {
class board
{
public:
	enum class piece
	{
		empty = 0, king, queen, rook, bishop, knight, pawn
	};
	using move_t = std::pair<int, int>;
	using board_t = std::array<piece, 64>;

	board();
	friend std::ostream &operator<<(std::ostream &os, const board &b);

	/**
	 * @brief return the character for a piece
	 * @param pos
	 * @return
	 */
	char operator[] (int pos) const;

	uint32_t operator() () const;

	bool move(int from, int to);

	bool is_check(bool king_color) const;

	inline bool turn() const { return cur_player; }

	/**
	 * @brief Get the numerical position given a string representation: i.e. "a1"
	 * -> 0, "h8" -> 63
	 * @param str The string representation of the position
	 * @return The numerical position
	 * @throws std::invalid_argument if the string is not a valid position
	 */
	static int get_pos(const std::string &str);

	/**
	 * @brief Get the human-readable string representation of a position: i.e. 0
	 * -> "a1", 63 -> "h8"
	 * @param pos The numerical position
	 * @return The string representation of the position
	 * @throws std::invalid_argument if the position is not valid
	 */
	static std::string get_str(int pos);

private:
	mutable board_t pieces[2];
	mutable bool cur_player;
	bool white_short, white_long, black_short, black_long;
	int en_passant_square;

	inline bool is(int pos, bool color) const
	{ return pieces[color][pos] != piece::empty; }

	/**
	 * @brief Checks for the following conditions:
	 * 	- The position the piece is moving to is a valid location on the board
	 * 	- The piece being moved belongs to the current player
	 * 	- There is no piece belonging to the current player at the position
	 * 	the piece is moving to
	 * @param pos The position the piece is located at currently
	 * @param to The position the piece is moving to
	 * @return true if the move is valid, false otherwise
	 */
	inline bool is_valid(int pos, int to) const
	{ return to >= 0 and to < 64 and
		is(pos,cur_player) and !is(to,cur_player); }

	bool bishop_legal_move(int pos, int to) const;
	bool rook_legal_move(int pos, int to) const;
	bool queen_legal_move(int pos, int to) const;
	bool knight_legal_move(int pos, int to) const;
	bool king_legal_move(int pos, int to) const;
	int pawn_legal_move(int pos, int to) const; // return -1 if en passant is
	// not possible, and return enpassant square if it is possible.
	bool is_legal(int from, int to) const;
	void update_castle_rights(int from);
	bool castle(int from, int to);

	static uint32_t seeded(piece p);

private:
	static constexpr char EMPTY_SQUARE = '.';
	/**
	 * Constants for colors
	 */
	static constexpr int WHITE = 0, BLACK = 1;
	/**
	 * Constants for pawn moves
	 */
	static constexpr int ILLEGAL_MOVE = 0, CAPTURE = -1, ADVANCE_1 = -2,
	EN_PASSANT = -3, PROMOTION = -4;
	static constexpr int QUEEN_PROMOTION = -100;
	static constexpr int ROOK_PROMOTION = -101;
	static constexpr int BISHOP_PROMOTION = -102;
	static constexpr int KNIGHT_PROMOTION = -103;

	static constexpr uint32_t SEED = 861317959;

	static constexpr uint32_t PRIMES[64] = {
		2,3,5,7,11,13,17,19,
		23,29,31,37,41,43,47,53,
		59,61,67,71,73,79,83,89,
		97,101,103,107,109,113,127,131,
		137,139,149,151,157,163,167,173,
		179,181,191,193,197,199,211,223,
		227,229,233,239,241,251,257,263,
		269,271,277,281,283,293,307,311
	};
public:
	/**
	 * Constants for squares
	 */
	static constexpr int A1 = 0, A2 = 1, A3 = 2, A4 = 3, A5 = 4, A6 = 5, A7 = 6, A8 = 7;
	static constexpr int B1 = 8, B2 = 9, B3 = 10, B4 = 11, B5 = 12, B6 = 13, B7 = 14, B8 = 15;
	static constexpr int C1 = 16, C2 = 17, C3 = 18, C4 = 19, C5 = 20, C6 = 21, C7 = 22, C8 = 23;
	static constexpr int D1 = 24, D2 = 25, D3 = 26, D4 = 27, D5 = 28, D6 = 29, D7 = 30, D8 = 31;
	static constexpr int E1 = 32, E2 = 33, E3 = 34, E4 = 35, E5 = 36, E6 = 37, E7 = 38, E8 = 39;
	static constexpr int F1 = 40, F2 = 41, F3 = 42, F4 = 43, F5 = 44, F6 = 45, F7 = 46, F8 = 47;
	static constexpr int G1 = 48, G2 = 49, G3 = 50, G4 = 51, G5 = 52, G6 = 53, G7 = 54, G8 = 55;
	static constexpr int H1 = 56, H2 = 57, H3 = 58, H4 = 59, H5 = 60, H6 = 61, H7 = 62, H8 = 63;
};
}


