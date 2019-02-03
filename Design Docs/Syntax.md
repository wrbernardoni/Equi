Tasks and Function definitions:  
Function -- A repeatable section of a code  
Task -- A task is a section of the code made to be parallel and run in parallel.  
  
Example function definition:  
	function fName: (input) -> (output) ::  
	{  
		//Code  
	}  
  
Calling a function:  
	out = fName(input)  
It functions as the datatype of the output would.  
  
Example task declaration:  
	task taskName: (input) -> (output) ::  
	{  
		//Code  
	}  
Declaring a task run:  
	out = (input)->taskName;  

