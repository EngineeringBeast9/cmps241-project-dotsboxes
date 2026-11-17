# Sprint 1 testing report and actions for Ali

Repository: [EngineeringBeast9/cmps241-project-dotsboxes](https://github.com/EngineeringBeast9/cmps241-project-dotsboxes)  
Reviewed revision: [877b8e3](https://github.com/EngineeringBeast9/cmps241-project-dotsboxes/commit/877b8e3f64045ed2803b5c94fe7d619d80c773ce)

## Findings and fixes

| Check | Result on the reviewed revision | What Ali should do |
|---|---|---|
| Clean build | Blocked. The Makefile uses `-Isrc` and lists the missing `src/game.c`. The console code also calls an older board API. | Change the include path to `-Iinclude`. Remove `src/game.c` from the source list and remove the obsolete `game.h` includes. Connect the game loop to `board_init`, `board_apply`, `board_finished`, and `Board.current`. Update the display to use the current edge API, `Board.owner`, and `Board.score`. |
| End-of-input | Code review found that both prompt loops retry after EOF. | Return an EOF result from the input functions and handle it in the menu and game loop. |
| Invalid and repeated moves | Full console check blocked by the build. | Read one line per prompt. Reject malformed, extra, out-of-range, diagonal, and repeated input. Use the existing board API for move validation and keep the current turn after an invalid move. |
| Board rules | The existing board tests passed locally with 30,188 checks and zero failures. | Use Malek's board logic for scoring and turn changes. Verify the same rules through the console after integration. |

## Retest before pushing

Run `make clean && make`, then `./dots_and_boxes`. Record the actual result for each check.

- [ ] The build completes and the command in the README launches the game.
- [ ] The board shows 4 × 5 boxes. The board and scores update after each valid move.
- [ ] Turn order alternates after moves without a capture. A capture shows the player's initial, adds a point, and gives that player another turn.
- [ ] Malformed, out-of-range, diagonal, and duplicate moves are rejected without changing the board or the turn.
- [ ] EOF at the menu or during a move ends the program without repeated prompts.
- [ ] A full two-player game ends after all 20 boxes are claimed and declares the correct winner or tie.

## Handoff

Push the tested fixes to GitHub and confirm they are on `main`. Send Malek the commit SHA and the retest results. Malek will verify that revision in Alpine, collect GDB and Valgrind logs, and check that the VM boots straight into the game.
