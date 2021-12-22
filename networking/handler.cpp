#include "handler.hpp"

namespace networking {

handler::handler(const std::string &code)
		: server_ip(codes::decode_ip(code)),
		  server_port(codes::decode_port(code)),
		  io_context(),
		  socket(io_context),
		  endpoint(server_ip, server_port)
{}

void handler::log_error (std::ostream &err_stream) const
{
	if (error_code)
		err_stream << error_code.message() << std::endl;
}

asio::const_buffer handler::to_buffer(header h, const std::string &data4)
{
	if (data4.size() != 4)
		throw std::invalid_argument("data4 must be 4 bytes long");
	std::stringstream ss;
	ss << static_cast<char>(h) << data4;
	return asio::buffer(ss.str(), 5);
}

asio::const_buffer handler::to_buffer(header h, uint16_t code)
{
	if (h != header::connection_request)
		throw std::invalid_argument("Header must be connection_request to "
									"send a code. To send a move, use a "
									"string instead like \"d2d4\"");
	std::stringstream ss;
	ss << static_cast<char>(h) << static_cast<char>(code >> 8)
	   << static_cast<char>(code & 0xFF) << '\0' << '\0';
	return asio::buffer(ss.str(), 5);
}

asio::const_buffer handler::to_buffer(header h, uint32_t hash)
{
	if (h != header::board_hash)
		throw std::invalid_argument("Header must be board_hash to send a "
									"hash. To send a move, use a string "
									"instead like \"d2d4\"");
	std::stringstream ss;
	ss << static_cast<char>(h) << static_cast<char>(hash >> 24)
	   << static_cast<char>(hash >> 16) << static_cast<char>(hash >> 8)
	   << static_cast<char>(hash & 0xFF);
	return asio::buffer(ss.str(), 5);
}

namespace codes {
/**
 * @brief Convert an IPv4 string seperated by dots to an unsigned integer.
 */
uint32_t ip_to_int(const std::string &ip)
{
	uint32_t result = 0;
	std::stringstream ss(ip);
	std::string segment;
	while (std::getline(ss, segment, '.'))
	{
		result = (result << 8) + std::stoi(segment);
	}
	return result;
}

/**
 * @brief Convert an IPv4 unsigned integer to a string seperated by dots.
 */
std::string int_to_ip(uint32_t ip)
{
	std::string result;
	for (int i = 0; i < 4; i++)
		result += std::to_string((ip >> (i * 8)) & 0xff) + ".";
	result.pop_back();
	return result;
}

/**
 * @brief Convert the last 48 bits of an unsigned long long to a code using the
 * letters A-Z, a-z, 0-9, *, and #.
 */
std::string int_to_code(uint64_t code)
{
	std::string result;
	for (int i = 0; i < 8; i++)
	{
		uint8_t c = (code >> (i * 6)) & 0xff;
		if (c < 26)
			result += 'A' + c;
		else if (c < 52)
			result += 'a' + c - 26;
		else if (c < 62)
			result += '0' + c - 52;
		else if (c == 62)
			result += '*';
		else if (c == 63)
			result += '#';
	}
	return result;
}

std::string encode(uint32_t ip, uint16_t port, uint16_t uid)
{
	uint64_t _ip = ip, _port = port, _uid = uid;
	return int_to_code(_ip + (_port << 32) + (_uid << 48));
}

/**
 * @brief Convert a code with the letters A-Z, a-z, 0-9, *, and # to a 48 bit
 * integer.
 */
uint64_t code_to_int(const std::string &code)
{
	uint64_t result = 0;
	for (int i = 0; i < 8; i++)
	{
		uint8_t c = code[i];
		if (c >= 'A' && c <= 'Z')
			result = (result << 6) + (c - 'A');
		else if (c >= 'a' && c <= 'z')
			result = (result << 6) + (c - 'a') + 26;
		else if (c >= '0' && c <= '9')
			result = (result << 6) + (c - '0') + 52;
		else if (c == '*')
			result = (result << 6) + 62;
		else if (c == '#')
			result = (result << 6) + 63;
	}
	return result;
}

asio::ip::address decode_ip(const std::string &code)
{
	return asio::ip::make_address(int_to_ip(code_to_int(code)));
}

uint16_t decode_port(const std::string &code)
{
	return code_to_int(code)>>32;
}

uint16_t decode_uid(const std::string &code)
{
	return code_to_int(code)>>48;
}

}

}