#pragma once

#include "handler.hpp"

namespace networking {

class slave : public handler
{
public:
	slave(const std::string &code, uint32_t initial_board_hash);
	~slave();

private:
	tcp::endpoint server;
};

}
