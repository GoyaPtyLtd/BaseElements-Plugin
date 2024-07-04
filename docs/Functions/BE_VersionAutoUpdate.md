## BE_Version

    BE_Version

**Description**  

The text of the plugin number, formatted as an 8 digit number, eg 04020400. This is broken into four parts, and so 4.2.4 would become 04020400 as 04 02 04 00. The final two digits come from a minor rebuild version, if there was one, when we did new builds but didn't change version number. This may be due to build issues or signing updates etc.

**Parameters**

None

**Keywords**  

Version Auto Update

**Version History**

* 1.0.0 : First Release

**Notes**

This function should be used in place of *BE_Version* when checking for latest updates and comparing versions as numbers, as the text string response of *BE_Version* is not as easily compared. Use *BE_Version* when showing a meaningful description to the end user.

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
