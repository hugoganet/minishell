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
		utils/export_utils.c \
		input/loop.c \
		input/prompt.c \
		syntax/validation/input_validation.c \
		syntax/validation/pipe_validation.c \
		syntax/validation/redirection_validation.c \
		syntax/validation/parentheses_validation.c \
		syntax/validation/braces_validation.c \
		syntax/validation/validation.c \
		syntax/tokenization/token_parsing.c \
		syntax/tokenization/token_reading.c \
		syntax/tokenization/tokenize_core.c \
		syntax/tokenization/token_type_utils.c \
		syntax/tokenization/token_validation.c \
		syntax/tokenization/build_token_list.c \
		syntax/parsing/refine_token_type.c \
		syntax/parsing/logic_groups.c \
		syntax/syntax_utils.c \
		parsing/process_input.c \
		parsing/expansion/expansion.c \
		parsing/expansion/expansion_ast.c \
		parsing/expansion/expansion_quotes.c \
		parsing/expansion/expansion_specials.c \
		parsing/expansion/expansion_utils.c \
		ast/ast_builder.c \
		ast/ast_executor.c \
		env/init_env.c \
		env/env.c \
		env/env_utils.c \
		env/var_utils.c \
		exec/exec_core.c \
		exec/exec_command.c \
		exec/exec_args.c \
		exec/exec_free.c \
		exec/path_resolution.c \
		exec/pipe_execution.c \
		exec/redirection.c \
		exec/exec_utils.c \
		exec/heredoc/heredoc_core.c \
		exec/heredoc/heredoc_utils.c \
		exec/heredoc/heredoc_expansion.c \
		exec/heredoc/heredoc_signals.c \
		exec/heredoc/heredoc_fd_utils.c \
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
	mkdir -p $(OBJ_DIR)/expansion
	mkdir -p $(OBJ_DIR)/init
	mkdir -p $(OBJ_DIR)/parsing
	mkdir -p $(OBJ_DIR)/utils
	mkdir -p $(OBJ_DIR)/exec
	mkdir -p $(OBJ_DIR)/exec/heredoc
	mkdir -p $(OBJ_DIR)/built-in
	mkdir -p $(OBJ_DIR)/signals

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
