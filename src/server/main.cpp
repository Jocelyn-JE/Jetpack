/*
** EPITECH PROJECT, 2025
** bsjetpack
** File description:
** main
*/

#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

#include "logic/Game.hpp"
#include "../../../include/GameData.hpp"
#include "./parsing/Parser.hpp"
#include "Server.hpp"
#include "logic/Game.hpp"
#include "logic/GameServer.hpp"

void gameLoop(std::shared_ptr<GameData> gameData) {
    Game game(gameData);
    game.start("map.joy");
}

void networkLoop(std::shared_ptr<GameData> gameData) {
    GameServer server(gameData->port, gameData);
    server.start();
}

int main(int argc, char **argv) {
    auto gameData = std::make_shared<GameData>();
    jetpack::Parser parser(argc, argv);

    try {
        parser.parseServerFlags(*gameData);
    } catch (const jetpack::Parser::ParsingError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser.getServerUsage() << std::endl;
        return 84;
    }

    if (std::filesystem::exists(gameData->filename)) {
        std::thread gameThread(gameLoop, gameData);
        std::thread networkThread(networkLoop, gameData);

        gameThread.join();
        networkThread.join();
    } else {
        std::cerr << "Error: " << gameData->filename << " not found"
                  << std::endl;
        return 1;
    }
    return jetpack::server::Server::runServer(atoi(argv[1]));
}
