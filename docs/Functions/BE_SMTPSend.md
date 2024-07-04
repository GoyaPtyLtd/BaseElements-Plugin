## BE_SMTPSend

    BE_SMTPSend ( from ; to ; subject ; text ; { cc ; bcc ; replyTo ; html ; attachments} )

**Description**  

Sends an email via SMTP. Use in conjunction with a call to BE_SMTPServer prior to calling this function. The To, CC, BCC and attachments parameteres can be passed value lists in order to send to multiple people or to include multiple attachments.

**Parameters**

* *from* : the from name and email address.
* *to* : the to name and email address.
* *subject* : subject line for the email ( a required parameter, but can be an empty string ).
* *text* : text content of the email ( a required parameter, but can be an empty string ).
* *cc* ( optional ) : the cc name and email address.
* *bcc* ( optional ) : the bcc name and email address.
* *replyTo* ( optional ) : the replyTo name and email address.
* *html* ( optional ) : the text of the HTML version of the email content.
* *attachments* ( optional ) : a list of file paths for attachments to be included.

**Keywords**  

keyword keyword

**Version History**

* 3.1.0 : First Release
* 4.0.0 : Added an RFC 1123 format date header
* 4.0.2 : Renamed from BE_SMTPSend

**Notes**

The content of emails on some servers appears to strip FileMaker line endings. Try replacing Char ( 13 ) with Char ( 10 ) in the email content.

Some servers, in particular various Exchange or Office365 based servers use a different authentication method. When attempting SMTP normally you may get a 35 error. You can force the correct method with :

	BE_CurlSetOption ( "BE_CURLOPT_FORCE_STARTTLS" ; True )

before doing BE_SMTPSend.

**About HTML Content**

HTML content is complex and not easily setup.  We recommend you use an external process or application to generate the HTML content, and use either embedded base64 images, or links to externally hosted images.  There is not support for multipart mime messages that would let you attach multiple images and then use them inline inside the HTML content.

There are lots of examples and help around crafting HTML email content such as :

[https://sendgrid.com/en-us/blog/create-html-emails](https://sendgrid.com/en-us/blog/create-html-emails) however we make no guarantees around the ability of the plugin to send your HTML email and have it arrive successfully and look as intended.

We recommend people not use SMTP for email and use the various services that have APIs instead.

**About SMTP Send and "Sent" Folders**

A message being put into sent folder is a function of the mail client that sends it : it creates the message to send, sends a copy via the SMTP server, and then on success gives the IMAP server a second copy to save into the users *Sent* folder via IMAP.

The BE plugin SMTP only does the SMTP send part, so in order to save a copy in a Sent folder, you'd need to have use IMAP functions to save to the sent folder, along with the correct IMAP server credentials, which may be different than the SMTP server and credentials.  This is technically possible using the HTTP functions and curl options, but is beyond the scope of the intended use of this function.

Other options for retaining sent emails are :

* Some email services let you use an API instead of SMTP, ( eg google or O365 ) and that API may keep copies in the sent folder.
* BCC to an archive email address that stores old copies on the mail server.
* Keep them in a table in FileMaker instead.

See the *BE_SMTPAddAttachment* documentation for sending attachments from container fields.

---

Authentication is controlled with curl, so additional options, other than the defaults can be set with the *BE_CurlSetOption* function, and headers can be set via the *BE_SMTPSetHeader* function.

Not all servers respond with data when doing curl operations so use *BE_GetLastError* after the function call to tell if the call was able to be made.

Error codes that you get from the *BE_GetLastError* function after this function call comes from the curl library itself and not the plugin.  To find a specific error code use this documentation : 

[http://curl.haxx.se/libcurl/c/libcurl-errors.html](http://curl.haxx.se/libcurl/c/libcurl-errors.html)

Also use the *BE_CurlTrace* function to see the transcript of the connection to diagnose any other issues that may have come up and can't be determined from the error value alone.


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

	BE_SMTPSend ( "me@me.com" ; "you@you.com" ; "Subject goes here" ; $textContent  ; $cc ; $bcc ; "replyto@me.com" ; $htmlContent ; "/path/to/doc.pdf¶/path/to/secondDoc.pdf" )