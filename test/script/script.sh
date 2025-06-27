#!/bin/bash

# INCLUDE
source ./colors.sh

# VARIABLE
declare	PART

declare VALGRIND="valgrind"
declare MEMCHECK="--leak-check=full --show-leak-kinds=all --track-origins=yes\
					--trace-children=yes"

declare LOG_VALGRIND="--log-file="
declare LOG_FILE

declare FILE_BINARY="test/file/binary"
declare FILE_EMPTY="test/file/empty.txt"
declare FILE_INFILE1="test/file/infile1.txt"
declare FILE_INFILE2="test/file/infile2.txt"
declare FILE_REGULAR="test/file/regular.txt"
declare FILE_OUTFILE1="test/file/outfile1.txt"
declare FILE_OUTFILE2="test/file/outfile2.txt"
declare FILE_NO_PERMISSION="test/file/no_permission.txt"

declare	OUTFILE_TMP="test/log/outfile_tmp.txt"
declare	OUTFILE_BASH="test/log/outfile_bash.txt"
declare	OUTFILE_MINISHELL="test/log/outfile_minishell.txt"

declare BUILTIN=false


main()
{
	pushd ../.. > /dev/null
	if make > /dev/null; then
		# clear
		# print_header
		init_tester
		if [ $# -gt 0 ]; then
			if [ "$1" = "builtin" ]; then
				test_builtin
			elif [ "$1" = "simple_command" ]; then
				test_simple_command
			elif [ "$1" = "redirection" ]; then
				test_redirection
			elif [ "$1" = "expansion" ]; then
				test_expansion
			elif [ "$1" = "heredoc" ]; then
				test_heredoc
			elif [ "$1" = "quote" ]; then
				test_quote
			elif [ "$1" = "pipeline" ]; then
				test_pipeline
			elif [ "$1" = "signal" ]; then
				test_signal
			elif [ "$1" = "list" ]; then
				test_list
			elif [ "$1" = "wildcard" ]; then
				test_wildcard
			elif [ "$1" = "evaluation" ]; then
				test_evaluation
			fi
			exit 0
		fi
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
	popd > /dev/null
	echo
	exit 0
}

print_header()
{
	echo -e "${BOLD}${WHITE}\t\t############################${DEFAULT}"
	echo -e "${BOLD}${WHITE}\t\t##     MINISHELL TEST     ##${DEFAULT}"
	echo -e "${BOLD}${WHITE}\t\t############################\n${DEFAULT}"
}

init_tester()
{
	if [ -d "test/file" ] || [ -d "test/log" ]; then
		rm -rf	test/file test/log
	fi
	mkdir	test/file test/log

	echo "infile1"			> ${FILE_INFILE1}
	echo "infile2"			> ${FILE_INFILE2}
	echo "This is a test"	> ${FILE_REGULAR}

	if [ -f ${FILE_OUTFILE1} ]; then
		rm ${FILE_OUTFILE1}
	fi
	if [ -f ${FILE_OUTFILE2} ]; then
		rm ${FILE_OUTFILE2}
	fi
	touch ${FILE_EMPTY} ${LOG_FILE}
	touch ${FILE_BINARY}		&& chmod 100 ${FILE_BINARY}
	touch ${FILE_NO_PERMISSION}	&& chmod 000 ${FILE_NO_PERMISSION}
}

test_evaluation()
{
	local -a TEST
	PART="EVALUATION"
	mkdir test/log/evaluation
	LOG_FILE="test/log/evaluation/"

	TEST=(
		""
		"  "
		"	"
		"echo -n"
		"/bin/ls"
		"echo \"cat lol.c | cat > lol.c\""
	)
	launch_test "${TEST[@]}"
}

test_simple_command()
{
	local -a TEST
	PART="SIMPLE COMMAND"
	mkdir test/log/simple_command
	LOG_FILE="test/log/simple_command/"

	TEST=(
		""
		"   "
		"ls"
		"dir"
		"ls -l"
		"dir -a"
		"/bin/ls"
		"/bin/dir"
		"/bin/ls -l"
		"/bin/dir -a"
		# "ls wrong_opt"
		# "dir wrong_opt"
		"/bin/ls wrong_opt"
		"/bin/dir wrong_opt"
		"wrong_cmd"
		"wrong_cmd ls"
		"wrong_cmd wrong_opt"
		"wrong_cmd wrong_opt1 wrong_opt2"
	)
	launch_test "${TEST[@]}"
}

test_redirection()
{
	local -a TEST
	PART="REDIRECTION"
	mkdir test/log/redirection
	LOG_FILE="test/log/redirection/"

	TEST=(
		"<"
		">"
		"<<"
		">>"
		# "<>"	bash specific operator
		# "<>>"	bash: syntax error near unexpected token `>'
		# "<<<"	bash specific operator
		"><"
		"><<"
		">>>"
		"<<>"
		"<<>>"
		">><"
		">><<"
		"cat<${FILE_INFILE1}"
		"cat < ${FILE_INFILE1}"
		"cat < ${FILE_INFILE1} < ${FILE_INFILE1} < ${FILE_INFILE1}"
		# "cat < ${FILE_NO_PERMISSIOn}"
		# "cat < ${FILE_BINARY}"
		"cat<wrong_file"
		"cat < wrong_file"
		"cat < wrong_file1 < wrong_file2"
		"cat < ${FILE_INFILE1} -n"
		"cat<${FILE_INFILE1}-n"
		# "cat < ${FILE_INFILE1} wrong_opt"
		"cat < ${FILE_INFILE1} < ${FILE_INFILE2}"
		# "< ${FILE_INFILE1} cat < ${FILE_INFILE2} wrong_opt"
		"< ${FILE_INFILE1} cat"
		"< ${FILE_INFILE1} cat -n"
		"< ${FILE_INFILE1} cat < ${FILE_INFILE2}"
		"wrong_cmd < ${FILE_INFILE1}"
		"wrong_cmd < ${FILE_INFILE1} < ${FILE_INFILE2}"
		"< ${FILE_INFILE1} wrong_cmd"
		"< ${FILE_INFILE1} wrong_cmd < ${FILE_INFILE2}"
		"ls > ${FILE_OUTFILE1}"
		"ls > ${FILE_OUTFILE1} > ${FILE_OUTFILE2}"
		"wrong_cmd > ${FILE_OUTFILE1}"
		"wrong_cmd > ${FILE_OUTFILE1} > ${FILE_OUTFILE2}"
	)
	launch_test "${TEST[@]}"
}

test_pipeline()
{
	local -a TEST
	PART="PIPELINE"
	mkdir test/log/pipeline
	LOG_FILE="test/log/pipeline/"

	TEST=(
		"|"
		"ls | wc"
		"ls | cat"
		"ls | wrong_cmd"
		"wrong_cmd | ls"
		"wrong_cmd | wrong_cmd"
		"ls | wc | ls | wc | ls"
		"ls | wrong_cmd | ls | wc | ls"
		"ls | wc | ls | wrong_cmd | ls"
		"ls | wrong_cmd | ls | wrong_cmd | ls"
		"ls | sleep 2 | cat"
		"cat < ${FILE_INFILE1} | cat < ${FILE_INFILE2}"
		"cat < ${FILE_INFILE1} > ${FILE_OUTFILE1} | cat < ${FILE_INFILE2} > ${FILE_OUTFILE1}"
	)
	launch_test "${TEST[@]}"
}

test_builtin()
{
	local -a TEST
	PART="BUILTIN"
	mkdir test/log/builtin
	LOG_FILE="test/log/builtin/"
	launch_test

	BUILTIN=true
	PART="PIPELINE"
	mkdir test/log/builtin/pipeline
	LOG_FILE="test/log/builtin/pipeline/"
	TEST=(
		"cd | cd"
		"echo | echo"
		"env | env"
		"env | wc -l"
		"env | grep HOME"
		"env | echo"
		"exit | exit"
		"export | export"
		"pwd | pwd"
		"unset |unset"
	)
	launch_test "${TEST[@]}"

	PART="CD"
	mkdir test/log/builtin/cd
	LOG_FILE="test/log/builtin/cd/"
	TEST=(
		"cd"
		"cd ''"
		"cd \"\""
		"cd /"
		"cd ."
		"cd .."
		"cd ~"
		"cd @"
		"cd -"
		"cd _"
		"cd minishell"
		"cd wrong_dir"
		"cd /dev/tty"
		"cd /dev/null"
	)
	launch_test "${TEST[@]}"

	PART="ECHO"
	mkdir test/log/builtin/echo
	LOG_FILE="test/log/builtin/echo/"
	TEST=(
		"echo"
		"echo ''"
		"echo \"\""
		"echo -"
		"echo -n"
		"echo \n"
		"echo -n \n"
		"echo --"
		"echo -- \n"
		"echo -nn"
		"echo --n"
		"echo --nn"
		"echo -n-n"
		"echo -n-n \n"
		"echo -n-n test\n"
		"echo -n -n"
		"echo -n -n \n"
		"echo -n -n test\n"
		"echo -n -n -n"
		"echo -n -n -n \n"
		"echo -n -n -n test\n"
		"echo -a"
		'echo $HOME'
		'echo $USER'
		'echo $wrong_expan'
		'echo $HOME $USER'
		'echo $HOME$USER'
		'echo $HOME$wrong_expan$USER'
	)
	launch_test "${TEST[@]}"

	PART="ENV"
	mkdir test/log/builtin/env
	LOG_FILE="test/log/builtin/env/"
	TEST=(
		"env"
		"env wrong_opt"
		'env HOME'
		'env $HOME'
	)
	launch_test "${TEST[@]}"

	PART="EXIT"
	mkdir test/log/builtin/exit
	LOG_FILE="test/log/builtin/exit/"
	TEST=(
		"exit"
		"exit \"\""
		"exit ''"
		"exit -"
		"exit --"
		"exit ++"
		"exit 0"
		"exit -0"
		"exit +0"
		"exit 0a"
		"exit a0"
		"exit 1"
		"exit -1"
		"exit +1"
		"exit 256"
		"exit -256"
		"exit +256"
		"exit 2a56"
		"exit 256a"
		"exit non_numeric"
		"exit @"
		"exit 1 1"
		"exit a 1"
		"exit 1 1 1 1 1 1"
		"exit 1 1 a 1 a 1"
		"exit 2147483647"
		"exit -2147483647"
		"exit +2147483647"
		"exit 2147483648"
		"exit -2147483648"
		"exit +2147483648"
		"exit 4294967295"
		"exit -4294967295"
		"exit +4294967295"
		"exit 4294967296"
		"exit -4294967296"
		"exit +4294967296"
		"exit 9223372036854775807"
		"exit +9223372036854775807"
		"exit -9223372036854775807"
	)
	launch_test "${TEST[@]}"

	PART="EXPORT"
	mkdir test/log/builtin/export
	LOG_FILE="test/log/builtin/export/"
	TEST=(
		"export"
		"export \"\""
		"export ''"
		"export ="
		"export @"
		"export ."
		"export .."
		"export /"
		"export 0"
		"export 0 1 2"
		"export wrong_name"
		"export wrong_name wrong_name"

	)
	launch_test "${TEST[@]}"

	PART="PWD"
	mkdir test/log/builtin/pwd
	LOG_FILE="test/log/builtin/pwd/"
	TEST=(
		"pwd"
		"pwd -"
		'pwd $'
		'pwd $HOME'
		"pwd pwd"
		"pwd wrong_opt"
	)
	launch_test "${TEST[@]}"

	PART="UNSET"
	mkdir test/log/builtin/unset
	LOG_FILE="test/log/builtin/unset/"
	TEST=(
		"unset"
		"unset -"
		"unset _"
		"unset 1"
		"unset @"
		"unset wrong_opt"
		'unset wrong_opt $wrong_expan'
	)
	launch_test "${TEST[@]}"
	BUILTIN=false
}

test_heredoc()
{
	local -a TEST
	PART="HEREDOC"
	mkdir test/log/heredoc
	LOG_FILE="test/log/heredoc/"

	# TEST=(
	# 	"ls << eof"
	# )
	launch_test "${TEST[@]}"
}

test_quote()
{
	local -a TEST
	PART="QUOTE"
	mkdir test/log/quote
	LOG_FILE="test/log/quote/"

	TEST=(
		"\"\""
		"''"
		"'\"\"'"
		"\"''\"\"''\""
		"\"'\"\"\"\"'\""
		"\"'\"\"'\""
		"'\"''\"'"
		"\"ls\""
		"'ls'"
		"\"ls -l\""
		"'ls -l'"
		"\"ls | ls | ls | ls\""
		"\"ls '|' ls | ls | ls\""
		"\"ls | ls | ls '|' ls\""
		# "\"< ${FILE_INFILE1}\""
		"\"< infile\""
		"\"< infile | < infile | < infile\""
	)
	launch_test "${TEST[@]}"
}

test_expansion()
{
	local -a TEST
	PART="EXPANSION"
	mkdir test/log/expansion
	LOG_FILE="test/log/expansion/"

	TEST=(
		'$'
		'$?'
		'$HOME'
		'$PATH'
		'$wrong_expan'
		'$wrong_expan ls'
		'ls $wrong_expan'
		'wrong_cmd $wrong_expan'
		'$wrong_expan wrong_cmd'
	)
	launch_test "${TEST[@]}"
}

test_signal()
{
	local -a TEST
	PART="SIGNAL"

	# Ctrl-C
	# Ctrl-D
	# Ctrl-\
	# ./minishell &
	# sleep 1
	# echo $!
	# kill $!
	# echo $?
	launch_test "${TEST[@]}"
}

test_list()
{
	local -a TEST
	PART="LIST"

	TEST=(
		")"
		"()"
		"&&"
		"||"
		"echo a && echo b"
		"echo a || echo b"
	)
	launch_test "${TEST[@]}"
}

test_wildcard()
{
	local -a TEST
	PART="WILDCARD"

	TEST=(
		"*"
	)
	launch_test "${TEST[@]}"
}

launch_test()
{
	local i=0
	local ID=1
	local ERROR
	local len=${#TEST[@]}
	local -a TEST
	local BASH_EXIT_STATUS
	local MINISHELL_EXIT_STATUS

	TEST=("$@")
	if [ $BUILTIN == true ]; then
		echo -e "${PART}"
	else
		echo -e "${BOLD}${WHITE}${PART}${DEFAULT}"
	fi
	while [[ i -lt len ]]; do
		echo -e 		"\t\t\t\t\t### ${PART} ###" >	"${LOG_FILE}${ID}.txt"
		echo -e			"\n\t-- Test[${ID}] --\n"	>>	"${LOG_FILE}${ID}.txt"
		echo			"INPUT=${TEST[i]}"			>>	"${LOG_FILE}${ID}.txt"

		MINISHELL="${VALGRIND} ${MEMCHECK} ${LOG_VALGRIND}${LOG_FILE}${ID}.vlgrnd ./Minishell"

		bash -c			"${TEST[i]}"				> ${OUTFILE_BASH} 2>&1
		BASH_EXIT_STATUS=$?

		${MINISHELL}	"${TEST[i]}"				> ${OUTFILE_MINISHELL} 2>&1
		MINISHELL_EXIT_STATUS=$?

		if [ "${PART}" = "SIGNAL" ]; then
			break
		elif [ "${PART}" = "HEREDOC" ]; then
			break
		elif [ "${PART}" = "ASSIGNMENT" ]; then
			break
		else
			if grep -q "syntax error" ${OUTFILE_BASH}; then
				if grep -q "line 0" ${OUTFILE_BASH}; then
					sed -i 's/ -c: line 0://g' ${OUTFILE_BASH}
				elif grep -q "line 1" ${OUTFILE_BASH}; then
					sed -i 's/ -c: line 1://g' ${OUTFILE_BASH}
				fi
				head --lines=1 ${OUTFILE_BASH} > ${OUTFILE_TMP}
				cat ${OUTFILE_TMP} > ${OUTFILE_BASH}
				rm ${OUTFILE_TMP}
			fi

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

			echo -e 		"\n\t-- RETURN VALUE --"		>> "${LOG_FILE}${ID}.txt"
			if diff ${OUTFILE_BASH} ${OUTFILE_MINISHELL}	>> "${LOG_FILE}${ID}.txt"; then
				echo -ne	"${BOLD}${GREEN}[${ID}.OK]${DEFAULT}"
				cat			< ${OUTFILE_MINISHELL}				>> "${LOG_FILE}${ID}.txt"
				echo		"---->[OK]"							>> "${LOG_FILE}${ID}.txt"
			else
				echo -ne	"${BOLD}${RED}[${ID}.KO]${DEFAULT}"
				echo		"---->[KO]"							>> "${LOG_FILE}${ID}.txt"
			fi

			echo -e 	"\n\t-- EXIT STATUS --"					>> "${LOG_FILE}${ID}.txt"
			echo		"MINISHELL=${MINISHELL_EXIT_STATUS}"	>> "${LOG_FILE}${ID}.txt"
			echo		"BASH=${BASH_EXIT_STATUS}"				>> "${LOG_FILE}${ID}.txt"
			if [ ${MINISHELL_EXIT_STATUS} = ${BASH_EXIT_STATUS} ]; then
				echo -ne	"${BOLD}${CYAN}[E.OK]${DEFAULT}"
				echo		"---->[OK]"							>> "${LOG_FILE}${ID}.txt"
			else
				echo -ne	"${BOLD}${MAGENTA}[E.KO]${DEFAULT}"
				echo		"---->[KO]"							>> "${LOG_FILE}${ID}.txt"
			fi

			ERROR=false
			while IFS= read -r line; do
    			if [[ "$line" == *"ERROR SUMMARY"* ]]; then
        			if [[ "$line" != *"0 errors"* ]]; then
						ERROR=true
            			break
        			fi
    			fi
			done < "${LOG_FILE}${ID}.vlgrnd"
			if [ $ERROR == true ]; then
				echo -ne "${BOLD}${YELLOW}[V.KO] "${DEFAULT}
			else
				echo -ne "${BOLD}${BLUE}[V.OK] "${DEFAULT}
			fi

			echo 	"${TEST[i]} "
			echo -e "\n\t-- VALGRIND --"	>> "${LOG_FILE}${ID}.txt"
			cat "${LOG_FILE}${ID}.vlgrnd"	>> "${LOG_FILE}${ID}.txt"
			rm	"${LOG_FILE}${ID}.vlgrnd"
		fi

		((ID++))
		((i++))
	done
	if [[ len -gt 0 ]]; then
		echo
	fi

	if [ -f ${OUTFILE_BASH} ]; then
		rm ${OUTFILE_BASH}
	fi
	if [ -f ${OUTFILE_MINISHELL} ]; then
		rm ${OUTFILE_MINISHELL}
	fi
}

main $@