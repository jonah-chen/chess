
#pragma once

#include <string>
#include <stdexcept>

namespace chess {

enum class piece_ty : signed char
{
	empty = 0, king, queen, rook, bishop, knight, pawn
};

/**
 * @brief Get the numerical position given a string representation: i.e. "a1"
 * -> 0, "h8" -> 63
 * @param str The string representation of the position
 * @return The numerical position
 * @throws std::invalid_argument if the string is not a valid position
 */
int get_pos(const std::string &str);

/**
 * @brief Get the human-readable string representation of a position: i.e. 0
 * -> "a1", 63 -> "h8"
 * @param pos The numerical position
 * @return The string representation of the position
 * @throws std::invalid_argument if the position is not valid
 */
std::string get_str(int pos);


/**
 * @brief Abstract class for chess pieces. Classes that inherit from this class
 * must implement the following functions:
 * 	- legal_move(int)
 * @note a1 = 0, a2 = 1, ..., h8 = 63
 */
class piece
{
public:
	int pos;

	virtual bool legal_move(int to) const = 0;

protected:
	bool is_white;
	piece_ty type;

	piece(bool is_white, piece_ty type, int pos)
			: is_white(is_white), type(type), pos(pos)
	{}
};

class king : public piece
{
public:
	king(bool is_white, int pos) : piece(is_white, piece_ty::king, pos)
	{}

	bool legal_move(int to) const override;
};

class queen : public piece
{
public:
	queen(bool is_white, int pos) : piece(is_white, piece_ty::queen, pos)
	{}

	bool legal_move(int to) const override;
};

class rook : public piece
{
public:
	rook(bool is_white, int pos) : piece(is_white, piece_ty::rook, pos)
	{}

	bool legal_move(int to) const override;
};

class bishop : public piece
{
public:
	bishop(bool is_white, int pos) : piece(is_white, piece_ty::bishop, pos)
	{}

	bool legal_move(int to) const override;
};

class knight : public piece
{
public:
	knight(bool is_white, int pos) : piece(is_white, piece_ty::knight, pos)
	{}

	bool legal_move(int to) const override;
};

class pawn : public piece
{
public:
	pawn(bool is_white, int pos) : piece(is_white, piece_ty::pawn, pos)
	{}

	bool legal_move(int to) const override;
};

}
