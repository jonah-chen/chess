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

/**
 * @brief The header for any message sent over the network.
 */
enum class header : uint8_t
{
	connection_request, board_hash, move, game_over, reject, disconnect
};

/**
 * @brief The handler class is the base class that all network handlers should
 * inherit from. It provides the basic functionality for sending and receiving
 * messages using TCP.
 *
 * @warning This class should not be instantiated directly as the constructor
 * does not establish the connection.
 */
class handler
{
protected:
	static constexpr std::size_t MSG_SIZE = 5; // The size of each message

public:
	virtual ~handler() = default;

	/**
	 * @brief The listener thread listens to incoming messages. It should be run
	 * on a separate thread as it is blocking. It will not return until the
	 * handler is disconnected.
	 *
	 * Once the listener receives a message, it will set the data_ready flag to
	 * true, and it will stop receiving messages until the flag is set to
	 * false by reading the data.
	 *
	 */
	void listener();

	/**
	 * @brief Sends a message to the server.
	 * @param msg const reference to asio::const_buffer of the message to
	 * send. These messages can be created using the to_buffer functions
	 */
	void send(const asio::const_buffer &msg);

	/**
	 * @brief Attempt to read the message received from the server, then if
	 * successful, set the data_ready flag to false to allow the listener to read
	 * more data.
	 * @param head the header of the message read. Not modified if no message is
	 * available.
	 * @param body the body of the message read. Not modified if no message is
	 * available.
	 * @return true if the message was read successfully, false otherwise.
	 *
	 */
	bool read(header &head, std::string &body);

	/**
	 * @brief Disconnects the handler from the server. This will stop the
	 * listener thread if it is running.
	 */
	inline void disconnect() { connected = false; }

	/**
	 * @brief Log the error message to the console, if it exists.
	 * @param err_stream the output stream to log to.
	 */
	void log_error(std::ostream &err_stream = std::cerr) const;

	/**
	 * @brief Package a 4-byte string into a buffer to be sent over the network.
	 * @param h The header describing the type of message.
	 * @param data4 4-byte string to be sent.
	 * @return The header and data packaged into an asio::const_buffer which is
	 * ready to be sent over the network.
	 */
	static asio::const_buffer to_buffer(header h, const std::string &data4);

	/**
	 * @brief Package a 2-byte game code into a buffer to be sent over the network.
	 * @param h The header describing the type of message. Must be connection_request.
	 * @param code 2-byte game code to be sent.
	 * @return The header and data packaged into an asio::const_buffer which is
	 * ready to be sent over the network. Note that the last two bytes of the
	 * message are irrelevant here.
	 */
	static asio::const_buffer to_buffer(header h, uint16_t code);

	/**
	 * @brief Package a 4-byte board hash into a buffer to be sent over the network.
	 * @param h The header describing the type of message. Must be board_hash.
	 * @param hash 4-byte board hash to be sent.
	 * @return The header and data packaged into an asio::const_buffer which is
	 * ready to be sent over the network.
	 */
	static asio::const_buffer to_buffer(header h, uint32_t hash);

protected:
	using tcp = asio::ip::tcp;

	asio::ip::address server_ip;// The IP address of the server
	uint16_t server_port;		// The port of the server

	asio::io_context io_context;// The context for the I/O service
	tcp::socket socket;			// The socket for the connection

	asio::error_code error_code;// The error code for the last operation

	bool data_ready, connected; // Flags for the listener thread
	std::string data;			// The data received from the server

protected:
	/**
	 * @brief Handler constructor obtains the information about the server
	 * using the game code. It then initializes the what is required to establish
	 * the connection.
	 *
	 * @param code The game code of the server.
	 */
	handler(const std::string &code);

};

namespace codes {
	/**
	 * @brief Convert an IPv4 string seperated by dots to an unsigned integer.
	 * @param ip_str The IPv4 string to convert.
	 * @return The unsigned integer representation of the IPv4 string.
	 */
	uint32_t ip_to_int(const std::string &ip_str);

	/**
	 * @brief Converts an unsigned integer to an IPv4 string seperated by dots.
	 * @param ip_int The unsigned integer to convert.
	 * @return The IPv4 string representation.
	 */
	std::string int_to_ip(uint32_t ip_int);

	/**
	 * @brief Convert an unsigned 64-bit integer to a string representation
	 * using the characters A-Z, a-z, 0-9, #, and *.
	 * @return The string encoding of the unsigned 64-bit integer.
	 */
	std::string int_to_code(uint64_t);

	/**
	 * @brief Encode an ip address and port number alongside a unique game code
	 * into a string using the characters A-Z, a-z, 0-9, #, and *.
	 * @param ip_int 32-bit integer representation of the ip address.
	 * @param port The port number to encode.
	 * @param uid The unique ID for the game to encode.
	 * @return The string encoding of the ip address, port number, and unique
	 * ID which serves as the game code.
	 */
	std::string encode(uint32_t ip_int, uint16_t port, uint16_t uid);

	/**
	 * @brief Decode a string encoding back into the 64-bit integer.
	 * @param code The string encoding to decode.
	 * @return Decoded 64-bit integer.
	 */
	uint64_t code_to_int(const std::string &code);

	/**
	 * @brief Obtain the ip address from the game code.
	 * @param code The game code.
	 * @return The ip address as an asio::ip::address object.
	 */
	asio::ip::address decode_ip(const std::string &code);

	/**
	 * @brief Obtain the port number from the game code.
	 * @param code The game code.
	 * @return The port number as an unsigned 16-bit integer.
	 */
	uint16_t decode_port(const std::string &code);

	/**
	 * @brief Obtain the unique ID from the game code.
	 * @param code The game code.
	 * @return The unique ID as an unsigned 16-bit integer.
	 */
	uint16_t decode_uid(const std::string &code);

}

}