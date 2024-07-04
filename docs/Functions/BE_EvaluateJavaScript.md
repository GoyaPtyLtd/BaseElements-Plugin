## BE_EvaluateJavaScript

    BE_EvaluateJavaScript ( javaScript )

**Description**  

Evaluates the JavaScript passed to it as text and returns a result, if any.

**Parameters**

* *javaScript* : the JavaScript to evaluate.

**Keywords**  

JavaScript

**Version History**

* 3.1.0 : First Release

**Notes**

There are also two callback functions, so you can call FileMaker calculations using BE_Evaluate_FileMaker_Calculation or run FileMaker Script using BE_ExecuteScript from within the JavaScript code.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Testing |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | No  |  
| iOS | No  |  
| Linux | No  |  

Marked as **Testing** as it's not widely used, and functionality may change in the future, or even be removed.

This function uses the [Duktape](https://duktape.org) library.  We have not had many reports of people using this library in useful ways, so it may become deprecated in a future release.

The other direction this library could go is that we use it to add new built in BE plugin functions, that are purely js run via Duktape - we're open to ideas and suggestions for options here.

**Example Code**
