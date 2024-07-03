## BE_StackPop

	BE_StackPop ( name )

**Description**  

Returns the first value from the stack called *name* and removes it from the stack, reducing the stack count by 1.

**Parameters**

* *name* : the name of the stored stack.

**Keywords**  

Stack Pop

**Version History**

* 4.1.0 : First Release

**Notes**

Be careful when testing code and using the Pop function in the data viewer as once a value is removed from the stack, it's deleted - that's the intention :)

A stack is an internal plugin storage type, where you push data into a stack ( like a list of values ) and Pop data out. In the plugin, stacks are LIFO ( Last In First Out ) so when you push a value onto the named stack, it will be the next value to POP out of the stack. 

See this for more info : [https://www.thoughtco.com/definition-of-stack-in-programming-958162](https://www.thoughtco.com/definition-of-stack-in-programming-958162)

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
