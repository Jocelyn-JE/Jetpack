/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <string>
#include <iostream>
#include "./client/src/parsing/Parser.hpp"
// #include "./client/src/client/Client.hpp"

int main(int argc, char **argv) {
    jetpack::Parser parser(argc, argv);

    try {
        parser.parseArgs();
    } catch(const jetpack::Parser::ParsingError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser.getUsage() << std::endl;
        return 84;
    }
    // return jetpack::client::Client::runGameUI(argv[1], atoi(argv[2]));
    return 0;
}
