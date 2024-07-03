## BE_StackPush

	BE_StackPush ( name ; value )

**Description**  

Puts a value into the stack called name and "pushes" everything down the stack one value deeper.

**Parameters**

* *name* : The name of the stored stack.
* *value* : The value to store in the stack.

**Keywords**  

Stack Push

**Version History**

* 4.1.0 : First Release

**Notes**

A stack is an internal plugin storage type, where you push data into a stack ( like a list of values ) and Pop data out. In the plugin, stacks are LIFO ( Last In First Out ) so when you push a value onto the named stack, it will be the next value to POP out of the stack. 

See this for more info : [https://www.thoughtco.com/definition-of-stack-in-programming-958162](https://www.thoughtco.com/definition-of-stack-in-programming-958162

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | Yes  |  
| Linux | Yes  |  

**Example Code**
