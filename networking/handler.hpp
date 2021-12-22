#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

namespace networking {

enum class header : uint8_t
{
	connection_request, board_hash, move, game_over
};

class handler
{
public:
	handler(const std::string &code);

	virtual ~handler() = default;

	void log_error(std::ostream &err_stream = std::cerr) const;

	static asio::const_buffer to_buffer(header h, const std::string &data4);
	static asio::const_buffer to_buffer(header h, uint16_t code);
	static asio::const_buffer to_buffer(header h, uint32_t hash);

protected:
	using tcp = asio::ip::tcp;

	asio::ip::address server_ip;
	uint16_t server_port;

	asio::io_context io_context;
	tcp::socket socket;
	tcp::endpoint endpoint;

	asio::error_code error_code;
};

namespace codes {
	/**
	 * @brief Convert an IPv4 string seperated by dots to an unsigned integer.
	 */
	uint32_t ip_to_int(const std::string &);

	std::string int_to_ip(uint32_t);

	std::string int_to_code(uint64_t);

	std::string encode(uint32_t, uint16_t, uint16_t);

	uint64_t code_to_int(const std::string &);

	asio::ip::address decode_ip(const std::string &);

	uint16_t decode_port(const std::string &);

	uint16_t decode_uid(const std::string &);

}

}