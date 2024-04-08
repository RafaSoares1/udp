RESET   = \033[0m
BLACK   = \033[1;30m
RED     = \033[1;31m
GREEN   = \033[1;32m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
PURPLE  = \033[1;35m
CYAN    = \033[1;36m
WHITE   = \033[1;37m

NAME = udpProgram

CXX = c++

CXXFLAGS = -g -Wall -Wextra -Werror -std=c++17 -g

RM = rm -rf

SRCS = demo/main.cpp \
		demo/udp_socket.cpp \
		lib/src/reactor.cpp \


OBJS = $(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	clear
	@echo "$(GREEN)./udpProgram executable is ready!$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(RED)Object files have been deleted!$(RESET)"

fclean: clean
	@$(RM) $(NAME) 
	clear
	@echo "$(RED)Object and Executable files have been deleted!$(RESET)"

re: fclean all

.SILENT: