 # Equicontinuous
 **Main:** [![Build Status](https://travis-ci.com/wrbernardoni/Equi.svg?token=bRFz62XyihcjsQqousQJ&branch=master)](https://travis-ci.com/wrbernardoni/Equi)  
 **Dev:** [![Build Status](https://travis-ci.com/wrbernardoni/Equi.svg?token=bRFz62XyihcjsQqousQJ&branch=main-dev)](https://travis-ci.com/wrbernardoni/Equi)  
 
 Equicontinuous (aka Equi, aka EquiParallel -- whichever sticks) will one day be an interpreted parallel programming language.

 Equi is currently in **v.0.0.6 -- Chihuahua Red**

 To run Equi, either set a system path to Equi (preferred), or run the executable in a shell script.

 Then call the executable ala:

 ```
Equi -- -V 1
 ```

 This runs off of stdin (hence the -- as the second input) with a verbosity of 1 (will output the result of each line)

 If you want to run off of a file (say the test file 1--helloworld.equi), you can run it as so:

 ```
Equi 1--helloworld.equi
 ```


## Changelog
**v.0.0.6 -> Current**
  * Fixed reintroduced issue which did not allow you to use temporary variable member functions.
**v.0.0.5 -> v.0.0.6**
  * Added tasks, which may be invoked `bar -> foo` where bar is an input to the function foo, which may be ran on an external task. Results of the function may be gotten through the use of futures (i.e. `future res = bar->foo`, which may later get the result of the function via the member function `res.get()`)
  * Added a command line option to specify the number of threads used, e.g. `Equi helloworld.equi -t 0` to run single threaded, or `Equi helloworld.equi -t 10` to run using 10 threads in addition to the main thread.
  * Added additional step where Equi compiles into a semi-bytecode, this is for later making multi-processing easier and allowing for more compile time optimizations in the future.
  * Altered roadmap
  * Added `=&` operator, which will define and initialize a variable iff it has not been defined yet
  * Added `as` operator, which allows for function calls like: `f(1,2, "multiply" as operation)`, which sets `operation` to the string `"multiply"` before executing the function `f`
  * Added a `size()` and `at(int)` member function to the tuple class.
  * Added tests for multithreading, as, and new member functions.

**v.0.0.4_rev1 -> v.0.0.5**
  * Added custom function declarations that allow for using other custom functions and recursion.
  * May now reference an empty tuple, i.e. `() t = ()`
  * Equi now can evaluate expressions to access array elements, i.e. `a[3*2] == a[6]`
  * Roadmap revised (pushing off type work until after multithreading)

**v.0.0.4 -> v.0.0.4_rev1**
  * Fixes issue where temporary variables were deleted before you could access their member functions (i.e. `"hi".size()` now works)
  * Added tuple style compound assigment `(a,b,c) = (1,2,3)` will set `a = 1`, `b=2`, and `c=3`.
  
**v.0.0.3 -> v.0.0.4**
  * Added two command line options -PF (parse whole file and then run) and -PF (parse line by line and run as you get each line), by default when ran on a file it runs in PF, when run on command line input it runs on PL
  * Made evaluating the parse tree spawn fewer objects, and be slightly more memory efficient.
  * Fixed a memory leak in the for loops and one in the tuple object
  * Added member functions to arrays and tuples (just size() and at() right now), in a way that is very general.
  * Added arrays and added tuple declaration
  * Syntax parser is now a little more verbose in errors (still work to do though)
  * Now builds and tests on Travis CI

**v.0.0.2 -> v.0.0.3**  
  * Implemented modulus (%) operator for generic types. Now having a defined division, multiplication, and subtraction defines a modulus.
  * Added for, while, and do...while loops.
  * Added ++ and -- operators.
  * Added scoping to memory. Things defined in a block statement will be deleted after the block statement is complete.
  * Fixed crash on spawning strings.
  * Updated roadmap.
  * Added capability to have '\n' and '\"' in a string.
  * Added capability to redefine tokens in a lower scope.
  
**v.0.0.1 -> v.0.0.2**  
  * Added parsing and execution of if, else, and else if code blocks.  
  * Fixed minor error in CFG documentation, multiplicative should have been 1 or 2 arguments, not fixed at 2. In addition fixed minor spelling mistake.  
  * Now evaluates the constants `true` and `false` correctly into boolean values.  
  * Added v.0.0.2 Equi Test program.  

