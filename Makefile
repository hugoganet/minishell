# Nom de l'exécutable
NAME = minishell

# Dossiers personnalisés
SRC_DIR = src
OBJ_DIR = executables
INCL_DIR = includes
LIBFT_DIR = libft

BREW_READLINE := $(shell brew --prefix readline)

# Commandes et options de compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -I$(INCL_DIR) -I$(BREW_READLINE)/include
LDFLAGS = -L$(BREW_READLINE)/lib -lreadline -pthread

# Fichiers sources
SRC = 	main.c \
		init/initialisation.c \
		utils/init_error.c \
		utils/print_token_list.c \
		utils/free_utils.c \
		utils/print_env_list.c \
		utils/print_ast.c \
		utils/cleanup.c \
		input/loop.c \
		input/prompt.c \
		syntax/syntax_check.c \
		syntax/syntax_utils.c \
		syntax/tokenize.c \
		syntax/token_utils.c \
		syntax/validation.c \
		syntax/refine_token_type.c \
		parsing/process_input.c \
		syntax/build_token_list.c \
		ast/ast_builder.c \
		ast/ast_executor.c \
		env/init_env.c \
		env/var_expand.c \
		env/var_utils.c \
		env/env.c \
		env/env_utils.c \
		exec/exec_cmd.c \
		exec/resolve_command_path.c \
		exec/pipe.c \
		exec/redirection.c \
		exec/heredoc.c \
		signals/signals.c \
		built-in/builtin_exec.c\
		built-in/cd.c\
		built-in/echo.c\
		built-in/env.c\
		built-in/pwd.c\
		built-in/unset.c\
		built-in/exit.c \
		built-in/export.c \
	

# Rassembler les sources
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

# Fichiers objets
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))

# Liens vers les librairies des sous-projets
LIBFT = $(LIBFT_DIR)/libft.a

# Drapeaux pour les librairies
LIBS = -L$(LIBFT_DIR) -lft
INC = -I$(LIBFT_DIR) -I$(INCL_DIR)

# Règle par défaut
all: $(LIBFT) $(NAME)

# Compilation de l'exécutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	@echo "Compilation réussie de $(NAME)"

# Règle pour compiler les objets dans SRC (autres fichiers)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@) 
	@echo "Compilation de $< en $@"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Création du dossier des fichiers objets, y compris les sous-dossiers
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/input
	mkdir -p $(OBJ_DIR)/syntax
	mkdir -p $(OBJ_DIR)/ast
	mkdir -p $(OBJ_DIR)/env
	mkdir -p $(OBJ_DIR)/input
	mkdir -p $(OBJ_DIR)/init
	mkdir -p $(OBJ_DIR)/parsing
	mkdir -p $(OBJ_DIR)/utils

# Générer les librairies des sous-projets
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Nettoyer les objets et librairies compilés y compris les sous-dossiers dans executable/
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Tout reconstruire
re: fclean all

# Pas de fichiers de sortie ici
.PHONY: all clean fclean re
