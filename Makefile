# ==============================================================================
# MINISHELL MAKEFILE - Enhanced with Dynamic Color-Gradient Progress Bars
# ==============================================================================

# Project Configuration
NAME = minishell

# Directory Structure
SRC_DIR = src
OBJ_DIR = executables
INCL_DIR = includes
LIBFT_DIR = libft

# Readline Configuration (macOS with Homebrew)
BREW_READLINE := $(shell brew --prefix readline 2>/dev/null || echo "/usr/local")

# Progress Bar Configuration
PROGRESS_WIDTH := 50

# Compilation Settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -I$(LIBFT_DIR) -I$(INCL_DIR) -I$(BREW_READLINE)/include
LDFLAGS = -L$(BREW_READLINE)/lib -lreadline -pthread

# ==============================================================================
# MODULAR PROGRESS BAR FUNCTIONS
# ==============================================================================

# Function: Calculate RGB values for smooth red-to-green transition
# Parameters: $(1) = current progress (0-100), $(2) = color component (r/g/b)
define calc_color_component
$(shell \
	progress=$(1); \
	if [ "$(2)" = "r" ]; then \
		echo $$((255 - progress * 255 / 100)); \
	elif [ "$(2)" = "g" ]; then \
		echo $$((progress * 255 / 100)); \
	else \
		echo 0; \
	fi \
)
endef

# Function: Generate ANSI color code for current progress percentage
# Parameters: $(1) = progress percentage (0-100)
define get_gradient_color
$(shell \
	progress=$(1); \
	r=$$(echo $$((255 - progress * 255 / 100))); \
	g=$$(echo $$((progress * 255 / 100))); \
	printf "\033[38;2;%d;%d;0m" $$r $$g \
)
endef

# Function: Render dynamic progress bar with gradient colors
# Parameters: $(1) = current, $(2) = total, $(3) = description
# This is the core function that handles all progress bar rendering logic
define render_progress_bar
	$(eval CURRENT := $(1))
	$(eval TOTAL := $(2))
	$(eval DESCRIPTION := $(3))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT) * 100 / $(TOTAL)))))
	$(eval FILLED := $(shell echo $$(($(CURRENT) * $(PROGRESS_WIDTH) / $(TOTAL)))))
	$(eval EMPTY := $(shell echo $$(($(PROGRESS_WIDTH) - $(FILLED)))))
	$(eval GRADIENT_COLOR := $(call get_gradient_color,$(PERCENTAGE)))
	@printf "\r\033[K$(GRADIENT_COLOR)"
	@if [ $(FILLED) -gt 0 ]; then \
		for i in $$(seq 1 $(FILLED)); do printf "■"; done; \
	fi
	@printf "\033[90m"
	@if [ $(EMPTY) -gt 0 ]; then \
		for i in $$(seq 1 $(EMPTY)); do printf "░"; done; \
	fi
	@printf "\033[0m $(PERCENTAGE)%% - $(DESCRIPTION)"
	@if [ $(CURRENT) -eq $(TOTAL) ]; then \
		printf "\n"; \
	fi
endef

# Function: Display final success or error message with visual enhancement
# Parameters: $(1) = success/error, $(2) = message, $(3) = details
define show_final_message
	@echo ""
	@if [ "$(1)" = "success" ]; then \
		printf "\033[1;32m✅ $(2)\033[0m"; \
	else \
		printf "\033[1;31m❌ $(2)\033[0m"; \
	fi
	@if [ "$(3)" != "" ]; then \
		printf " $(3)"; \
	fi
	@echo ""
	@echo ""
	@echo ""
endef

# Function: Animated progress for compilation with file-by-file tracking
# This function updates the progress bar for each compiled file
define show_compilation_progress
	$(eval TOTAL_FILES := $(words $(SRC)))
	$(eval CURRENT_FILE := $(shell if [ -f .build_count ]; then cat .build_count; else echo 0; fi))
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	@echo $(CURRENT_FILE) > .build_count
	$(call render_progress_bar,$(CURRENT_FILE),$(TOTAL_FILES),compiling minishell)
	@if [ $(CURRENT_FILE) -eq $(TOTAL_FILES) ]; then rm -f .build_count; fi
endef

# Function: Animated progress for cleaning operations
# Parameters: $(1) = step number, $(2) = total steps, $(3) = operation description
define show_cleanup_progress
	$(eval STEP := $(1))
	$(eval TOTAL_STEPS := $(2))
	$(eval OPERATION := $(3))
	$(call render_progress_bar,$(STEP),$(TOTAL_STEPS),$(OPERATION))
	@sleep 0.2
endef

# Function: Smooth animated progress with variable steps
# Parameters: $(1) = start, $(2) = end, $(3) = total, $(4) = description
define show_smooth_progress
	@for i in $$(seq $(1) $(2)); do \
		percentage=$$(($$i * 100 / $(3))); \
		filled=$$(($$i * 50 / $(3))); \
		empty=$$((50 - $$filled)); \
		r=$$((255 - $$percentage * 255 / 100)); \
		g=$$(($$percentage * 255 / 100)); \
		printf "\r\033[K\033[38;2;%d;%d;0m" $$r $$g; \
		if [ $$filled -gt 0 ]; then for j in $$(seq 1 $$filled); do printf "■"; done; fi; \
		printf "\033[90m"; \
		if [ $$empty -gt 0 ]; then for j in $$(seq 1 $$empty); do printf "░"; done; fi; \
		printf "\033[0m $$percentage%% - $(4)"; \
		sleep 0.03; \
	done; \
	printf "\n"
endef

