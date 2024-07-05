## BE_ScriptExecute

    BE_ScriptExecute ( scriptName ; { fileName ; parameter ; scriptControl } )

**Description**  

Performs the script called *scriptName* in the current file ( if no fileName provided, or in *fileName* if available ). 
**Parameters**

* *scriptName* : the name of the script to perform.
* *fileName* ( optional, default:current File ) : a filename of the file in which to locate the script.
* *parameter* ( optional ) : the parameter to pass to the script called.
* *scriptControl* ( optional, default:Pause ) : The values for this parameter are: Halt = 0, Exit = 1, Resume = 2, Pause = 3.

**Keywords**  

Script

**Version History**

* 1.0.0 : First Release

**Notes**

Scripts called from the plugin are added to the script queue, so they run when the current script queue is empty.  In other words you can't have a calculation within a script, call a script using this plugin function, and have it start immediately.  It will run at the completion of the current script. Script results are not set and cannot be returned.

Be aware, there is an issue in the plugin API where filenames can only have a single "." in their name. Files with more than one will return an error 100 - File is missing.

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
