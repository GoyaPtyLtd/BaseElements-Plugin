## BE_SMTPAddAttachment

    BE_SMTPAddAttachment ( { attachment ; contentType } )

**Description**  

Adds the details of the file in the *attachment* container field to the list of attachments, for future *BE_SMTPSend* functions, to be sent with the *contentType*.

**Parameters**

* *attachment* : A container field to use as an attachment in an email.
* *contentType* : A mime content type for the attachment.

**Keywords**  

SMTP Attachment Add ContainerField

**Version History**

* 3.3.0 : First Release
* 4.0.0 : Changed the *container* parameter to *attachment* and added the *contentType* parameter
* 4.0.2 : Renamed from BE_SMTP_AddAttachment

**Notes**

The *BE_SMTPSend* function lets you add attachments via a path whereas this function lets you select attachments from fields.  You can add multiple attachments by calling this function multiple times.  Once *BE_SMTPSend* is done it clears out the list of stored attachments regardless of the success of the Send operation.

Attachments are actually written to disk by the plugin, to the users temp folder, so this does require you to have access to a working temp folder ( defined by the OS ).

The *contentType* is what is set as the mime type for emails.  So usually something like :

	Content-Type: application/pdf; charset=UTF-8
	Content-Type: text/plain; name="file.txt"
	Content-Type: application/octet-stream; name="file.pdf"

Possible mime types are found here :

[https://www.w3docs.com/learn-html/mime-types.html](https://www.w3docs.com/learn-html/mime-types.html) 

The result of adding the attachment may be unknown until after the *BE_SMTPSend* function has run.  Use the *BE_CurlTrace* function and look for the section where the file path is referenced and any resulting error messages.

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
