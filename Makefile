# Nom de l'exécutable
NAME = Minishell

# Dossiers personnalisés
SRC_DIR = src
OBJ_DIR = executables
INCL_DIR = includes
LIBFT_DIR = libft

# Commandes et options de compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline -pthread

# macOS Homebrew Readline paths
BREW_READLINE = /opt/homebrew/opt/readline

# macOS build rule (avoids recursive variable reference)
macos: clean
	$(MAKE) CFLAGS="-Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -I$(INCL_DIR) -I$(BREW_READLINE)/include" LDFLAGS="-lreadline -pthread -L$(BREW_READLINE)/lib" all
	@echo "Compilation macOS réussie de $(NAME) avec Homebrew Readline."

# Fichiers sources
SRC = 	main.c \
		init/initialisation.c \
		utils/init_error.c \
		input/loop.c \
		input/prompt.c \
		input/readline_hooks.c \
		syntax/syntax_check.c \
		syntax/syntax_utils.c \
		syntax/tokenisation.c \


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
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(LDFLAGS)
	@echo "Compilation réussie de $(NAME)"

# Règle pour compiler les objets dans SRC (autres fichiers)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) 
	@echo "Compilation de $< en $@"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Création du dossier des fichiers objets, y compris les sous-dossiers
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

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
