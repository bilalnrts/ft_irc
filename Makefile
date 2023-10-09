NAME		=	ircserver
CC			=	g++
CPPFLAGS	=	-Wall -Wextra -Werror -g -std=c++98
SRC_DIR		=	src/
OBJ_DIR		=	obj/
rm			=	rm -f

SRC_NAMES	=	Channel Command Execute Numeric Server User Utils main
SRC_FILES	=	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_NAMES)))
OBJ_FILES	=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_NAMES)))

OBJ			=	$(OBJ_FILES)
OBJF		=	.cache_exists

all: 			$(NAME)

$(NAME):		$(OBJF) $(OBJ)
				@$(CC) $(CPPFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp $(OBJF)
				@$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJF):
				@mkdir -p $(OBJ_DIR)
				@touch $(OBJF)

clean:
				@$(RM) -r $(OBJ_DIR)
				@$(RM) $(OBJF)

fclean: clean
				@$(RM) $(NAME)
				@$(RM) -rf *.dSYM
				@find . -name ".DS_Store" -delete

re: fclean all

.PHONY: all clean fclean re
