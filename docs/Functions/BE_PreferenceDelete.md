## BE_PreferenceDelete

    BE_PreferenceDelete ( key ; { domain } )

**Description**  

The is the reverse of the BE_PreferenceSet function, this removes the preference at *key*, in *domain* from the system.

Preferences set via the BE plugin are available across all open copies of FileMaker applications.

**Parameters**

* *key* : the key code for the value to set.
* *domain* ( optional, default: See Notes ) : the domain value of where to locate the preference file.

**Keywords**  

Preference Delete

**Version History**

* 4.1.0 : First Release

**Notes**

If the domain is not included, there is a default used on Mac and on iOS : 

	au.com.goya.baseelements.plugin-user
	
On Windows the default will be :

	Software\\Goya\\BaseElements\\PluginUser
	
( stored inside HKEY_CURRENT_USER ).

On Linux this function does not run, will return error 3 - Command is unavailable.

You can override the domain to specify a different file name on the mac, or a different path on Windows. To respect the various OS conventions you should use something similar to the above, but tailored to your solution.

This function is required in some situations as the user can't directly delete preferences files to remove a stored preference.  For example on the Mac, Setting a preference will create a corresponding file in the Users Library/Preferences folder.  However deleting that file does not delete this preference until logout, and it's possible that the preference file will be recreated from memory.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | Yes  |  
| Linux | No  |  

**Example Code**
