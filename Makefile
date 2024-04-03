SRCS	= main.cpp Server_core.cpp Server_cmds.cpp Server_utils.cpp ClientData.cpp ChannelData.cpp

OBJS	= ${SRCS:.cpp=.o}

OBJB	= ${SRCSB:.cpp=.o}

NAME	=  IRC

CXX	= clang++

RM	= rm -f

CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address

.c.o:
		${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
				${CXX} ${CXXFLAGS} -o $(NAME) $(OBJS)

bonus:		
			@make OBJS="${OBJB}"

all:		${NAME}

clean:
				${RM} ${OBJS} ${OBJB}

fclean: 	clean
				${RM} ${NAME}

re: 		fclean all
