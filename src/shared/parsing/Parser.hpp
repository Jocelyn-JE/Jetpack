/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Parser
*/

#ifndef SRC_SHARED_PARSING_PARSER_HPP_
#define SRC_SHARED_PARSING_PARSER_HPP_

#include <exception>
#include <string>
#include <vector>
#include <memory>


#include "../../../include/Coin.hpp"
#include "../../../include/Obstacle.hpp"
#include "../../../include/GameData.hpp"

namespace jetpack {
class Parser {
 public:
    Parser() = delete;
    Parser(int argc, char **argv);
    ~Parser();
    class ParsingError : public std::exception {
     public:
        explicit ParsingError(std::string message);
        ~ParsingError();
        const char *what() const noexcept override;

     private:
        std::string _message;
    };
    void parseServerArgs();
    void parseClientArgs();
    std::string getServerUsage();
    std::string getClientUsage();
    void parseServerFlags(GameData& data);

 private:
    std::vector<std::string> _args;
    int _argc;
    size_t findFlagIndex(const std::string& flag) const;
};

class MapParser {
   public:
      static bool parseMap(const std::string& filename,
                           std::vector<std::shared_ptr<coinsPos_t>>& coins,
                           std::vector<std::shared_ptr<obstacle_t>>& obstacles);
  
   private:
      static void processLine(
          const std::string& line, int y,
          std::vector<std::shared_ptr<coinsPos_t>>& coins,
          std::vector<std::shared_ptr<obstacle_t>>& obstacles);
  };

}  // namespace jetpack

#endif  // SRC_SHARED_PARSING_PARSER_HPP_
