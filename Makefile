# Nom de l'exécutable
NAME = Minishell

# Dossiers personnalisés
SRC_DIR = SRC
OBJ_DIR = executable
INCL_DIR = INCL
LIBFT_DIR = libft

# Commandes et options de compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline -pthread

# Fichiers sources
SRC = tokenisation.c

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
	@echo "Compilation de $< en $@"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Création du dossier des fichiers objets, y compris les sous-dossiers
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Générer les librairies des sous-projets
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Nettoyer les objets et librairies compilés
clean:
	rm -rf $(OBJ_DIR)/*.o
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Tout reconstruire
re: fclean all

# Pas de fichiers de sortie ici
.PHONY: all clean fclean re
