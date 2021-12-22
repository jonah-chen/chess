#include "handler.hpp"
#include <random>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " [dot-seperated IPv4] [port]" << std::endl;
		return 0;
	}

	char *ip_str = argv[1];
	char *port_str = argv[2];

	uint32_t ip_code = networking::codes::ip_to_int(ip_str);
	uint16_t port = atoi(port_str);
	// generate a random 16 bit code
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 65535);
	uint16_t code = dis(gen);

	std::string game_code = networking::codes::encode(ip_code, port, code);

	std::cout << "Your unique ID is: " << code << std::endl;
	std::cout << "Your game code is: " << game_code << std::endl;
	std::cout << "Your IP would be: " << networking::codes::decode_ip(game_code) << std::endl;
	std::cout << "Your port would be: " << networking::codes::decode_port(game_code) << std::endl;
	std::cout << "Your code would be: " << networking::codes::decode_uid(game_code) << std::endl;
	std::cout << "Make sure these match!" << std::endl;

	return 0;
}