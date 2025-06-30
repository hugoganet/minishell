#!/bin/bash

# INCLUDE - Import des couleurs pour l'affichage coloré des résultats
source ./colors.sh

# VARIABLES GLOBALES
declare	PART  # Variable pour identifier la partie en cours de test

# Configuration de Valgrind pour la détection de fuites mémoire
declare VALGRIND="valgrind"
declare MEMCHECK="--leak-check=full --show-leak-kinds=all --track-origins=yes\
					--trace-children=yes"  # Options pour un contrôle mémoire complet

declare LOG_VALGRIND="--log-file="  # Préfixe pour les fichiers de log Valgrind
declare LOG_FILE="../"  # Chemin du fichier de log courant

# Fichiers de test utilisés pour les redirections et tests d'entrée/sortie
declare FILE_BINARY="test/file/binary"           # Fichier binaire pour tests de permissions
declare FILE_EMPTY="test/file/empty.txt"         # Fichier vide
declare FILE_INFILE1="test/file/infile1.txt"     # Fichier d'entrée 1
declare FILE_INFILE2="test/file/infile2.txt"     # Fichier d'entrée 2  
declare FILE_REGULAR="test/file/regular.txt"     # Fichier texte normal
declare FILE_OUTFILE1="test/file/outfile1.txt"   # Fichier de sortie 1
declare FILE_OUTFILE2="test/file/outfile2.txt"   # Fichier de sortie 2
declare FILE_NO_PERMISSION="test/file/no_permission.txt"  # Fichier sans permissions

# Fichiers temporaires pour comparer les sorties
declare	OUTFILE_TMP="test/log/outfile_tmp.txt"        # Fichier temporaire
declare	OUTFILE_BASH="test/log/outfile_bash.txt"      # Sortie de bash
declare	OUTFILE_MINISHELL="test/log/outfile_minishell.txt"  # Sortie de minishell

declare BUILTIN=false  # Flag pour identifier si on teste les builtins


