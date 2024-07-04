## BE_FileMakerSQL

    BE_FileMakerSQL ( sqlStatement ; { columnSeparator ; rowSeparator ; databaseName ; asText ; outputPath } )

**Description**  

Performs SQL commands on the tables inside FileMaker.

**Parameters**

* *sqlStatement* : the SQL command to perform.
* *columnSeparator* ( optional ) : the column separator for the output data - single characters only.
* *rowSeparator* ( optional ) : the row separator for the output data - single characters only.
* *databaseName*  ( optional ) : allows you to specify an open database other than the current one
* *asText* ( optional, default:True ) : This is True by default as the SQL returns data from fields as text. Set this to false when retrieving a single container field.
* *outputPath* ( optional ) : Allows you to write the result of the SQL query to disk.

**Keywords**  

SQL

**Version History**

* 1.2.0 : First Release
* 1.3.0 : added optional database parameter to allow operations on databases other than the current one
* 4.2.0 : return container/binary data in addition to text (new asText parameter) and outputPath parameter added

**Notes**

The new *asText* parameter when set to False allows you to return a single file, when running SQL on container fields. So although you can perform an SQL query that selects multiple container fields at once, writing them to a container field doesn't make sense, and writing to disk becomes complex when matched with a need to specify all the paths and filenames. So this function is, by design, limited to a single container field at a time. If you need multiple fields, use a loop script, or while function.

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
