## BE_JSON_jq

    BE_JSON_jq ( json ; filter { ; options } )

**Description**  

jq is a lightweight and flexible command-line JSON processor akin to sed,awk,grep, and friends for JSON data. It allows you to easily slice, filter, map, and transform structured data.

**Parameters**

* *json* : the input to the jq function.
* *filter* : the filter(s) to run on the input.
* *options* : one or more of the following characters : V c j r - see notes.

**Keywords**  

jq json

**Version History**

* 5.0.0 : First Release

**Notes**

More info is here : https://jqlang.github.io/jq/

And a manual with all the details is here : https://jqlang.github.io/jq/manual/

For testing your filters use : https://jqplay.org

The option values are : 

c - compact - By default, jq pretty-prints JSON output. Using this option will result in more compact output by instead putting each JSON object on a single line.

r - raw output ( ie not quoted text strings ) - With this option, if the filter's result is a string then it will be written directly to standard output rather than being formatted as a JSON string with quotes. This can be useful for making jq filters talk to non-JSON-based systems.

j - join output : Like -r but jq won't print a newline after each output.

V - version info.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | No  |  
| FMS | Yes  |  
| iOS | Yes  |  
| Linux | Yes  |  

**Example Code**

Get all the names of product types : 

    BE_JSON_jq ( $json ; ".productTypes[].name" )

In an array of prices select the ones where quantity is 1, then sort by price, and find the largest, and return that price :

    BE_JSON_jq ( $json ; "[ .prices[] | select(.quantity == 1 ) ] | max_by(.price) | .price" ; "r" )

Find all possible values of a node, deep in a json object : 

    BE_JSON_jq ( $json ; “.data.products.edges[].node.category.name” ; “r” )