# ==============================================================================
# SOURCE FILES CONFIGURATION
# ==============================================================================

SRC = 	main.c \
		init/initialisation.c \
		init/shlvl_utils.c \
		utils/init_error.c \
		utils/free_utils.c \
		utils/free_utils_2.c \
		utils/cleanup.c \
		utils/builtins_utils.c \
		utils/print_utils/print_env_list.c \
		utils/print_utils/print_exec_utils.c \
		utils/print_utils/print_ast.c \
		utils/print_utils/print_token_list.c \
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
		parsing/expansion/expansion_utils1.c \
		parsing/expansion/expansion_utils2.c \
		ast/ast_builder.c \
		ast/ast_utils.c \
		ast/ast_executor.c \
		env/init_env.c \
		env/env.c \
		env/env_utils.c \
		env/increment_shlvl.c \
		exec/exec_core.c \
		exec/exec_command.c \
		exec/exec_args.c \
		exec/exec_free.c \
		exec/path_resolution.c \
		exec/pipe/pipe_execution.c \
		exec/pipe/pipe_simple.c \
		exec/pipe/pipe_simple_child.c \
		exec/pipe/pipe_complex.c \
		exec/pipe/pipe_complex_utils.c \
		exec/pipe/pipe_utils.c \
		exec/pipe/pipe_child_utils.c \
		exec/pipe/pipe_cleanup.c \
		exec/pipe/pipe_heredoc.c \
		exec/pipe/pipe_status.c \
		exec/pipe/pipe_fd_utils.c \
		exec/redirection.c \
		exec/redirection_utils.c \
		exec/exec_utils.c \
		exec/exec_fork.c \
		exec/heredoc/heredoc_core.c \
		exec/heredoc/heredoc_utils.c \
		exec/heredoc/heredoc_expansion.c \
		exec/heredoc/heredoc_signals.c \
		exec/heredoc/heredoc_fd_utils.c \
		exec/heredoc/heredoc_processing.c \
		signals/signals.c \
		built-in/builtin_exec.c\
		built-in/cd.c\
		built-in/echo.c\
		built-in/env.c\
		built-in/pwd.c\
		built-in/unset.c\
		built-in/exit.c \
		built-in/export.c \
		built-in/export_utils.c \

# Path Resolution
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))

# Library Configuration
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft
INC = -I$(LIBFT_DIR) -I$(INCL_DIR)

# ==============================================================================
# BUILD RULES
# ==============================================================================

# Default rule: Build everything with enhanced progress tracking
all: $(LIBFT) init_build_env $(NAME)
	@if [ ! -f .compilation_happened ]; then \
		echo "\033[1;32m✅ Everything is up to date! No compilation needed.\033[0m"; \
	fi
	@rm -f .compilation_happened .compilation_started

# Initialize build environment and clean previous build artifacts
init_build_env:
	@rm -f .build_count .compilation_started .compilation_happened

# Main executable linking with progress indication
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	$(call show_final_message,success,Build completed successfully!,Executable '$(NAME)' ready)

# Individual object file compilation with progress tracking
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@if [ ! -f .compilation_started ]; then \
		echo "\033[1;34m🔨 Starting minishell compilation...\033[0m"; \
		touch .compilation_started; \
	fi
	@touch .compilation_happened
	$(call show_compilation_progress)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Create object directory structure (mirrors source structure)
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/input
	@mkdir -p $(OBJ_DIR)/syntax/validation
	@mkdir -p $(OBJ_DIR)/syntax/tokenization
	@mkdir -p $(OBJ_DIR)/syntax/parsing
	@mkdir -p $(OBJ_DIR)/ast
	@mkdir -p $(OBJ_DIR)/env
	@mkdir -p $(OBJ_DIR)/init
	@mkdir -p $(OBJ_DIR)/parsing/expansion
	@mkdir -p $(OBJ_DIR)/utils/print_utils
	@mkdir -p $(OBJ_DIR)/exec/pipe
	@mkdir -p $(OBJ_DIR)/exec/heredoc
	@mkdir -p $(OBJ_DIR)/built-in
	@mkdir -p $(OBJ_DIR)/signals

# Libft compilation with progress indication
$(LIBFT):
	@echo "\033[1;34m📚 Starting libft compilation...\033[0m"
	@touch .compilation_happened
	$(call show_smooth_progress,0,39,39,compiling libft program)
	@$(MAKE) -C $(LIBFT_DIR) -s
	$(call show_final_message,success,Libft compiled successfully!,)

# ==============================================================================
# CLEANUP RULES WITH ENHANCED PROGRESS BARS
# ==============================================================================

# Clean intermediate files with animated progress
clean:
	@echo "\033[1;33m🧹 Starting cleanup process...\033[0m"
	$(call show_smooth_progress,0,30,30,cleaning project objects)
	@rm -rf $(OBJ_DIR) 2>/dev/null || true
	@rm -f .build_count .compilation_started 2>/dev/null || true
	@$(MAKE) -C $(LIBFT_DIR) clean -s 2>/dev/null || true
	$(call show_final_message,success,Cleanup completed!,Object files and temporary files removed)

# Full clean including executables
fclean: clean
	@echo "\033[1;33m🗑️  Removing executables...\033[0m"
	$(call show_smooth_progress,0,15,15,removing executables)
	@rm -f $(NAME) 2>/dev/null || true
	@$(MAKE) -C $(LIBFT_DIR) fclean -s 2>/dev/null || true
	$(call show_final_message,success,Full cleanup completed!,All executables and libraries removed)

# Rebuild everything from scratch
re: fclean all

# Phony targets (not actual files)
.PHONY: all clean fclean re init_build_env
