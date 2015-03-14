#Detailed description of the proposed project.

# Introduction #

The proposed project is to create a program that will produce as output programs that consistently win the game of minesweeper. The main challenge of this project will be to randomly create a population of programs that can play minesweeper with varying degrees of success. Then, using principles of genetic algorithms, test, mutate, and recombine the initial population iteratively until a successful minesweeper program results.

# Requirements #

The project will require some basic components to function. They are as follows:

## Game Board ##

First will be a version of minesweeper that can be interfaced by our generated program population. Essentially it will be a user-less game of minesweeper that can be queried for the state of its game board. As well as accept moves from the virtual machine. The result of said queries will be machine readable representations of the playing state of the bored, (i.e. what numbers are on what squares and what squares are still available to be picked.) This version will be able to be graphically displayable, but not necessarily so. More importantly, multiple instances will run simultaneously in order to test multiple individuals from the population at the same time. Boards will have the options of varying difficulty and size. Also an option to randomize the board will be available (defaulted to on, removable for testing) Upon completion of the game, boards will return metrics to be used to evaluate the success of their 'player' program.

The game board will either come from the KDE Minesweeper source, be built from scratch, or some combination of the both.

## Virtual Machine ##

Next will be a virtual machine on which minesweeper related code can be run. Rather than have our main program create code requiring compilation, we will build a virtual engine that will encompass the breadth and depth of all minesweeper related commands. This will include the following:

  * variable assignment
  * arrays
  * conditional testing (if / then / else )
  * looping (for / do / while )
  * querying the board for its state
  * game board creation
  * game board options
  * win / loss conditions
  * performance metric standardization

The virtual machine will be the central entity connecting the test population with the game boards as well as with the main program. Furthermore the main program will be using the library of tools associated with the minesweeper game found in virtual machine for creating the test population. (i.e minesweeper commands)

The virtual machine will be developed from scratch.

# Methodology #

This program has the possibility of having a very open scope. Minesweeper was chosen as a starting point to create the test population generator as well as test the genetic iteration tester. With that said, it is possible to replace minesweeper as the chosen game with other games as long as they have an interface compatible with the main program. Therefore one of the goals of this project is to separate all minesweeper specific code from the main program and keep it instead in a virtual machine plug-in. This will allow the main program to create test populations for other games provided that they have a compatible virtual machine interface.

An extension of this line of thought will be an overall attempt to modularize the individual components (game, vm, and main) to allow for easy updates/adaptations as well as smooth parallel development.

# Main Program #

The goal of the main program is to generate code specifically capable of playing the game of minesweeper. These pieces of code do not need to beat the game of minesweeper, but they do need to play the game in a deterministic fashion. Each test case created by the main program will be tested against a board via the virtual machine and evaluated based on how well it played and or how fast it solved the board. The more successful test cases will be kept, cross bred with other cases, mutated and then retested until satisfactory speed and completion rates are achieved.

From the main program options for population size, mutation rates, and crossover rates will be present along with game options queried from the virtual machine. In the case of minesweeper this will take the form of board size and game difficulty. Other options will involve the visualization of the game boards (which are hidden, which aren't) and progress/population statistics (most successful, population size, success rate, etc.)

[Prospectus](Prospectus.md)
[Home](Home.md)