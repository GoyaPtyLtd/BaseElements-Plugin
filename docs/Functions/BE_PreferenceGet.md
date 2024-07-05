## BE_PreferenceGet

    BE_PreferenceGet ( key ; { domain } )

**Description**  

Gets the preference value stored by BE_SetPreference at *key*, in *domain*, from the system.

Preferences set via the BE plugin are available across all open copies of FileMaker applications.

**Parameters**

* *key* : the key code for the value to get.
* *domain* ( optional, default: See Notes ) : the domain value of where to locate the preference file.

**Keywords**  

Preference Get

**Version History**

* 1.3.0 : First Release
* 4.0.0 : Return error 3, command not available on Linux
* 4.0.2 : Renamed from BE_GetPreference

**Notes**

If the domain is not included, there is a default used on Mac and on iOS : 

	au.com.goya.baseelements.plugin-user
	
On Windows the default will be :

	Software\\Goya\\BaseElements\\PluginUser
	
( stored inside HKEY_CURRENT_USER ).

On Linux this function does not run, will return error 3 - Command is unavailable.

You can override the domain to specify a different file name on the mac, or a different path on Windows. To respect the various OS conventions you should use something similar to the above, but tailored to your solution.

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
