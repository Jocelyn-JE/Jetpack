/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <iostream>
#include "./server/src/server/Server.hpp"
#include "./server/src/parsing/Parser.hpp"

int main(int argc, char **argv) {
    jetpack::Parser parser(argc, argv);

    try {
        parser.parseArgs();
    } catch(const jetpack::Parser::ParsingError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser.getUsage() << std::endl;
        return 84;
    }
    return jetpack::server::Server::runServer(atoi(argv[1]));
}
