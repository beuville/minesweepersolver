# Minesweeper Solver #
## Milestone 1 Meeting Report ##

### Progress To Date ###
  * GTK gui / game interface completed
  * Program language definition completed
  * Program parser started

The requirements called out by our [design document](DesignDocument.md), namely the game board/interface and program language definition have been completed. Furthermore in the process of completing the programming language definition a portion of the program parser was started putting the project ahead of schedule.

### Projected Progress ###
  * Parser completed
  * Integration with game completed
  * Virtual machine multi-threaded

The second milestone of this project, and the next step in development will see the virtual machine completed. This will couple the game board from the first milestone with the immediate project of the program parser into one entity. The final task of the second milestone will be to create multiple, simultaneous instances of the coupled game instance in one multi-threaded virtual machine.

### Problems Encountered ###
  * Wasted time on regex libraries
  * Deciding on a graphics library
  * Narrow development path

Some time was wasted on trying to find/install/troubleshoot regular expression libraries for c++. This effort was found to be unsuccessful and as such the string matching functions will have to be written by hand. This is not a major setback because that task was slated to be accomplished in the second milestone anyway.

The game board was eventually programmed using the GTK library, however several other libraries and approaches were tried before arriving at GTK. This did setback development significantly.

The fact that the game board and program language definitions were the main focus of the first milestone left one member of the team in a position that did not allow for full participation. This will be remedied in the future both by a widening scope of development in the second milestone as well as a new programming practice that will allow for less localized development. Namely, creating skeleton functions that are well commented but incomplete and then allowing team members to complete the functions as needed. The issues tool provided by Google Code will facilitate this as well.

### Conclusion ###

Thus far we have managed to keep the project going slightly ahead of schedule without large workloads. Chances are with the more complicated tasks coming up the workload will increase but progress should remain on, or ahead of schedule. The time-lost issues of library selection are behind us and the rest of the work should be essentially development/test oriented.

[Home](Home.md)