/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Parser
*/

#ifndef CLIENT_SRC_PARSING_PARSER_HPP_
    #define CLIENT_SRC_PARSING_PARSER_HPP_

    #include <exception>
    #include <string>
    #include <vector>

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
        void parseArgs();
        std::string getUsage();
 private:
        std::vector<std::string> _args;
        int _argc;
};
}  // namespace jetpack

#endif  // CLIENT_SRC_PARSING_PARSER_HPP_
