## BE_Version

    BE_Version

**Description**  

Returns the plugin version number as text, eg 1.1.3

**Parameters**

None

**Keywords**  

Version

**Version History**

* 1.0.0 : First Release

**Notes**

This function should be not be used to when checking for latest updates, as the text string response of it is not easily compared. For example :

	"1.9.0" < "1.10.0"
	
even though 10 is greater than 9.  To compare versions in order of release, use *GetAsNumber ( BE_VersionAutoUpdate )* instead and then use *BE_Version* to show a meaningful result to the end user.

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
