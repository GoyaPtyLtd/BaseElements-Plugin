## BE_StackCount

	BE_StackCount ( name )

**Description**  

This function returns a count of the total number of values remaining in stack *name*.

**Parameters**

* *name* : The name of the stored stack.

**Keywords**  

Stack Count

**Version History**

* 4.1.0 : First Release

**Notes**

A stack is an internal plugin storage type, where you push data into a stack ( like a list of values ) and Pop data out.  In the plugin, stacks are LIFO ( Last In First Out ) so when you push a value onto the named stack, it will be the next value to POP out of the stack.

Stacks have names, much like variables do.

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
