#include "master.hpp"

namespace networking {

master::master(const std::string &code, uint32_t initial_board_hash)
		: handler(code),
		  reciever(tcp::v4(), codes::decode_port(code)),
		  acceptor(io_context, reciever)
{
	uint16_t uid = codes::decode_uid(code);

	char connection_req[MSG_SIZE];
	while (!validate_connection(connection_req, uid))
	{
		acceptor.accept(socket, error_code);
		log_error();
		asio::read(socket, asio::buffer(connection_req, MSG_SIZE), error_code);
		log_error();

		asio::write(socket, to_buffer(header::board_hash, initial_board_hash), error_code);
	}

	std::cout << "Connection established" << std::endl;
	connected = true;
}

master::~master()
{

}


bool master::validate_connection(const char *msg, uint16_t uid)
{
	return static_cast<header>(*msg) == header::connection_request and
		   uid ==
		   (static_cast<uint16_t>(msg[1]) << 8 | static_cast<uint16_t>(msg[2]));
}

}