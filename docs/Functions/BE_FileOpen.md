## BE_FileOpen

    BE_FileOpen ( path )

**Description**  

Opens the file or folder at *path* using the default application chosen by the operating system.

**Parameters**

* *path* : the plugin path to the file.

**Keywords**  

File Open Path

**Version History**

* 1.3.0 : First Release
* 4.0.0 : return error 3, command not available on iOS and Linux
* 4.0.2 : Renamed from BE_OpenFile

**Notes**

There are some quirks to the way this function works on Mac OS : it sends an open request to the file, which is either able to be sent or not, and the response/error only reflects whether or not the request was sent. So for example if the file doesn't exist, or if you don't have permissions to open it, you get an error.

But whether the file actually opens or not is another thing, the OS will check things like apps for security and that can take some time. So whether or not the open is successful is different from whether or not the plugin tried to open the file. The two can be different, and the plugin only handles the sending of the open request, it doesn't poll to figure out if the open was successful or not.

In technical terms, the two are asynchronous - meaning we get back the first "done and ok" response and would have to continuously query the OS to find out the final state. The plugin doesn't do that. You'd need to use some other function to work out if the open did what you were expecting.

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
