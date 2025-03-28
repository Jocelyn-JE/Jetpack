##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile
##

.PHONY: all clean fclean re tests_run vg cs linter

%.o: %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

SHARED_SRC			=	./src/shared/parsing/Parser.cpp						\
						./src/shared/socket/Socket.cpp						\

SHARED_OBJ			=	$(SHARED_SRC:.cpp=.o)

SERVER_BINARY_NAME	=	jetpack_server

MAIN_SERVER_SRC		=	./src/server/main.cpp

SERVER_SRC			=	./src/server/client/Client.cpp						\
						./src/server/pollfdlist/PollFdList.cpp				\
						./src/server/Server.cpp								\

SERVER_OBJ			=	$(SERVER_SRC:.cpp=.o)

MAIN_SERVER_OBJ		=	$(MAIN_SERVER_SRC:.cpp=.o)

CLIENT_BINARY_NAME	=	jetpack_client

MAIN_CLIENT_SRC		=	./src/client/main.cpp

CLIENT_SRC			=

CLIENT_OBJ			=	$(CLIENT_SRC:.cpp=.o)

MAIN_CLIENT_OBJ		=	$(MAIN_CLIENT_SRC:.cpp=.o)

TESTS_SRC			=

INCLUDES			=	-I ./src/shared/parsing -I ./src/shared/socket	\
						-I ./src/shared/utility -I ./src/server/pollfdlist	\
						-I ./src/server/client

CPPFLAGS			+=	-std=c++20 -Wall -Wextra -Werror $(INCLUDES) -O2 -g

CPPTESTFLAGS		=	--coverage -lcriterion $(CPPFLAGS)

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
	--filter=-legal/copyright,-build/c++17,+build/c++20,-runtime/references	\
	--recursive																\

VALGRIND_LOG		=	valgrind.log

all: server client

$(SERVER_BINARY_NAME):	$(SERVER_OBJ) $(MAIN_SERVER_OBJ) $(SHARED_OBJ)
	g++ $(CPPFLAGS) -o $(SERVER_BINARY_NAME) $(SERVER_OBJ) \
$(MAIN_SERVER_OBJ) $(SHARED_OBJ)

$(CLIENT_BINARY_NAME):	$(CLIENT_OBJ) $(MAIN_CLIENT_OBJ) $(SHARED_OBJ)
	g++ $(SFML_FLAGS) -o $(CLIENT_BINARY_NAME) $(CLIENT_OBJ) \
$(MAIN_CLIENT_OBJ) $(SHARED_OBJ)

client: $(CLIENT_BINARY_NAME)

server: $(SERVER_BINARY_NAME)

vg: $(SERVER_BINARY_NAME) $(CLIENT_BINARY_NAME)
	valgrind $(VALGRIND_FLAGS) ./$(SERVER_BINARY_NAME) 4242 tests
	cat $(VALGRIND_LOG)
	valgrind $(VALGRIND_FLAGS) ./$(CLIENT_BINARY_NAME) 0.0.0.0 4242 tests
	cat $(VALGRIND_LOG)

tests_run:
	g++ -o unit_tests $(SERVER_SRC) $(CLIENT_SRC) $(SHARED_SRC) $(TESTS_SRC) \
$(CPPTESTFLAGS)
	./unit_tests
	gcovr --exclude tests/
	gcovr -e tests --branch

clean:
	rm -f $(SERVER_OBJ) $(MAIN_SERVER_OBJ) $(SHARED_OBJ)
	rm -f $(CLIENT_OBJ) $(MAIN_CLIENT_OBJ) $(SHARED_OBJ)
	rm -f *.gcda
	rm -f *.gcno
	rm -f vgcore.*
	rm -f *.log

fclean: clean
	rm -f $(SERVER_BINARY_NAME) $(CLIENT_BINARY_NAME)
	rm -f unit_tests

re: fclean all

cs:	clean
	coding-style . .
	cat coding-style-reports.log
	rm -f coding-style-reports.log

linter: clean
	cpplint $(CPPLINT_FLAGS) ./src/
