## Special Types
*generic* -- Generics can take the form of anything. They preserve the type of whatever they are assigned, and are a way to pass information when you don't know what it is.  

*instance* -- Instances are ways to reference tasks.
Functions:  
instance.join() -- the current task will wait on the completion of the instance and join will return the output of the task.  
instance.complete() -- returns a bool, true if the task is done, false if not.

*tasklist* -- Tasklists are collections of tasks. They work similarly to instances.  
tasklist.merge() -- Waits on all tasks in the list to be completed and then collates them in a collection {{}};  

## Collections, Tuples and Arrays
Collections -- {{type}} work like python lists  
Tuples -- (type, type, ...) are fixed length arrays  
Arrays -- type[] work like C++ vectors  

## Tasks, Function, and Service definitions:  
*Function* -- A repeatable section of a code  
*Task* -- A task is a section of the code made to be parallel and run in parallel.  
*Service* -- A service is a semaphore'd class. It is a collection of protected data and functions offered across tasks, with each function and data allowing a specified amount of parallel access. As there is no global scope in Equi, instead this can be created by making a global service sharing all of the global data.  
  
**Example function definition:**  
```
function fName: (input) -> (output) ::  
{  
	//Code  
}  
```
  
**Calling a function:**  
```
out = fName(input)  
```
The variable out is then filled by the function output. We could also just use the function inline as if it were a member of the output datatype.  
  
**Example task declaration:**  
```
task taskName: (input) -> (output) ::  
{  
	//Code  
}  
```
**Declaring a task run:**  
```
instance t = input->taskName;  
output o = t.join();
```
Spawning a task creates an instance object which we can use to reference the task later. We can call its **join** function to wait until task completion and get the output of the task.

**Example Service declaration:**  
```
tbd
```
**Using a service:**  
```
tbd
```

### Advanced inputs
Sometimes we have optional parameters we want to pass into a function sometimes and not others. To do this we have a special operator.
```
function f: (int a,int b) -> (int) ::
{
	string operation =& "add";

	if (operation == "add")
		return a+b;
	else if (operation == "multiply")
		return a*b;
	else if (operation == "subtract")
		return a-b;
	else if (operation == "divide")
		return a/b;
	else
		return 0;
}

int multiplied = f(4,2, "multiply" as operation);
// multiplied equals 8 here, 4 * 2

int added = f(4,2);
// added equals 6 here, 4 + 2
```

What we did here is use the special "=&" operator in the function, which is basically a if-not-defined-equal. It lets us know that we can pass in an already defined function in. This is basically passing by reference in C++. Then when we call the function we use the "as" keyword, telling it to pass in the string "multiply" as the variable operation.

### Tasks and functions as variables
What if we had a really complicated bit of machinery and wanted to pass in a task or a function as a variable, how would we do that?

Well, when we define the task or function it automatically becomes usable that way, and we can use it like that:
```
task runf: (function f) -> (generic) ::
{
	generic input =& ();
	return(f(generic));
}

function console: (string s) -> () ::
{
	print(s);
}

function f = console;
task t = runf;
((f,"Hello World" as input)->t).join();
```
Above we declare a function and a task, the task taking a function as an input (although we easily could have gone in the other direction), and the function taking a string as an input.

Note here the use of the generic type -- what this is basically is saying that at this scope we don't know the type of a variable, but we know we need to pass it along. If we cast a generic to something that it truly isn't, then we will get an error.

### Explicit Concurrency Control
In it's base form Equi is free to freeze a low priority task in order to complete a high priority task. In some applications it may be necessary to reach a certain point before a task may yield. To allow greater control over the concurrency of tasks on a single worker, you can enable an explicit yield block like so:  
```
task sampleT: () -> () ::
{
	//Do things
	#explicitYield
	// Will not yield without a yield command here.
	...
	yield();
	// Given the yield() command the worker checks for higher priority tasks and yields to them.
	...
	#implicitYield
	
	// May yield at any point here
}
```

## Special Functions
*array(collection)* -- takes a collection and formats it as an array  
*sort(array, function)* -- takes an array and a function (formatted like a C style comparator) and sorts the array using the function, it returns the sorted array.  
*csv(string)* -- reads a file and spins off tasks for each line, the tasks are sent in as *lineTask* and the delimiters are sent in via *delims*. If no task is sent in then csv throws an error. It returns a tasklisk.

