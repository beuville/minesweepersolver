Completed By: 11/17/08

# Components #

  * Configuration files / file formats
  * Population
  * Iteration

# Goals #

The goal of the third milestone is to have the bulk of the main program complete. With the virtual machine completed in the second milestone, the work remaining revolves around creating populations of game-playing code and iterating their 'evolution' via mutations, crossovers and prunings.

## Configuration files / file formats ##

Supervisor: Beau Furrow

The first step to developing the main program will be to create two configuration files. These files will provide syntax information to the main program for the programming language of the virtual machine, and the game specific commands of the actual minesweeper game. This allows for total logical separation between the main program and the game itself.

## Population ##

Supervisor: Cris Tarr

The second phase of the third milestone will be to use the information from the two config. files to create a random population of programs for the virtual machine to test. These programs must be able to 'play' the game of minesweeper in a deterministic fashion, but still maintain a random structure.

## Iteration ##

Supervisor: Mike Stowe

The final portion of the main program will entail the addition of the genetic algorithms used to iterate the program. These algorithms will prune, mutate and/or crossover the population based on the results of their test runs in the virtual machine. Rates and thresholds for prunes, mutations, and crossovers, along with other options, will be controlled via the main programs user interface.

[Post Milestones](postMilestone.md)

[Description](DDDescription.md) [Design Document](DesignDocument.md) [Home](Home.md)