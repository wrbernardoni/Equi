 # Equicontinuous
 **Main:** [![Build Status](https://travis-ci.com/wrbernardoni/Equi.svg?token=bRFz62XyihcjsQqousQJ&branch=master)](https://travis-ci.com/wrbernardoni/Equi)  
 **Dev:** [![Build Status](https://travis-ci.com/wrbernardoni/Equi.svg?token=bRFz62XyihcjsQqousQJ&branch=main-dev)](https://travis-ci.com/wrbernardoni/Equi)  
 
 Equicontinuous (aka Equi, aka EquiParallel -- whichever sticks) will one day be an interpreted parallel programming language.

 Equi is currently in **v.0.0.3 -- Chihuahua Purple**

 To run Equi, either set a system path to Equi (preferred), or run the executable in a shell script like so:

 ```
Equi -- -V 1
 ```

 This runs off of stdin (hence the -- as the second input) with a verbosity of 1 (will output the result of each line)

 If you want to run off of a file (say the test file 1--helloworld.equi), you can run it as so:

 ```
Equi 1--helloworld.equi
 ```


## Changelog
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

