# Monte Carlo Tree Search

## Implemented by

- Andrea Mascaretti (andrea1.mascaretti@mail.polimi.it)
- Michele Pellegrino (michele2.pellegrino@mail.polimi.it)
*(Source code available on [GitHub](https://github.com/mascaretti/mcts.git))*

## Requirements

- C++11 compiler (tested of g++ and clang)
- MPI (for parallelization)

Optional:
- R (for data analysis and plots)
   ..* tidydiverse package
   ..* forecats package

## Compiling

- Run:
  `$ make [options]`
in the main directory; `DEBUG=yes` (`DEBUG=no`) enables (disable) debugging (with GDB, for instance)
Then:
  `$ cd example`
  `$ make [options]`
to compile the examples scripts, while:
  `$ cd test/[test_name]`
  `$ make [options]`
to compile the testing scripts.

## Running

Run:
  `$ make run [options]`
to run the parallel script; `NP=n_cores` to change the number of processes,
                            `DEFAULT_EXEC=exec` to change the executable to run
In speedup_test run test_gen.sh to run all the tests in the folder.
Otherwise, run:
  `$ ./[executable]`
to execute the serial script.

## Description

- src:        contains the source code and the template classes that implement both the games and
            the search tree
- examples:   contains examples of matches, namely ai vs. random player, ai vs. ai and
            ai vs. human player
- test:       contains various tests concerning data structures (struct_test), games (nim_test
            and oxo_test), parallel speedup (speedup_test) and data analysis (analysis)

Developed and tested on Debian, CentOs and MacOS X
