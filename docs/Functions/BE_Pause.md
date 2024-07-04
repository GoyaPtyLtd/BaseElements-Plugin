BE_Pause

    BE_Pause ( milliseconds )

**Description**  

Pauses ( within the plugin code, so doesn't return to FMP ) for *milliseconds*.

**Parameters**

* *milliseconds* : The number of milliseconds to wait for.

**Keywords**  

Pause Time

**Version History**

* 3.3.0 : First Release

**Notes**

The plugin code runs in a single thread, so this will lock up any running script for this amount of time.

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
