# Minesweeper Solver #
## Milestone 3 Meeting Report ##

### Progress To Date ###
  * GTK GUI / game interface completed
  * Program language definition completed
  * Program interpreter completed
  * Configuration file for program interpreter complete
  * Game instance complete
  * Configuration files completed
  * Creation of populations
  * Iteration through the populations
  * Give reports on given populations and which ones move on to the next generation

Our project is nearly complete.  We have the program create random pieces of code in our newly created language.  However, our goal to have the genetic algorithm completed has fallen a little short.  We have ideas on how we would like to implement our algorithm.  Basically by creating working blocks of code that could be put together to create another working program.  The problem occurs when creating these blocks of code, the variations of the code blocks make it difficult to combine and mutate them.  The current blocks are in themselves capable of solving the mine sweeper game but attempting to combine another solver creates a host of complications.


### Projected Progress ###
  * Configuration files / file formats
  * Population
  * Iteration

We have created the format for the configuration files for the main program to tell the virtual machine what to do.  Also, we have created population files that allow the interpreter to run through the code and have the main program analyse the data.  However, we do not have the genetic algorithm up and running currently.  The program will iterate through the population files but it will not create new generations based on the previous generations population.


### Problems Encountered ###
  * Learning how to debug the new language
  * Finding a way to combine and mutate populations to new generations in this new language

Due to the newly created programming language, knowing what caused erroneous results without any compiler telling you what caused them sometimes proved difficult, even in a very simple setting.  Many times, a misspelled word in our population creation algorithms resulted in the solver returning a solving rate of zero percent for all populations, which is impossible if one square was picked.

As stated earlier, knowing how to create a simple enough algorithm in a very simple language to combine it with another algorithm was difficult.  Our idea consisted of creating preset blocks of code that we knew would work so the program will generate random solvers by picking different pieces that had already been created.  However, our blocks of code used were generally so intricate, they were already self sufficient and adding an extra block of code from another solver would prove meaningless.

We will have to keep considering and working on new ideas on how we want to solve the genetic algorithm problem.  It would seem our initial idea of blocking out pieces of code may not work as efficiently as we would like it to for solving this problem and we may need to compartmentalize the pieces more to allow for combination and alteration.

### Conclusion ###
The project is very close to completion.  All that needs to be done is to figure out a coherent solution to the genetic algorithm we can use to solve the mine sweeper game.  After that, we can do some more thorough testing to ensure that our project is stable and working currently.  Then we will be able to present our findings to the class.

[Home](Home.md)