## BE_VariableGet

    BE_VariableGet ( name )

**Description**  

Gets the value of the previously stored plugin variable with *name*.

**Parameters**

* *name* : the name of the variable to return.

**Keywords**  

Variable Get

**Version History**

* 4.1.0 : First Release

**Notes**

Plugin variables are much like $local and $$global FileMaker variables, except the scope is the instance of the plugin, so will persist across FileMaker files and even on closing and opening of files.  They are only lost when the plugin is loaded or unloaded, usually only ever when FileMaker restarts.

On FileMaker Server they exist in a single session, and not across different sessions as each session is a new application launch effectively.

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
