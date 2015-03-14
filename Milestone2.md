Completed By: 10/27/08

# Components #

  * Program parser
  * Game instance
  * Integration

# Goals #

The goal of the second milestone is to complete the virtual machine and test its components. The virtual machine will run multiple game instances simultaneously testing the entire population. Each game requires a working parser and game interface. All of these components need to be completed and tested in order to assemble the virtual machine.

## Program parser ##

Supervisor: Mike Stowe

The program parser will be responsible for taking a program in text format and translating the commands into their functionality. The parser will be tied to symbol table and any other components of the programming language. Commands recognized by the program parser will be those defined in the language definition only. Any other commands present will be assumed to be game-specific and thus passed to the game interface.

## Game instance ##

Supervisor: Cris Tarr

With the game interface completed from [Milestone1](Milestone1.md) and the parser completed, the game instance can be assembled. The virtual machine will be running multiple game instances simultaneously to test the entire population.

## Integration ##

Supervisor: Beau Furrow

The integration phase of the second milestone revolves around connecting the constituent parts of the game interface with one-another as well as completing the virtual machine by attaching multiple game interfaces to separate, concurrent, processes fulfilling the goal of multi-threading the virtual machine. These processes will need to, in an orderly non-locking fashion, report the results of the games to shared resources that will be eventually reported back to the main program. This phase will also see the creation of   functionality to control/communicate with the virtual machine through the main program.

[Milestone3](Milestone3.md)

[Description](DDDescription.md) [Design Document](DesignDocument.md) [Home](Home.md)