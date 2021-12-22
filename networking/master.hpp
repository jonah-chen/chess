#pragma once

#include "handler.hpp"

#include <iostream>

namespace networking {

class master : public handler
{
public:
	master(const std::string &code, uint32_t initial_board_hash);
	~master();

private:
	tcp::endpoint reciever;
	tcp::acceptor acceptor;

	bool validate_connection(const char *msg, uint16_t uid);
};

}