## BE_StackDelete

	BE_StackDelete ( name )

**Description**  

This function deletes the stack called *name* regardless of how many values it has in it.  Mostly only for cleanup purposes, stacks are removed when empty.

**Parameters**

* *name* : the name of the stored stack.

**Keywords**  

Stack Delete

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
