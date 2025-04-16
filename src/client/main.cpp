/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <iostream>
#include <string>

#include "Exception.hpp"
#include "Logger.hpp"
#include "Parser/ClientParser.hpp"
#include "ProgramGraphic.hpp"

int main(int argc, char **argv) {
    try {
        jetpack::Client::Parser parser(argc, argv);
        jetpack::Logger logger(parser.isDebugMode());
        logger.log("Debug mode is " + std::to_string(parser.isDebugMode()));
        logger.log("IP: " + parser.getIp());
        logger.log("Port: " + std::to_string(parser.getPort()));
        jetpack::Client::Program app(parser.getIp().c_str(), parser.getPort(),
                                     logger);
        app.loop();
    } catch (HelpException &e) {
        std::cout << "HELP" << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
