##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile
##

.PHONY: all clean fclean re tests_run vg cs linter format

%.o: %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

# Shared files ----------------------------------------------------------------
SHARED_SRC			=	./src/shared/socket/Socket.cpp						\

SHARED_OBJ			=	$(SHARED_SRC:.cpp=.o)

# Server
SERVER_BINARY_NAME	=	jetpack_server

MAIN_SERVER_SRC		=	./src/server/main.cpp

SERVER_SRC			=	./src/server/client/Client.cpp						\
						./src/server/pollfdlist/PollFdList.cpp				\
						./src/server/Server.cpp								\
						./src/server/parsing/Parser.cpp						\

SERVER_OBJ			=	$(SERVER_SRC:.cpp=.o)

MAIN_SERVER_OBJ		=	$(MAIN_SERVER_SRC:.cpp=.o)

# Client
CLIENT_BINARY_NAME	=	jetpack_client

MAIN_CLIENT_SRC		=	./src/client/main.cpp

CLIENT_SRC			=	./src/client/Graphic.cpp							\
						./src/client/Menu/Menu.cpp							\
						./src/client/Game/Game.cpp							\
						./src/client/Program.cpp							\
						./src/client/Parser/ClientParser.cpp				\
						./src/client/Player/Player.cpp						\
						./src/client/Coin/Coin.cpp							\
						./src/client/Laser/Laser.cpp						\

CLIENT_OBJ			=	$(CLIENT_SRC:.cpp=.o)


MAIN_CLIENT_OBJ		=	$(MAIN_CLIENT_SRC:.cpp=.o)

# Library files ---------------------------------------------------------------
LIB_SRC				=	./src/shared/utility/splitString.cpp				\
						./src/shared/utility/isNumber.cpp					\
						./src/shared/sfml/Sprite.cpp						\
						./src/shared/sfml/SpriteSheet.cpp					\

LIB_OBJ				=	$(LIB_SRC:.cpp=.o)

LIB_NAME			=	libjetpack.a

# Tests sources ---------------------------------------------------------------
TESTS_SRC			=

# Flags -----------------------------------------------------------------------
INCLUDES			=	-I ./src/shared/parsing -I ./src/shared/socket		\
						-I ./src/shared/utility -I ./src/server/pollfdlist	\
						-I ./src/server/client  -I ./src/shared/sfml

CPPFLAGS			+=	-std=c++20 -Wall -Wextra -Werror $(INCLUDES) 		\
 						-L./ -ljetpack -O2 -g

CPPTESTFLAGS		=	--coverage -lcriterion $(CPPFLAGS) $(SFML_FLAGS)

SFML_FLAGS			=	\
	-lsfml-graphics															\
	-lsfml-audio															\
	-lsfml-window 															\
	-lsfml-system															\

VALGRIND_FLAGS		=														\
	--leak-check=full														\
	--show-leak-kinds=definite												\
	--track-origins=yes														\
	--errors-for-leak-kinds=definite										\
	--log-file="$(VALGRIND_LOG)"											\

CPPLINT_FLAGS		=														\
	--root=./include														\
	--repository=. 															\
	--filter=-legal/copyright,-build/c++17,+build/c++20,-runtime/references,$\
-build/include_subdir,-build/c++11											\
	--recursive																\

VALGRIND_LOG		=	valgrind.log

# Rules -----------------------------------------------------------------------
all: server client

$(SERVER_BINARY_NAME):	$(SERVER_OBJ) $(MAIN_SERVER_OBJ) $(SHARED_OBJ) \
	$(LIB_NAME)
	g++ $(CPPFLAGS) -o $(SERVER_BINARY_NAME) $(SERVER_OBJ) \
$(MAIN_SERVER_OBJ) $(SHARED_OBJ)

$(CLIENT_BINARY_NAME):	$(CLIENT_OBJ) $(MAIN_CLIENT_OBJ) $(SHARED_OBJ) \
	$(LIB_NAME)
	g++ -o $(CLIENT_BINARY_NAME) $(CLIENT_OBJ) \
$(MAIN_CLIENT_OBJ) $(SHARED_OBJ) $(CPPFLAGS) $(SFML_FLAGS)

client: $(CLIENT_BINARY_NAME)

server: $(SERVER_BINARY_NAME)

$(LIB_NAME): $(LIB_OBJ)
	ar rc $(LIB_NAME) $(LIB_OBJ)

vg: $(SERVER_BINARY_NAME) $(CLIENT_BINARY_NAME)
	valgrind $(VALGRIND_FLAGS) ./$(SERVER_BINARY_NAME) 4242 tests
	cat $(VALGRIND_LOG)
	valgrind $(VALGRIND_FLAGS) ./$(CLIENT_BINARY_NAME) 0.0.0.0 4242 tests
	cat $(VALGRIND_LOG)

tests_run: $(LIB_NAME)
	g++ -o unit_tests $(SERVER_SRC) $(CLIENT_SRC) $(SHARED_SRC) $(TESTS_SRC) \
$(CPPTESTFLAGS)
	./unit_tests
	gcovr --exclude tests/
	gcovr -e tests --branch

clean:
	rm -f $(SERVER_OBJ) $(MAIN_SERVER_OBJ) $(SHARED_OBJ)
	rm -f $(CLIENT_OBJ) $(MAIN_CLIENT_OBJ)
	rm -f $(LIB_OBJ)
	rm -f *.gcda
	rm -f *.gcno
	rm -f vgcore.*
	rm -f *.log

fclean: clean
	rm -f $(SERVER_BINARY_NAME) $(CLIENT_BINARY_NAME) $(LIB_NAME)
	rm -f unit_tests

re: fclean all

cs:	clean
	coding-style . .
	cat coding-style-reports.log
	rm -f coding-style-reports.log

linter: clean
	cpplint $(CPPLINT_FLAGS) ./src/

format: clean
	find . -type f \( -name "*.cpp" -o -name "*.hpp" \) ! -path "./tests/*"	\
	-exec clang-format -i {} +
