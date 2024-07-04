## BE_SMTPSetHeader

    BE_SMTPSetHeader ( { header ; value } )

**Description**  

Sets a custom SMTP header for subsequent SMTP send actions.

**Parameters**

* *header* ( optional ) : header name.
* *value* ( optional ) : header value.

You can remove all headers by calling with no parameters.

**Keywords**  

SMTP Header Set

**Version History**

* 4.0.0 : First Release
* 4.0.2 : Renamed from BE_SMTP_Set_Header

**Notes**

A useful list of possible header values can be found here : 

[https://www.iana.org/assignments/message-headers/message-headers.xhtml](https://www.iana.org/assignments/message-headers/message-headers.xhtml) 

Some possible uses for this function are to apply headers such as : 

	X-Priority: 1 (Highest)
	X-MSMail-Priority: High
	Importance: High

The actual effect of setting this header depends on the both the mail server and clients.

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
