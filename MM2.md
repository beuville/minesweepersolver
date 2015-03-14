# Minesweeper Solver #
## Milestone 2 Meeting Report ##

### Progress To Date ###
  * GTK gui / game interface completed
  * Program language definition completed
  * Program parser completed
  * Configuration file for program parser complete
  * Game instance complete

We have fallen slightly behind on the requirements called out by our [design document](DesignDocument.md).  The virtual machine running, but is not multi-threaded. All of the components are fully functional with the program parser/game interface integration having some small hangups.  Specifically, the game interface is currently executing an old, text-based minesweeper gui. We have not begun digging too deeply into how exactly our genetic algorithms will be formed, but that is soon to come.

### Projected Progress ###
  * Virtual machine multi-threaded/multi-thread decision made
  * Population creation
  * Iteration scheme developed

The third milestone of this project is where the genetic algorithm portion of this project actually comes into play.  The soon to be completed game instance and virtual machine will be able to take the initial population of minesweeper solvers and compare completion percentages to determine which ones were the most successful.  These elite solvers can then be modified through mutations and crossovers that will (ideally) create better minesweeper solvers.  This process will be repeated until an ideal solver has been found or no more progress is being made.

### Problems Encountered ###
  * Problems with GTK+ libraries and Visual Studio
  * Heavy reliance on a linear progression of components
  * Multi-threading difficulties
  * Compiler transitions

Some time was wasted in trying to get the original minesweeper game to run in Visual Studio involving problems with some GTK+ libraries.

A decision needs to be made regarding the multi-threaded nature of the virtual machine. Multi-threading will make the project much more platform dependent, and the performance gains do not appear to be substantial. This may be a project that is put off to a later date as it does not impede further development.

Early stages of the program were compiled using cygwin linux, while the current virtual machine was compiled using visual studio. There are small differences in the code that compiles on cygwin vs. code that compiles in visual studio (i.e. strtok\_s) The original design document calls for development in visual studio, but a decision on which direction to proceed in should be made prior to main development.

This milestone had 3 major parts that had to be done in order to move on, creating a bottleneck that caused progress to halt whenever a speed bump was encountered.  Hopefully we can make up for lost time over the next few days so focus can be squarely placed upon the genetic algorithms.

### Conclusion ###

We have fallen slightly behind schedule with this milestone, but it shouldn't be too difficult to come back on schedule with a bit of extra time and effort over the next few days.  The third milestone is the most important one since it incorporates the genetic algorithms in with the rest of the elements, which is a major focus of this project.

[Home](Home.md)