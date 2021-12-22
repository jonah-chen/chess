#include "slave.hpp"

namespace networking {

slave::slave(const std::string &code, uint32_t initial_board_hash)
	: handler(code), server(server_ip, server_port)
{
	socket.connect(server, error_code);
	log_error();

	uint16_t uid = codes::decode_uid(code);
	asio::write(socket, to_buffer(header::connection_request, uid), error_code);
	log_error();

	char buffer[MSG_SIZE];
	asio::read(socket, asio::buffer(buffer, MSG_SIZE), error_code);
	log_error();

	uint32_t server_hash = static_cast<uint32_t>(buffer[0]) << 24 |
						   static_cast<uint32_t>(buffer[1]) << 16 |
						   static_cast<uint32_t>(buffer[2]) << 8 |
						   static_cast<uint32_t>(buffer[3]);
	if (static_cast<header>(buffer[0]) == header::board_hash and
		server_hash == initial_board_hash)
		connected = true;
	else
	{

		asio::write(socket, to_buffer(header::disconnect, uid), error_code);
		socket.close();
		throw std::runtime_error("Your board is not the same as the server's "
								 "board. "
								 + std::to_string(initial_board_hash) + "!="
								 + std::to_string(server_hash));
	}
}




}