# FONCTION PRINCIPALE
main()
{
	# Se déplace vers le répertoire racine du projet (../../ depuis test/script/)
	pushd ../.. > /dev/null
	
	# Compile le projet avec make. Si la compilation échoue, on sort
	if make > /dev/null; then
		# clear  # Nettoie l'écran (commenté)
		# print_header  # Affiche l'en-tête (commenté)
		
		# Initialise l'environnement de test
		init_tester
		
		# Si des arguments sont passés au script, exécute seulement le test demandé
		if [ $# -gt 0 ]; then
			if [ "$1" = "builtin" ]; then
				test_builtin           # Test des commandes intégrées (echo, cd, env, etc.)
			elif [ "$1" = "simple_command" ]; then
				test_simple_command    # Test des commandes simples (ls, cat, etc.)
			elif [ "$1" = "redirection" ]; then
				test_redirection       # Test des redirections (<, >, >>, <<)
			elif [ "$1" = "expansion" ]; then
				test_expansion         # Test de l'expansion de variables ($HOME, $PATH, etc.)
			elif [ "$1" = "heredoc" ]; then
				test_heredoc          # Test des here-documents (<<)
			elif [ "$1" = "quote" ]; then
				test_quote            # Test des guillemets simples et doubles
			elif [ "$1" = "pipeline" ]; then
				test_pipeline         # Test des pipes (|)
			elif [ "$1" = "signal" ]; then
				test_signal           # Test de la gestion des signaux (Ctrl+C, etc.)
			elif [ "$1" = "list" ]; then
				test_list             # Test des opérateurs && et ||
			elif [ "$1" = "wildcard" ]; then
				test_wildcard         # Test des wildcards (*)
			elif [ "$1" = "evaluation" ]; then
				test_evaluation       # Test d'évaluation générale
			fi
			exit 0
		fi
		
		# Si aucun argument, lance tous les tests dans l'ordre
		test_evaluation
		test_simple_command
		test_redirection
		test_heredoc
		test_expansion
		test_quote
		test_pipeline
		test_builtin
		test_signal
		test_list
		test_wildcard
	fi
	
	# Retourne au répertoire d'origine
	popd > /dev/null
	echo
	exit 0
}

# Affiche un en-tête décoratif pour le testeur (fonction non utilisée actuellement)
print_header()
{
	echo -e "${BOLD}${WHITE}\t\t############################${DEFAULT}"
	echo -e "${BOLD}${WHITE}\t\t##     MINISHELL TEST     ##${DEFAULT}"
	echo -e "${BOLD}${WHITE}\t\t############################\n${DEFAULT}"
}

# INITIALISATION DE L'ENVIRONNEMENT DE TEST
init_tester()
{
	# Supprime les anciens répertoires de test s'ils existent
	if [ -d "test/file" ] || [ -d "test/log" ]; then
		rm -rf	test/file test/log
	fi
	
	# Crée les répertoires nécessaires pour les tests
	mkdir	test/file test/log

	# Crée les fichiers de test avec du contenu prédéfini
	echo "infile1"			> ${FILE_INFILE1}    # Fichier d'entrée 1
	echo "infile2"			> ${FILE_INFILE2}    # Fichier d'entrée 2  
	echo "This is a test"	> ${FILE_REGULAR}    # Fichier avec texte normal

	# Supprime les anciens fichiers de sortie s'ils existent
	if [ -f ${FILE_OUTFILE1} ]; then
		rm ${FILE_OUTFILE1}
	fi
	if [ -f ${FILE_OUTFILE2} ]; then
		rm ${FILE_OUTFILE2}
	fi
	
	# Crée des fichiers spéciaux pour tester les permissions et autres cas
	touch ${FILE_EMPTY}                                # Fichier vide seulement (LOG_FILE sera créé plus tard)
	touch ${FILE_BINARY}		&& chmod 100 ${FILE_BINARY}        # Fichier binaire (exécutable seulement)
	touch ${FILE_NO_PERMISSION}	&& chmod 000 ${FILE_NO_PERMISSION}  # Fichier sans permissions
}

# TEST D'ÉVALUATION GÉNÉRALE
# Teste des cas basiques comme les commandes vides, les espaces, etc.
test_evaluation()
{
	local -a TEST
	PART="EVALUATION"
	mkdir -p test/log/evaluation    # -p évite les erreurs si le répertoire existe
	LOG_FILE="test/log/evaluation/"

	TEST=(
		""                                    # Commande vide
		"  "                                 # Espaces seulement
		"	"                                # Tabulation seulement  
		"echo -n"                            # Echo sans saut de ligne
		"/bin/ls"                            # Commande avec chemin absolu
		"echo \"cat lol.c | cat > lol.c\""   # Echo d'une chaîne complexe
	)
	launch_test "${TEST[@]}"
}

# TEST DES COMMANDES SIMPLES
# Teste l'exécution de commandes basiques avec ou sans options
test_simple_command()
{
	local -a TEST
	PART="SIMPLE COMMAND"
	mkdir -p test/log/simple_command    # -p évite les erreurs si le répertoire existe
	LOG_FILE="test/log/simple_command/"

	TEST=(
		""                              # Commande vide
		"   "                          # Espaces multiples
		"ls"                           # Commande simple
		"dir"                          # Autre commande simple (non-POSIX)
		"ls -l"                        # Commande avec option
		"dir -a"                       # Commande avec option
		"/bin/ls"                      # Chemin absolu
		"/bin/dir"                     # Chemin absolu
		"/bin/ls -l"                   # Chemin absolu avec option
		"/bin/dir -a"                  # Chemin absolu avec option
		# "ls wrong_opt"               # Option invalide (commenté)
		# "dir wrong_opt"              # Option invalide (commenté)
		"/bin/ls wrong_opt"            # Option invalide avec chemin absolu
		"/bin/dir wrong_opt"           # Option invalide avec chemin absolu
		"wrong_cmd"                    # Commande inexistante
		"wrong_cmd ls"                 # Commande inexistante avec argument
		"wrong_cmd wrong_opt"          # Commande et option inexistantes
		"wrong_cmd wrong_opt1 wrong_opt2"  # Commande inexistante avec multiples arguments
	)
	launch_test "${TEST[@]}"
}

# TEST DES REDIRECTIONS
# Teste toutes les formes de redirections d'entrée/sortie
test_redirection()
{
	local -a TEST
	PART="REDIRECTION"
	mkdir -p test/log/redirection       # -p évite les erreurs si le répertoire existe
	LOG_FILE="test/log/redirection/"

	TEST=(
		"<"                                                    # Redirection d'entrée seule (erreur de syntaxe)
		">"                                                    # Redirection de sortie seule (erreur de syntaxe)
		"<<"                                                   # Here-document seul (erreur de syntaxe)
		">>"                                                   # Redirection d'ajout seule (erreur de syntaxe)
		# "<>"	                                             # Opérateur spécifique à bash (commenté)
		# "<>>"                                               # Erreur de syntaxe bash (commenté)
		# "<<<"                                               # Opérateur spécifique à bash (commenté)
		"><"                                                   # Combinaison invalide
		"><<"                                                  # Combinaison invalide
		">>>"                                                  # Combinaison invalide
		"<<>"                                                  # Combinaison invalide
		"<<>>"                                                 # Combinaison invalide
		">><"                                                  # Combinaison invalide
		">><<"                                                 # Combinaison invalide
		"cat<${FILE_INFILE1}"                                  # Redirection collée à la commande
		"cat < ${FILE_INFILE1}"                                # Redirection d'entrée normale
		"cat < ${FILE_INFILE1} < ${FILE_INFILE1} < ${FILE_INFILE1}"  # Redirections multiples
		# "cat < ${FILE_NO_PERMISSION}"                       # Fichier sans permission (commenté)
		# "cat < ${FILE_BINARY}"                              # Fichier binaire (commenté)
		"cat<wrong_file"                                       # Fichier inexistant collé
		"cat < wrong_file"                                     # Fichier inexistant avec espace
		"cat < wrong_file1 < wrong_file2"                     # Multiples fichiers inexistants
		"cat < ${FILE_INFILE1} -n"                            # Redirection avec option
		"cat<${FILE_INFILE1}-n"                               # Redirection et option collées
		# "cat < ${FILE_INFILE1} wrong_opt"                   # Option invalide avec redirection (commenté)
		"cat < ${FILE_INFILE1} < ${FILE_INFILE2}"             # Multiples redirections d'entrée
		# "< ${FILE_INFILE1} cat < ${FILE_INFILE2} wrong_opt"  # Redirection avant commande avec option invalide (commenté)
		"< ${FILE_INFILE1} cat"                               # Redirection avant commande
		"< ${FILE_INFILE1} cat -n"                            # Redirection avant commande avec option
		"< ${FILE_INFILE1} cat < ${FILE_INFILE2}"             # Redirection avant et après commande
		"wrong_cmd < ${FILE_INFILE1}"                         # Commande invalide avec redirection
		"wrong_cmd < ${FILE_INFILE1} < ${FILE_INFILE2}"       # Commande invalide avec multiples redirections
		"< ${FILE_INFILE1} wrong_cmd"                         # Redirection avant commande invalide
		"< ${FILE_INFILE1} wrong_cmd < ${FILE_INFILE2}"       # Redirection avec commande invalide et autre redirection
		"ls > ${FILE_OUTFILE1}"                               # Redirection de sortie simple
		"ls > ${FILE_OUTFILE1} > ${FILE_OUTFILE2}"           # Multiples redirections de sortie
		"wrong_cmd > ${FILE_OUTFILE1}"                       # Commande invalide avec redirection de sortie
		"wrong_cmd > ${FILE_OUTFILE1} > ${FILE_OUTFILE2}"    # Commande invalide avec multiples redirections de sortie
	)
	launch_test "${TEST[@]}"
}

# TEST DES PIPELINES
# Teste les pipes (|) pour chaîner les commandes
test_pipeline()
{
	local -a TEST
	PART="PIPELINE"
	mkdir -p test/log/pipeline
	LOG_FILE="test/log/pipeline/"

	TEST=(
		"|"                                                              # Pipe seul (erreur de syntaxe)
		"ls | wc"                                                       # Pipeline simple : liste les fichiers et compte les lignes
		"ls | cat"                                                      # Pipeline simple : liste et affiche
		"ls | wrong_cmd"                                                # Pipeline avec commande invalide à droite
		"wrong_cmd | ls"                                                # Pipeline avec commande invalide à gauche
		"wrong_cmd | wrong_cmd"                                         # Pipeline avec commandes invalides des deux côtés
		"ls | wc | ls | wc | ls"                                       # Pipeline long avec commandes valides
		"ls | wrong_cmd | ls | wc | ls"                                # Pipeline long avec commande invalide au milieu
		"ls | wc | ls | wrong_cmd | ls"                                # Pipeline long avec commande invalide vers la fin
		"ls | wrong_cmd | ls | wrong_cmd | ls"                         # Pipeline long avec multiples commandes invalides
		"ls | sleep 2 | cat"                                           # Pipeline avec commande qui prend du temps
		"cat < ${FILE_INFILE1} | cat < ${FILE_INFILE2}"                # Pipeline avec redirections d'entrée
		"cat < ${FILE_INFILE1} > ${FILE_OUTFILE1} | cat < ${FILE_INFILE2} > ${FILE_OUTFILE1}"  # Pipeline complexe avec redirections
	)
	launch_test "${TEST[@]}"
}

# TEST DES COMMANDES INTÉGRÉES (BUILTINS)
# Teste toutes les commandes intégrées du shell : cd, echo, env, export, pwd, unset, exit
test_builtin()
{
	local -a TEST
	PART="BUILTIN"
	mkdir -p test/log/builtin
	LOG_FILE="test/log/builtin/"
	launch_test  # Lance un test vide pour initialiser

	# Active le mode builtin pour les sous-tests
	BUILTIN=true
	
	# TEST DES BUILTINS DANS LES PIPELINES
	PART="PIPELINE"
	mkdir -p test/log/builtin/pipeline
	LOG_FILE="test/log/builtin/pipeline/"
	TEST=(
		"cd | cd"              # cd dans un pipeline (comportement spécial)
		"echo | echo"          # echo dans un pipeline
		"env | env"            # env dans un pipeline
		"env | wc -l"          # env pipé vers wc pour compter les variables
		"env | grep HOME"      # env pipé vers grep pour filtrer
		"env | echo"           # env pipé vers echo
		"exit | exit"          # exit dans un pipeline (comportement spécial)
		"export | export"      # export dans un pipeline
		"pwd | pwd"            # pwd dans un pipeline
		"unset |unset"         # unset dans un pipeline (sans espace intentionnel)
	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE CD (CHANGE DIRECTORY)
	PART="CD"
	mkdir -p test/log/builtin/cd
	LOG_FILE="test/log/builtin/cd/"
	TEST=(
		"cd"                   # cd sans argument (va vers HOME)
		"cd ''"                # cd vers chaîne vide (guillemets simples)
		"cd \"\""              # cd vers chaîne vide (guillemets doubles)
		"cd /"                 # cd vers la racine
		"cd ."                 # cd vers le répertoire courant
		"cd .."                # cd vers le répertoire parent
		"cd ~"                 # cd vers HOME (tilde)
		"cd @"                 # cd vers un caractère invalide
		"cd -"                 # cd vers le répertoire précédent
		"cd _"                 # cd vers un caractère underscore
		"cd minishell"         # cd vers un sous-répertoire
		"cd wrong_dir"         # cd vers un répertoire inexistant
		"cd /dev/tty"          # cd vers un fichier spécial
		"cd /dev/null"         # cd vers /dev/null
	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE ECHO
	PART="ECHO"
	mkdir -p test/log/builtin/echo
	LOG_FILE="test/log/builtin/echo/"
	TEST=(
		"echo"                      # echo sans argument
		"echo ''"                   # echo chaîne vide (guillemets simples)
		"echo \"\""                 # echo chaîne vide (guillemets doubles)
		"echo -"                    # echo avec tiret seul
		"echo -n"                   # echo sans saut de ligne
		"echo \n"                   # echo avec \n échappé
		"echo -n \n"                # echo -n avec \n
		"echo --"                   # echo avec double tiret
		"echo -- \n"                # echo -- avec \n
		"echo -nn"                  # echo avec -nn (option multiple)
		"echo --n"                  # echo avec --n
		"echo --nn"                 # echo avec --nn
		"echo -n-n"                 # echo avec -n-n
		"echo -n-n \n"              # echo -n-n avec \n
		"echo -n-n test\n"          # echo -n-n avec texte et \n
		"echo -n -n"                # echo avec multiple -n séparés
		"echo -n -n \n"                    # echo avec multiple -n et \n
		"echo -n -n test\n"                # echo avec multiple -n, texte et \n
		"echo -n -n -n"                    # echo avec triple -n
		"echo -n -n -n \n"                 # echo avec triple -n et \n
		"echo -n -n -n test\n"             # echo avec triple -n, texte et \n
		"echo -a"                          # echo avec option invalide
		'echo $HOME'                       # echo avec variable d'environnement HOME
		'echo $USER'                       # echo avec variable d'environnement USER
		'echo $wrong_expan'                # echo avec variable inexistante
		'echo $HOME $USER'                 # echo avec multiples variables
		'echo $HOME$USER'                  # echo avec variables concaténées
		'echo $HOME$wrong_expan$USER'      # echo avec variable inexistante au milieu
	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE ENV (AFFICHAGE DES VARIABLES D'ENVIRONNEMENT)
	PART="ENV"
	mkdir -p test/log/builtin/env
	LOG_FILE="test/log/builtin/env/"
	TEST=(
		"env"                    # env sans argument (affiche toutes les variables)
		"env wrong_opt"          # env avec option invalide
		'env HOME'               # env avec nom de variable (invalide en bash)
		'env $HOME'              # env avec expansion de variable
	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE EXIT
	PART="EXIT"
	mkdir -p test/log/builtin/exit
	LOG_FILE="test/log/builtin/exit/"
	TEST=(
		"exit"                          # exit sans argument
		"exit \"\""                     # exit avec chaîne vide (guillemets doubles)
		"exit ''"                       # exit avec chaîne vide (guillemets simples)
		"exit -"                        # exit avec tiret
		"exit --"                       # exit avec double tiret
		"exit ++"                       # exit avec double plus
		"exit 0"                        # exit avec code 0
		"exit -0"                       # exit avec -0
		"exit +0"                       # exit avec +0
		"exit 0a"                       # exit avec nombre suivi de lettre
		"exit a0"                       # exit avec lettre suivie de nombre
		"exit 1"                        # exit avec code 1
		"exit -1"                       # exit avec -1
		"exit +1"                       # exit avec +1
		"exit 256"                      # exit avec code 256 (débordement sur 8 bits)
		"exit -256"                     # exit avec -256
		"exit +256"                     # exit avec +256
		"exit 2a56"                     # exit avec nombre invalide
		"exit 256a"                     # exit avec nombre suivi de lettre
		"exit non_numeric"              # exit avec chaîne non numérique
		"exit @"                        # exit avec caractère spécial
		"exit 1 1"                      # exit avec multiples arguments
		"exit a 1"                      # exit avec argument invalide puis valide
		"exit 1 1 1 1 1 1"              # exit avec trop d'arguments
		"exit 1 1 a 1 a 1"              # exit avec mélange d'arguments
		"exit 2147483647"               # exit avec INT_MAX
		"exit -2147483647"              # exit avec -INT_MAX
		"exit +2147483647"              # exit avec +INT_MAX
		"exit 2147483648"               # exit avec INT_MAX + 1 (débordement)
		"exit -2147483648"              # exit avec INT_MIN
		"exit +2147483648"              # exit avec débordement positif
		"exit 4294967295"               # exit avec UINT_MAX
		"exit -4294967295"              # exit avec -UINT_MAX
		"exit +4294967295"              # exit avec +UINT_MAX
		"exit 4294967296"               # exit avec UINT_MAX + 1
		"exit -4294967296"              # exit avec débordement négatif
		"exit +4294967296"              # exit avec débordement positif
		"exit 9223372036854775807"      # exit avec LONG_MAX
		"exit +9223372036854775807"     # exit avec +LONG_MAX
		"exit -9223372036854775807"     # exit avec -LONG_MAX
	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE EXPORT
	PART="EXPORT"
	mkdir -p test/log/builtin/export
	LOG_FILE="test/log/builtin/export/"
	TEST=(
		"export"                        # export sans argument (liste les variables exportées)
		"export \"\""                   # export avec chaîne vide (guillemets doubles)
		"export ''"                     # export avec chaîne vide (guillemets simples)
		"export ="                      # export avec signe égal seul
		"export @"                      # export avec caractère invalide
		"export ."                      # export avec point
		"export .."                     # export avec double point
		"export /"                      # export avec slash
		"export 0"                      # export avec chiffre (nom invalide)
		"export 0 1 2"                  # export avec multiples chiffres
		"export wrong_name"             # export d'une variable sans valeur
		"export wrong_name wrong_name"  # export de multiples variables

	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE PWD (PRINT WORKING DIRECTORY)
	PART="PWD"
	mkdir -p test/log/builtin/pwd
	LOG_FILE="test/log/builtin/pwd/"
	TEST=(
		"pwd"                    # pwd sans argument
		"pwd -"                  # pwd avec tiret
		'pwd $'                  # pwd avec dollar
		'pwd $HOME'              # pwd avec variable (invalide)
		"pwd pwd"                # pwd avec argument pwd
		"pwd wrong_opt"          # pwd avec option invalide
	)
	launch_test "${TEST[@]}"

	# TEST DE LA COMMANDE UNSET
	PART="UNSET"
	mkdir -p test/log/builtin/unset
	LOG_FILE="test/log/builtin/unset/"
	TEST=(
		"unset"                           # unset sans argument
		"unset -"                         # unset avec tiret
		"unset _"                         # unset avec underscore
		"unset 1"                         # unset avec chiffre (nom invalide)
		"unset @"                         # unset avec caractère spécial
		"unset wrong_opt"                 # unset avec option invalide
		'unset wrong_opt $wrong_expan'    # unset avec option et variable invalides
	)
	launch_test "${TEST[@]}"
	
	# Désactive le mode builtin
	BUILTIN=false
}

# TEST DES HERE-DOCUMENTS
# Teste les here-documents (<<) - actuellement non implémenté
test_heredoc()
{
	local -a TEST
	PART="HEREDOC"
	mkdir -p test/log/heredoc
	LOG_FILE="test/log/heredoc/"

	# TEST=(
	# 	"ls << eof"    # Exemple de here-document (commenté car non implémenté)
	# )
	launch_test "${TEST[@]}"  # Lance un test vide
}

# TEST DES GUILLEMETS
# Teste le comportement des guillemets simples et doubles
test_quote()
{
	local -a TEST
	PART="QUOTE"
	mkdir -p test/log/quote
	LOG_FILE="test/log/quote/"

	TEST=(
		"\"\""                                    # Chaîne vide avec guillemets doubles
		"''"                                      # Chaîne vide avec guillemets simples
		"'\"\"'"                                  # Guillemets doubles à l'intérieur de simples
		"\"''\"\"''\""                            # Alternance de guillemets
		"\"'\"\"\"\"'\""                          # Combinaison complexe de guillemets
		"\"'\"\"'\""                              # Autre combinaison
		"'\"''\"'"                                # Guillemets doubles dans simples
		"\"ls\""                                  # Commande dans guillemets doubles
		"'ls'"                                    # Commande dans guillemets simples
		"\"ls -l\""                               # Commande avec option dans guillemets doubles
		"'ls -l'"                                 # Commande avec option dans guillemets simples
		"\"ls | ls | ls | ls\""                   # Pipeline dans guillemets doubles
		"\"ls '|' ls | ls | ls\""                 # Pipeline mixte avec guillemets
		"\"ls | ls | ls '|' ls\""                 # Pipeline avec pipe quoté
		# "\"< ${FILE_INFILE1}\""                 # Redirection quotée (commenté)
		"\"< infile\""                            # Redirection quotée simple
		"\"< infile | < infile | < infile\""      # Redirections multiples quotées
	)
	launch_test "${TEST[@]}"
}

# TEST DE L'EXPANSION DE VARIABLES
# Teste l'expansion des variables d'environnement ($VAR)
test_expansion()
{
	local -a TEST
	PART="EXPANSION"
	mkdir -p test/log/expansion
	LOG_FILE="test/log/expansion/"

	TEST=(
		'$'                          # Dollar seul
		'$?'                         # Code de retour de la dernière commande
		'$HOME'                      # Variable d'environnement HOME
		'$PATH'                      # Variable d'environnement PATH
		'$wrong_expan'               # Variable inexistante
		'$wrong_expan ls'            # Variable inexistante avec commande
		'ls $wrong_expan'            # Commande avec variable inexistante
		'wrong_cmd $wrong_expan'     # Commande invalide avec variable inexistante
		'$wrong_expan wrong_cmd'     # Variable inexistante avec commande invalide
	)
	launch_test "${TEST[@]}"
}

# TEST DE LA GESTION DES SIGNAUX
# Teste la gestion des signaux (Ctrl+C, Ctrl+D, Ctrl+\)
test_signal()
{
	local -a TEST
	PART="SIGNAL"

	# Ces tests nécessitent une interaction manuelle :
	# Ctrl-C  : Signal SIGINT (interruption)
	# Ctrl-D  : EOF (fin de fichier)
	# Ctrl-\  : Signal SIGQUIT
	# 
	# Test de processus en arrière-plan :
	# ./minishell &    # Lance minishell en arrière-plan
	# sleep 1          # Attend 1 seconde
	# echo $!          # Affiche le PID du dernier processus en arrière-plan
	# kill $!          # Tue le processus
	# echo $?          # Affiche le code de retour
	
	launch_test "${TEST[@]}"  # Lance un test vide (signaux non automatisables)
}

# TEST DES OPÉRATEURS DE LISTE
# Teste les opérateurs logiques && (AND) et || (OR)
test_list()
{
	local -a TEST
	PART="LIST"

	TEST=(
		")"                          # Parenthèse fermante seule (erreur de syntaxe)
		"()"                         # Parenthèses vides (non supporté en minishell de base)
		"&&"                         # Opérateur AND seul (erreur de syntaxe)
		"||"                         # Opérateur OR seul (erreur de syntaxe)
		"echo a && echo b"           # AND logique : execute echo b seulement si echo a réussit
		"echo a || echo b"           # OR logique : execute echo b seulement si echo a échoue
	)
	launch_test "${TEST[@]}"
}

# TEST DES WILDCARDS
# Teste l'expansion des jokers (*)
test_wildcard()
{
	local -a TEST
	PART="WILDCARD"

	TEST=(
		"*"      # Wildcard seul : devrait correspondre à tous les fichiers du répertoire courant
	)
	launch_test "${TEST[@]}"
}

# FONCTION PRINCIPALE DE LANCEMENT DES TESTS
# Cette fonction compare le comportement de bash avec celui de minishell
launch_test()
{
	# Variables locales pour le traitement des tests
	local i=0                        # Compteur de boucle
	local ID=1                       # Numéro du test en cours
	local ERROR                      # Flag pour les erreurs Valgrind
	local len=${#TEST[@]}            # Nombre total de tests à exécuter
	local -a TEST                    # Tableau des tests à exécuter
	local BASH_EXIT_STATUS           # Code de retour de bash
	local MINISHELL_EXIT_STATUS      # Code de retour de minishell

	# Récupère les tests passés en paramètre
	TEST=("$@")
	
	# Affichage différent selon si on teste les builtins ou non
	if [ $BUILTIN == true ]; then
		echo -e "${PART}"                               # Affichage simple pour les builtins
	else
		echo -e "${BOLD}${WHITE}${PART}${DEFAULT}"      # Affichage en gras pour les autres tests
	fi
	
	# BOUCLE PRINCIPALE : exécute chaque test
	while [[ i -lt len ]]; do
		
		# S'assurer que le répertoire de log existe avant d'écrire
		mkdir -p "$(dirname "${LOG_FILE}${ID}.txt")"
		
		# Vérifier si le répertoire a été créé
		if [ ! -d "$(dirname "${LOG_FILE}${ID}.txt")" ]; then
			echo "ERREUR: Impossible de créer le répertoire $(dirname "${LOG_FILE}${ID}.txt")"
			return 1
		fi
		
		# CRÉATION DU FICHIER DE LOG POUR CE TEST
		echo -e 		"\t\t\t\t\t### ${PART} ###" >	"${LOG_FILE}${ID}.txt"
		echo -e			"\n\t-- Test[${ID}] --\n"	>>	"${LOG_FILE}${ID}.txt"
		echo			"INPUT=${TEST[i]}"			>>	"${LOG_FILE}${ID}.txt"

		# Vérifier si le fichier de log a été créé
		if [ ! -f "${LOG_FILE}${ID}.txt" ]; then
			echo "ERREUR: Impossible de créer le fichier de log ${LOG_FILE}${ID}.txt"
			return 1
		fi

		# CONSTRUCTION DE LA COMMANDE MINISHELL AVEC VALGRIND
		# Valgrind est utilisé pour détecter les fuites mémoire
		
		# Vérifier que l'exécutable existe
		if [ ! -f "./Minishell" ]; then
			echo "ERREUR: L'exécutable ./Minishell n'existe pas !"
			echo "Assurez-vous que la compilation a réussi avec 'make'"
			return 1
		fi
		
		MINISHELL="${VALGRIND} ${MEMCHECK} ${LOG_VALGRIND}${LOG_FILE}${ID}.vlgrnd ./Minishell"

		# EXÉCUTION DU TEST AVEC BASH (RÉFÉRENCE)
		bash --posix -c	"${TEST[i]}"				> ${OUTFILE_BASH} 2>&1
		BASH_EXIT_STATUS=$?    # Capture le code de retour de bash

		# EXÉCUTION DU TEST AVEC MINISHELL
		${MINISHELL}	"${TEST[i]}"				> ${OUTFILE_MINISHELL} 2>&1
		MINISHELL_EXIT_STATUS=$?    # Capture le code de retour de minishell

		# TRAITEMENT SPÉCIAL POUR CERTAINS TYPES DE TESTS
		if [ "${PART}" = "SIGNAL" ]; then
			break    # Les tests de signaux nécessitent une interaction manuelle
		elif [ "${PART}" = "HEREDOC" ]; then
			break    # Les here-documents ne sont pas encore implémentés
		elif [ "${PART}" = "ASSIGNMENT" ]; then
			break    # Les assignations ne sont pas testées actuellement
		else
			# NETTOYAGE DE LA SORTIE DE BASH POUR LA COMPARAISON
			# Bash ajoute des informations de contexte (numéro de ligne, etc.) 
			# qu'il faut normaliser pour une comparaison équitable
			
			# Traitement des erreurs de syntaxe
			if grep -q "syntax error" ${OUTFILE_BASH}; then
				if grep -q "line 0" ${OUTFILE_BASH}; then
					sed -i 's/ -c: line 0://g' ${OUTFILE_BASH}    # Supprime "line 0"
				elif grep -q "line 1" ${OUTFILE_BASH}; then
					sed -i 's/ -c: line 1://g' ${OUTFILE_BASH}    # Supprime "line 1"
				fi
				# Ne garde que la première ligne du message d'erreur
				head --lines=1 ${OUTFILE_BASH} > ${OUTFILE_TMP}
				cat ${OUTFILE_TMP} > ${OUTFILE_BASH}
				rm ${OUTFILE_TMP}
			fi

			# Traitement des erreurs "command not found"
			if grep -q "command not found" ${OUTFILE_BASH}; then
				if grep -q "line 0" ${OUTFILE_BASH}; then
					sed -i 's/bash: line 0://g' ${OUTFILE_BASH}
				elif grep -q "line 1" ${OUTFILE_BASH}; then
					sed -i 's/bash: line 1: //g' ${OUTFILE_BASH}
				else
					sed -i 's/bash: //g' ${OUTFILE_BASH}
				fi
				head --lines=1 ${OUTFILE_BASH} > ${OUTFILE_TMP}
				cat ${OUTFILE_TMP} > ${OUTFILE_BASH}
				rm ${OUTFILE_TMP}
			fi

			# Traitement des erreurs "No such file or directory"
			if grep -q "No such file or directory" ${OUTFILE_BASH}; then
				if grep -q "line 0" ${OUTFILE_BASH}; then
					sed -i 's/line 0: //g' ${OUTFILE_BASH}
				elif grep -q "line 1" ${OUTFILE_BASH}; then
					sed -i 's/line 1: //g' ${OUTFILE_BASH}
				fi
				head --lines=1 ${OUTFILE_BASH} > ${OUTFILE_TMP}
				cat ${OUTFILE_TMP} > ${OUTFILE_BASH}
				rm ${OUTFILE_TMP}
			fi

			# Traitement des erreurs "too many arguments"
			if grep -q "too many arguments" ${OUTFILE_BASH}; then
				if grep -q "line 0" ${OUTFILE_BASH}; then
					sed -i 's/line 0://g' ${OUTFILE_BASH}
				elif grep -q "line 1" ${OUTFILE_BASH}; then
					sed -i 's/line 1: //g' ${OUTFILE_BASH}
				fi
				head --lines=1 ${OUTFILE_BASH} > ${OUTFILE_TMP}
				cat ${OUTFILE_TMP} > ${OUTFILE_BASH}
				rm ${OUTFILE_TMP}
			fi

			# COMPARAISON DES SORTIES STANDARD ET D'ERREUR
			echo -e 		"\n\t-- RETURN VALUE --"		>> "${LOG_FILE}${ID}.txt"
			if diff ${OUTFILE_BASH} ${OUTFILE_MINISHELL}	>> "${LOG_FILE}${ID}.txt"; then
				# Les sorties sont identiques : succès
				echo -ne	"${BOLD}${GREEN}[${ID}.OK]${DEFAULT}"
				cat			< ${OUTFILE_MINISHELL}				>> "${LOG_FILE}${ID}.txt"
				echo		"---->[OK]"							>> "${LOG_FILE}${ID}.txt"
			else
				# Les sorties diffèrent : échec
				echo -ne	"${BOLD}${RED}[${ID}.KO]${DEFAULT}"
				echo		"---->[KO]"							>> "${LOG_FILE}${ID}.txt"
			fi

			# COMPARAISON DES CODES DE RETOUR
			echo -e 	"\n\t-- EXIT STATUS --"					>> "${LOG_FILE}${ID}.txt"
			echo		"MINISHELL=${MINISHELL_EXIT_STATUS}"	>> "${LOG_FILE}${ID}.txt"
			echo		"BASH=${BASH_EXIT_STATUS}"				>> "${LOG_FILE}${ID}.txt"
			if [ ${MINISHELL_EXIT_STATUS} = ${BASH_EXIT_STATUS} ]; then
				# Les codes de retour sont identiques : succès
				echo -ne	"${BOLD}${CYAN}[E.OK]${DEFAULT}"
				echo		"---->[OK]"							>> "${LOG_FILE}${ID}.txt"
			else
				# Les codes de retour diffèrent : échec
				echo -ne	"${BOLD}${MAGENTA}[E.KO]${DEFAULT}"
				echo		"---->[KO]"							>> "${LOG_FILE}${ID}.txt"
			fi

			# ANALYSE DU RAPPORT VALGRIND POUR LES FUITES MÉMOIRE
			ERROR=false
			while IFS= read -r line; do
    			if [[ "$line" == *"ERROR SUMMARY"* ]]; then
        			if [[ "$line" != *"0 errors"* ]]; then
						ERROR=true    # Des erreurs mémoire ont été détectées
            			break
        			fi
    			fi
			done < "${LOG_FILE}${ID}.vlgrnd"
			if [ $ERROR == true ]; then
				echo -ne "${BOLD}${YELLOW}[V.KO] "${DEFAULT}  # Erreurs Valgrind
			else
				echo -ne "${BOLD}${BLUE}[V.OK] "${DEFAULT}   # Pas d'erreurs Valgrind
			fi

			# AFFICHAGE FINAL : commande testée
			echo 	"${TEST[i]} "
			
			# AJOUT DES INFORMATIONS VALGRIND AU LOG
			echo -e "\n\t-- VALGRIND --"	>> "${LOG_FILE}${ID}.txt"
			cat "${LOG_FILE}${ID}.vlgrnd"	>> "${LOG_FILE}${ID}.txt"
			rm	"${LOG_FILE}${ID}.vlgrnd"   # Supprime le fichier temporaire Valgrind
		fi

		# PASSAGE AU TEST SUIVANT
		((ID++))    # Incrémente le numéro de test
		((i++))     # Incrémente le compteur de boucle
	done
	
	# AFFICHAGE D'UNE LIGNE VIDE SI DES TESTS ONT ÉTÉ EXÉCUTÉS
	if [[ len -gt 0 ]]; then
		echo
	fi

	# NETTOYAGE DES FICHIERS TEMPORAIRES
	if [ -f ${OUTFILE_BASH} ]; then
		rm ${OUTFILE_BASH}
	fi
	if [ -f ${OUTFILE_MINISHELL} ]; then
		rm ${OUTFILE_MINISHELL}
	fi
}

# POINT D'ENTRÉE DU SCRIPT
# Lance la fonction main avec tous les arguments passés au script
main $@