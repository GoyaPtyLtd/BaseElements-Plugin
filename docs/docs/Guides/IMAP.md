## How to connect to an IMAP server from FileMaker

( With thanks to Claus Lavendt from DataManix. )

**About**

Because the BaseElements plugin includes a fairly comprehensive version of the cURL library, you can use it to connect and fetch emails from an IMAP server. This is not possible with the FileMaker built-in support for cURL ( added in v16 ), as the IMAP protocol is not supported.

Be aware that the IMAP protocol and commands are a bit odd, so you’ll need to do some trial & error.

**To begin with**

You will need 3 pieces of information from your IMAP account:

1. Server address like mail.company.com
2. Username, often a full email address like user@company.com
3. Password.

In the code examples below, we’ll assume that you are running these functions in a script and that you set these values into three local variables, $server, $user and $pass. We also assume that you will connect via port 993, which is the standard port for secure IMAPS.

You use the HTTP functions, but set a imaps url to contact the server. A typical imaps url would look like :

    imaps://<username>:<password>@<server>/

There's a good chance that with a complex password and with the @ symbol in the email that you will need to url encode the username and password. In FileMaker that could be written as:

    SetVariable ( $auth ; GetAsURLEncoded ( $user & “:” & $pass ) )

When you connect to the server, you're going to use the HTTP GET function which is actually going to do IMAP, and get a result each time.

**Connecting**

So the entirety of connecting to an IMAP server is :

    BE_HTTP_GET ( "imaps://" & $auth & "@" & $server & "/" )

That's it. If all the details are correct, you'll get back a list of folders like :

    * LIST (\HasChildren) "." Projects
    * LIST (\HasNoChildren) "." Projects.BaseElements
    * LIST (\HasNoChildren) "." Projects.Plugin
    * LIST (\HasNoChildren) "." Projects.RestFM
    * LIST (\HasNoChildren \Drafts) "." Drafts
    * LIST (\HasNoChildren \Trash) "." Trash
    * LIST (\HasNoChildren \Sent) "." Sent
    * LIST (\HasNoChildren \Junk) "." Junk
    * LIST (\HasNoChildren) "." INBOX

If removed a few from my list, but you can tell from that which ones have subfolders and which don't.

The next step is to select a folder, for example, the INBOX and get info about how many emails are there.

So **CURLOPT_CUSTOMREQUEST** is how you set the IMAP command that you're going to use, and then you run it via the GET function:

    BE_CurlSetOption ( "CURLOPT_CUSTOMREQUEST" ; "SELECT INBOX" )

then

    BE_HTTP_GET ( "imaps://" & $auth & "@" & $server & "/" )

If that works, you'll get a response like :

    * FLAGS (\Answered \Flagged \Deleted \Seen \Draft $NotJunk NotJunk $Forwarded Forwarded $Junk JunkRecorded Junk $MailFlagBit2 Redirected)
    * OK [PERMANENTFLAGS (\Answered \Flagged \Deleted \Seen \Draft $NotJunk NotJunk $Forwarded Forwarded $Junk JunkRecorded Junk $MailFlagBit2 Redirected \*)] Flags permitted.
    * 15 EXISTS
    * 0 RECENT
    * OK [UIDVALIDITY 1238974388] UIDs valid
    * OK [UIDNEXT 178860] Predicted next UID
    * OK [HIGHESTMODSEQ 634014] Highest

You can then get the number of emails in your inbox from the line that includes the "EXISTS" text. Parsing that out in FileMaker left as an exercise for the reader :)

**Downloading messages**

In IMAP every message has a UID which sounds like something unique… however, that’s not always the case and this is where you might need to do some trial & error testing, depending on your purpose. To begin with, assume the first message is UID 1 and go from there.

When downloading emails, you first need to remove the " SELECT INBOX" command, which you can do with :

    BE_CurlSetOption ( "CURLOPT_CUSTOMREQUEST" )

This clears out the previous option. So assuming you have the number "1" stored in the $uid local variable you do :

    BE_HTTP_GET ("imaps://" & $auth & "@" & $server & "/INBOX/;UID=" & $uid)

If it all works, you'll get the raw message content back, including all the headers. Again, parsing the content is up to the reader but if you're receiving automated emails you would look for various header lines and only view the text after those.

You can loop through messages by incrementing the uid number and then repeating the BE_HTTP_GET function.

**Deleting messages**

Finally, you might want to delete the emails, which you have fetched and for that you send a "STORE" command with the delete flag. All up that looks like :

    BE_CurlSetOption("CURLOPT_CUSTOMREQUEST";"STORE " & $uid & " +Flags \Deleted" )

assuming the same $uid of the message you retrieved before. Which you then send with :

    BE_HTTP_GET ( "imaps://" & $auth & "@" & $server & "/INBOX/“ )

But as mentioned, IMAP can be weird and we don't recommend using this on a mail client that is in heavy use by a user, as in between the GET and DELETE, the message list might have changed. Mail clients have a lot of error debugging to handle this, and in a purely automated mailbox, this would be ok, but in a live user mailbox, things could go wrong easily.

**Diagnostics**

Note : lots of servers, gmail and outlook in particular, require a specific ssl/tls connection method, so you may also need to add the FORCE_STARTTLS option at the start of the script. Try without it first, but if you get error 35 then add this to the script :

    BE_CurlSetOption ( "BE_CURLOPT_FORCE_STARTTLS" ; True )

**BE_CurlTrace** and **BE_GetLastError** are very helpful when debugging connection errors. In particular the Trace function will step you through issues as you go along.

More info about the error codes that all of the HTTP functions return can be found here: [https://curl.haxx.se/libcurl/c/libcurl-errors.html](https://curl.haxx.se/libcurl/c/libcurl-errors.html)

If you need more specific help with implementing this in your own solutions, or want help diagnosing connection issues, please consider a sponsorship.
