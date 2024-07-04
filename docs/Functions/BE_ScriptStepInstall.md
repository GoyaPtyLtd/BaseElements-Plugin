## BE_ScriptStepInstall

    BE_ScriptStepInstall ( name ; definitionXML ; id ; description ; calculation )

**Description**  

Creates a new definition of a script step, using *definitionXML* with the name of the step as *name*.  We created this function because we weren't sure what of our functions should be exposed as script steps as well as functions.  This step lets us ( and you ) build new steps from existing functionality.  Then we can decide to make them permanent features ( or not ).

**Parameters**

* *name* : the name of the new step.
* *definitionXML* : the XML that defines the operation of the step.  Details about this are in the SDK, but we'll also try to locate some better documentation.
* *id* : an internal id number for the step.  These can't be re-used.
* *description* : a text description that appears when you select the step in the script editor.
* *calculation* : the calculation for taking the step details and turning them into a result.

**Keywords**  

Script Step Install

**Version History**

* 4.0.0 : First Release

**Notes**

Calculation parameter :

You can reference the values returned by the UI parameters in your step, by using text like ###0### in your calculation.  Each parameter will be numbered from 0 to x and you just use ###0###, or ###1### or ###2### in your calculation and these will be replaced on completion.

XML Definition :

This XML is the text that will be passed to the scriptStepDefinition parameter of the RegisterScriptStep function. Up to ten script parameters can be specified in addition to the optional target parameter. All the parameters are defined with tags in a grouping.

The attributes for a tag include:

* Type - if not one of the following four types, the parameter is ignored

1. Calc - a standard Specify button that brings up the calculation dialog. When the script step is executed, the calculation will be evaluated and its results passed to the plug-in.
2. Bool - simple check box that returns the value of 0 or 1.
3. List - a static drop-down or pop-up list in which the id of the item selected is returned. The size limit of this list is limited by the capabilities of the UI widgets used to display it. A List type parameter expects to contain tags as specified below.
4. Target - will include a specify button that uses the new Insert From Target field targeting dialog that allows a developer to put the results of a script step into a field (whether or not it is on a layout), into a variable, or insert into the current active field on a layout. If no Target is defined then the result Data object is ignored. If there are multiple Target definitions, only the first one will be honored.

* ID - A value in the range of 0 to 9 which is used as an index into the DataVect& parms object for the plug-in to retrieve the value of the parameter. Indexes that are not in range or duplicated will cause the parameter to be ignored. A parameter of type Target ignores this attribute if specified

* Label - The name of parameter or control that is displayed in the UI

* DataType - only used by the Calc and Target parameter types. If not specified or not one of the six data types, the type Text will be used 1. Text 2. Number 3. Date 4. Time 5. Timestamp 6. Container

* ShowInline - value is either true or false. If defined and true, will cause the parameter to show up inlined with the script step in the Scripting Workspace

* Default - either the numeric index of the default list item or the true/false value for a bool item. Ignored for calc and target parameters

Parameters of type List are expected to contain tags whose values are used to construct the drop-down or pop-up list. The id of a value starts at zero but specific id can be given to a value by defining an "ID" attribute. If later values do not have an "ID" attributes the id will be set to the previous values id plus one.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | No  |  
| iOS | Yes  |  
| Linux | Yes  |  

**Example Code**
