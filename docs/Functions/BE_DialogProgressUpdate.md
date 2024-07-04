## BE_DialogProgressUpdate

    BE_DialogProgressUpdate ( number ; { description } )

**Description**  

Used to either close a *BE_DialogProgress* dialog, or advance a progress dialog to the next step.

**Parameters**

* *number* : The number to move the progress bar to.
* *description* ( optional, default:previous value ) : The text to put into the dialog description.  If not included it will use the last value sent to this function, or will use the value that initially was set via *BE_DialogProgress*.

**Keywords**  

Dialog Progress Update

**Version History**

* 2.1.0 : First Release
* 4.0.0 : Return error 3, command not available on iOS, Linux and under FMS.
* 4.0.2 : Renamed from BE_ProgressDialog_Update

**Notes**

Notes

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | No  |  
| iOS | No  |  
| Linux | No  |  

**Example Code**

	BE_DialogProgressUpdate ( 1 ; "description" )
	
Will close a dialog if created with empty as the maximum - eg Barber Pole.

	BE_DialogProgressUpdate ( $nextValue ; "description" )
	
Move the progress dialog, along to the next value, while updating the description text.

	BE_DialogProgressUpdate ( 10 )

Close a progress dialog that started with maximum 9, and had 10 steps ( 0 through 9 ).
