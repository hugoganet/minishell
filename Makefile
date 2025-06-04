 # Nom de l'exécutable
NAME = Minishell

# Dossiers personnalisés
SRC_DIR = src
OBJ_DIR = executables
INCL_DIR = includes
LIBFT_DIR = libft

# Commandes et options de compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -I$(INCL_DIR) -I$(BREW_READLINE)/include
LDFLAGS = -L$(BREW_READLINE)/lib -lreadline -pthread

# Détection automatique du chemin readline via brew
BREW_READLINE := $(shell brew --prefix readline)

# macOS build rule (avoids recursive variable reference)
macos: clean
	$(MAKE) CFLAGS="-Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -I$(INCL_DIR) -I$(BREW_READLINE)/include" LDFLAGS="-lreadline -pthread -L$(BREW_READLINE)/lib" all
	@echo "Compilation macOS réussie de $(NAME) avec Homebrew Readline."

# Fichiers sources
SRC = 	main.c \
		init/initialisation.c \
		utils/init_error.c \
		utils/print_token_list.c \
		utils/free_token_list.c \
		utils/print_env_list.c \
		utils/print_ast.c \
		input/loop.c \
		input/prompt.c \
		input/readline_hooks.c \
		syntax/syntax_check.c \
		syntax/syntax_utils.c \
		syntax/tokenize.c \
		syntax/token_utils.c \
		syntax/validation.c \
		parsing/process_input.c \
		syntax/build_token_list.c \
		ast/ast_builder.c \
		env/init_env.c \
		env/var_expand.c \
		env/var_utils.c \
	

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
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(LDFLAGS)
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

# Lancer le shell
go: macos
	@echo "==> Lancement de Minishell..."
	@./$(NAME)

leaks_macos:
	@echo "==> Lancement de Minishell avec leaks..."
	@leaks --atExit -- ./$(NAME)

# Cible pour les tests de tokenisation sur macOS
token_tests_mac:
	@echo "==> Compilation des tests de tokenisation..."
	$(CC) -Wall -Wextra -Werror -g3 \
		-I$(INCL_DIR) -I$(LIBFT_DIR) -I/opt/homebrew/opt/readline/include \
		-L$(LIBFT_DIR) -lft \
		src/tests/tokenizer_test.c \
		src/syntax/tokenize.c \
		src/utils/print_token_list.c \
		src/utils/free_token_list.c \
		src/utils/init_error.c \
		src/syntax/token_utils.c \
		src/syntax/build_token_list.c \
		-o tokenizer_tests
	@echo "✅ Binaire de test créé : ./tokenizer_tests"
	@echo "==> Lancement avec leaks..."
	@leaks --atExit -- ./tokenizer_tests

# Cible pour les tests de tokenisation
token_tests:
	@echo "==> Compilation des tests de tokenisation..."
	$(CC) -Wall -Wextra -Werror -g3 \
		-I$(INCL_DIR) -I$(LIBFT_DIR) \
		src/tests/tokenizer_test.c \
		src/syntax/tokenize.c \
		src/utils/print_token_list.c \
		src/utils/free_token_list.c \
		src/utils/init_error.c \
		src/syntax/token_utils.c \
		src/syntax/build_token_list.c \
		-o tokenizer_tests
	@echo "✅ Binaire de test créé : ./tokenizer_tests"
	@echo "==> Lancement avec leaks..."
	@valgrind --leak-check=full --show-leak-kinds=all ./tokenizer_tests


# Pas de fichiers de sortie ici
.PHONY: all clean fclean re macos go leaks token_tests 
