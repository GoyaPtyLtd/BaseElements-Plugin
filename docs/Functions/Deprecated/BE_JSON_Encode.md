BE_JSON_Encode

    BE_JSON_Encode ( key ; { value ; type } ) 

**Description**  

Encodes a single value as JSON encoded text.

**Parameters**

* *key* : the name of the key value to encode.
* *value* ( optional ) : The value of the key to encode.
* *type* ( optional ) : The type is determined automatically from fields content.

For text that you want to force to be a different type, use the *type* parameter. Possible values are Boolean ( JSON True or False ), Text or Number ( JSON integer or real, but also includes FileMaker date, time or timestamp ). Any other values will error.

**Keywords**  

Removed

**Version History**

* 2.1.0 : First Release
* 4.0.0 : Deprecated. Use internal JSON functions instead.
* 4.2.0 : Removed.

