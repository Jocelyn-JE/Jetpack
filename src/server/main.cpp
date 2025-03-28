/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <iostream>
#include "Server.hpp"
#include "Parser.hpp"

int main(int argc, char **argv) {
    Jetpack::Parser parser(argc, argv);

    try {
        parser.parseServerArgs();
    } catch(const Jetpack::Parser::ParsingError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser.getServerUsage() << std::endl;
        return 84;
    }
    return Jetpack::server::Server::runServer(atoi(argv[1]));
}
