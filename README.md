# Dots and Boxes

A command-line implementation of the classic Dots and Boxes game, written in C (C99) for Alpine Linux. Developed as the CMPS 241 (Software Construction) course project.

## Features

- **Sprint 1:** Two-player local game on a 5 × 6 grid of dots (4 × 5 boxes), with input validation, scoring, and winner or tie detection.
- **Sprint 2 (planned):** Player vs. Bot mode with a layered strategy: greedy captures, safe moves, and endgame chain handling, with optional minimax and alpha-beta pruning.
- **Sprint 3 (optional):** Network play over TCP and a receiver thread with mutex-protected board state.

## Requirements

- GCC with C99 support
- GNU Make
- GDB and Valgrind (for debugging)
- Alpine Linux (inside a VM, for the target environment)

## Build

```bash
make
```

Clean build artifacts:

```bash
make clean
```

## Run

```bash
./dots
```

Moves are entered as four integers, `r1 c1 r2 c2`, for example `0 4 0 3`. Dots are indexed by row (0–4) and column (0–5). Only horizontally or vertically adjacent dots may be connected.

## Debugging

Memory check with Valgrind (the target is zero leaks and zero errors):

```bash
make valgrind
```

Debug with GDB:

```bash
gdb ./dots
```

Session logs are stored in `docs/`.

## Project Structure

```
.
├── Makefile
├── include/     # Header files (board, game, input, display, bot, net)
├── src/         # Source files
├── tests/       # Unit tests
├── vm/          # Alpine VM setup and autostart notes
└── docs/        # Report, slides, and debugging output
```

## Alpine VM Autostart

The game launches automatically when the VM boots, with no login prompt. Setup steps are documented in [`vm/autostart.md`](vm/autostart.md).

## Project Plan

The full sprint schedule, architecture, and task breakdown are in [`plan.md`](plan.md).

## Team

- Malek
- Ali

## Course

CMPS 241, Software Construction, Fall 2026/27
