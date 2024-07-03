## BE_VariableSet

	BE_VariableSet ( name ; { value } )

**Description**  

This function creates and sets a variable of *name* with *value*.

**Parameters**

* *name* : The name of the variable to create/set.
* *value* ( optional ) : The value to store.  Optional because an empty value will delete that variable.

**Keywords**  

Variable Set

**Version History**

* 4.1.0 : First Release

**Notes**

Plugin variables are much like $local and $$global FileMaker variables, except the scope is the instance of the plugin, so will persist across FileMaker files and even on closing and opening of files.  They are only lost when the plugin is loaded or unloaded, usually only ever when FileMaker restarts.

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

