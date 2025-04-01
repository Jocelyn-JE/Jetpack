/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <iostream>

#include "./parsing/Parser.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
    jetpack::Parser parser(argc, argv);

    try {
        parser.parseServerArgs();
    } catch (const jetpack::Parser::ParsingError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser.getServerUsage() << std::endl;
        return 84;
    }
    return jetpack::server::Server::runServer(atoi(argv[1]));
}
