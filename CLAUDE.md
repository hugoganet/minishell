# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a **minishell** implementation for École 42 - a simplified shell that mimics bash behavior. The project recreates a Unix shell with core features including command execution, pipes, redirections, built-in commands, environment variables, and signal handling.

## Build System & Common Commands

### Building the Project
```bash
make                    # Build the minishell executable
make clean             # Remove object files
make fclean            # Remove object files and executable
make re                # Clean rebuild
```

### Testing
```bash
# Main test suite (comprehensive)
./test/script/script.sh

# CSV test suite (modular)
minishell_complete_tests_serie.csv

# Manual testing
./minishell                         # Interactive mode
./minishell "command"              # Single command mode (for testers)
```

### Memory Checking
```bash
# Valgrind is integrated into the test suite
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --suppressions=valgrind/readline.supp ./minishell
```

## Architecture Overview

### Core Processing Pipeline
1. **Input Handling** (`src/input/`) - Prompt display and command line reading with signal handling
2. **Syntax Validation** (`src/syntax/validation/`) - Input validation for quotes, pipes, redirections
3. **Tokenization** (`src/syntax/tokenization/`) - Breaking input into tokens (commands, operators, files)
4. **AST Building** (`src/ast/`) - Converting tokens into an Abstract Syntax Tree
5. **Expansion** (`src/parsing/expansion/`) - Variable expansion and quote removal
6. **Execution** (`src/exec/`) - AST traversal and command execution

### Key Components

#### Shell State (`t_shell` in `minishell.h`)
- `env` - Environment as string array (for execve)
- `env_list` - Environment as linked list (for manipulation)
- `export_list` - Exported variables list
- `tokens` - Current input tokens
- `ast` - Current AST
- `last_exit_status` - Exit code of last command
- `heredoc_fds` - File descriptors for heredoc pipes

#### AST Structure (`t_ast` in `ast.h`)
The AST represents the command structure with these node types:
- **CMD nodes** - Commands with arguments (`args` array)
- **PIPE nodes** - Pipeline operations (left and right child nodes)
- **Redirection nodes** - I/O redirections with file information
- Each node can have `redir` list for multiple redirections

#### Token System (`t_token` in `ast.h`)
Token types: `PIPE`, `REDIR_INPUT`, `REDIR_OUTPUT`, `REDIR_APPEND`, `HEREDOC`, `WORD`, `CMD`, `ARG`, `FILES`

### Module Organization

#### Built-in Commands (`src/built-in/`)
Implements: `cd`, `echo`, `env`, `export`, `pwd`, `unset`, `exit`
- Built-ins execute in parent process (except in pipes)
- `export` manages both environment and export lists
- `exit` handles numeric validation and argument limits

#### Execution Engine (`src/exec/`)
- **exec_core.c** - Main execution logic, built-in vs external command routing
- **pipe/** - Complex pipeline execution with process management
- **heredoc/** - Here-document processing with expansion and signal handling
- **redirection.c** - File descriptor manipulation for I/O redirection

#### Signal Handling (`src/signals/`)
- Global signal handling with `volatile sig_atomic_t g_signal`
- Different behavior in interactive vs heredoc vs child processes
- SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) handling

#### Environment Management (`src/env/`)
- Dual representation: string array (for execve) and linked list (for shell operations)
- `SHLVL` incrementing on shell initialization
- Environment inheritance and modification

### Important Implementation Details

#### Memory Management
- Each command cycle cleans tokens and AST but preserves environment
- Heredoc file descriptors tracked in shell structure for cleanup
- Comprehensive cleanup on shell exit

#### Process Management
- Built-ins run in parent process (unless in pipeline)
- External commands fork with proper signal handling
- Pipe execution uses recursive child processes
- SIGPIPE ignored in pipe child processes to prevent "Broken Pipe" messages

#### Error Handling
- Syntax errors reported without executing
- Command not found errors follow bash format
- Exit status handling matches bash behavior (0-255 range)

## Testing Strategy

The project includes two comprehensive test suites:
- **test/script/script.sh** - Detailed comparison testing with Valgrind integration
- **minishell_complete_tests_serie.csv** - Modular testing with category selection

Tests cover:
- Built-in commands with edge cases
- Pipeline execution
- Redirections and heredocs
- Quote handling and variable expansion
- Signal behavior
- Memory leak detection
- Error handling and exit statuses

## Development Notes
- Complete dyoxygen documentation for every function and structure
- Follow strict coding standards (norminette)

### Signal Implementation
The project uses a global signal variable pattern for communication between signal handlers and main logic. This is necessary due to readline's signal handling requirements.

### Tester Integration
The main.c includes special handling for non-interactive mode (argc > 1) to support automated testing frameworks.

### Platform Compatibility
Includes conditional headers for macOS/Linux compatibility, particularly for system limits and readline library paths.