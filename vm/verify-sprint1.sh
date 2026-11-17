#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
test -f /etc/alpine-release || { echo "Run this inside Alpine Linux." >&2; exit 1; }
for tool in gcc make git gdb valgrind; do command -v "$tool" >/dev/null; done
logs="Testing/evidence/$(date -u +%Y%m%dT%H%M%SZ)-$$"
mkdir -p "$logs"
printf 'Evidence folder: %s\n' "$logs"
{
    git rev-parse HEAD
    git status --short
    cat /etc/alpine-release
    for tool in gcc make gdb valgrind; do "$tool" --version; done
} >"$logs/environment.txt"
make clean >"$logs/build.txt" 2>&1
make CFLAGS='-std=c99 -g -Wall -Wextra -Wpedantic -Werror -Iinclude' >>"$logs/build.txt" 2>&1
make test >"$logs/board-tests.txt" 2>&1
awk 'BEGIN {
    print 1
    for (r=0; r<5; r++) for (c=0; c<5; c++) print r,c,r,c+1
    for (r=0; r<4; r++) for (c=0; c<6; c++) print r,c,r+1,c
    print 3
}' >"$logs/match-input.txt"
timeout 30 ./dots_and_boxes <"$logs/match-input.txt" >"$logs/match.txt"
grep -F "Game Over! It's a tie (10 to 10)!" "$logs/match.txt"
test "$(grep -c "turn. Enter" "$logs/match.txt")" -eq 49
gdb -q -batch -ex 'set pagination off' -ex 'break board_apply' -ex run \
    -ex 'print move' -ex 'print *board' -ex backtrace -ex 'disable 1' -ex continue \
    --args ./build/test_board >"$logs/gdb.txt" 2>&1
grep -Eq 'hit Breakpoint|Breakpoint 1, board_apply' "$logs/gdb.txt"
grep -q 'ALL BOARD TESTS PASSED' "$logs/gdb.txt"
make valgrind >"$logs/valgrind-board.txt" 2>&1
timeout 120 valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
    --error-exitcode=1 --log-file="$logs/valgrind-game.txt" ./dots_and_boxes \
    <"$logs/match-input.txt" >"$logs/valgrind-match.txt"
grep -F "Game Over! It's a tie (10 to 10)!" "$logs/valgrind-match.txt"
printf 'PASS: build, board tests, full match, GDB and Valgrind\n' | tee "$logs/summary.txt"
