/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <iostream>
#include <string>

#include "Parser.hpp"
#include "Program.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
    try {
        Jetpack::Client::Parser parser(argc, argv);
        Jetpack::Logger logger(parser.isDebugMode());
        logger.log("Debug mode is " + std::to_string(parser.isDebugMode()));
        logger.log("IP: " + parser.getIp());
        logger.log("Port: " + std::to_string(parser.getPort()));
        Jetpack::Client::Program app(parser.getIp().c_str(), parser.getPort(), logger);
        app.loop();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
