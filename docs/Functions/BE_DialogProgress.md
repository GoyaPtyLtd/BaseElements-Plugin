## BE_DialogProgress

    BE_DialogProgress ( title ; description ; { maximum } )

**Description**  

Shows either a barber pole progress dialog by leaving the *maximum* parameter out, or a regular start to finish progress dialog.

**Parameters**

* *title* : The dialog title.
* *description* : The content of the dialog.
* *maximum* ( optional ) : A positive integer for a progress dialog, or empty for barber pole.

**Keywords**  

Dialog Progress

**Version History**

* 2.1.0 : First Release
* 4.0.0 : Return error 3, command not available on iOS, Linux and under FMS.
* 4.0.2 : Renamed from BE_ProgressDialog

**Notes**

You can disable the "Cancel" button by calling the Allow User Abort [Off] script step prior to calling this function.

Barber pole dialogs are closed by calling BE_DialogProgressUpdate with a positive integer "number" parameter. Regular dialogs are closed with any value greater than maximum.

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

	BE_DialogProgress ( "title" ; "description" )
	
Will show the barber pole dialog.

	BE_DialogProgress ( "title" ; "description" ; 9 ) 

Will show a progress dialog with 10 steps ( 0 through 9 inclusive ).