#!/bin/bash

# ERROR
SEGFAULT=139

LOG_DIR="test/log"
# VALGRIND
VALGRIND="valgrind"
MEMCHECK="--leak-check=full --show-leak-kinds=all --track-origins=yes 
			--trace-children=yes --log-file=${LOG_DIR}/memcheck.txt"
CACHEGRIND="--tool=cachegrind --log-file=log/valgrind/cachegrind.txt"
CALLGRIND="--tool=callgrind --log-file=log/valgrind/callgrind.txt"
HELGRIND="--tool=helgrind --log-file=log/valgrind/helgrind.txt"
DRD="--tool=drd --log-file=log/valgrind/drd.txt"
MASSIF="--tool=massif --log-file=log/valgrind/massif.txt"
DHAT="--tool=dhat --log-file=log/valgrind/dhat.txt"

create_fsanitize_log()
{
	mkdir -p log/fsanitize
}

create_valgrind_log()
{
	mkdir -p log/valgrind
	> log/valgrind/memcheck.txt
	> log/valgrind/helgrind.txt
	> log/valgrind/cachegrind.txt
	> log/valgrind/callgrind.txt
	> log/valgrind/drd.txt
	> log/valgrind/massif.txt
	> log/valgrind/dhat.txt
}
