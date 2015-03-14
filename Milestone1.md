Completed by: 10/6/08

# Components #

  * Game board
  * Game interface
  * Programming language definition

# Goals #

The goal of the first milestone is to make all necessary preparations to begin work on the virtual machine. The virtual machine is one of the technical achievements for the project and requires some foundation before work on it can be started.

## Game board ##

Supervisor: Cris Tarr

The game board essentially is the game of minesweeper. In the interest of smoother integration we have opted to develop our own minesweeper game. This will allow for easy integration with our virtual machine. The actions allowed by the game board (i.e selecting squares, winning, losing) will be directly controlled by the game interface.

## Game interface ##

Supervisor: Beau Furrow

The game interface acts as a medium between the virtual machine and the actual game. The game interface will have 'knowledge' of all game-specific details via a text/xml configuration file.  Such details include the size of the board, the difficult, the number of mines, etc. This configuration file does not necessarily need to be built or defined until milestone 3, rather commands will be build into the interface to allow for testing of the board and program parser. The configuration file basically serves to separate the main program from the minesweeper game by providing a list of commands and settings for the minesweeper game interface.

## Programming language definition ##

Supervisor: Mike Stowe

The virtual machine will test the code of each member of the its population against game of minesweeper via a program parser, however the parser needs a detailed description of the programming language being used in order to interpret commands. Therefore, before the parser can be build and the virtual machine assembled, a detailed definition of the vm's programming language must be written. This includes the languages structures (i.e. symbol table) and syntax's (variables, conditionals, and loops)

[Milestone2](Milestone2.md)

[Description](DDDescription.md) [Design Document](DesignDocument.md) [Home](Home.md)