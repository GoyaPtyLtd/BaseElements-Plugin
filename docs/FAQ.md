## BaseElements Plugin on FileMaker Server

BaseElements Plugin is compatible with FileMaker server, with some caveats :

**Server Compatible Functions**

Some functions will return error 3 when called on platforms they do not work on, but not all functions can be relied on to work in every environment.  Access to the environment is different on different platforms, and can be affected by permissions, operating system and other factors beyond the control of the plugin.  Testing on multiple platforms is critical.

## File Permissions when running under FMS

A good method for testing some plugin functionality for FileMaker Server scripting, is to run it in the client first.  But permissions on the server will always interfere with the functionality on server, as the plugin runs a the "fmserver" user, which has very few permissions on the drive.

**Restrict file operations to the FMS Documents folder.**

If you keep all your file operations inside the FMS Data/Documents folder, there will be no permissions issues.  The fmserver user can read and write to this folder with no issues.  Setup other processes that either drop or read files inside that folder if you need to interact with other apps.

**Check errors**

Use the BE_LastError function every time you call a plugin function.  A permissions issue with a file operation will be BE_LastError 13.  This is for things like CreateFile or writing to disk.  Be aware too, that often you will get a True result from the BE_FileExists function, but not be able to write to that folder.

**Writing outside the FMS folder**

You will need to add the **fmserver** user to any folder that you want to write to.  But also you will need access to READ from any of the parent folders.  For example if you want to create a file inside this path :

    /Base/Docs/March/report.xls

Then the **fmserver** user needs read access to **/Base** and **/Base/Docs** and write access to **/Base/Docs/March**

## Notes about File Paths

Lots of the BE plugin functions use paths in the parameters or in values they return.  Please note :

**FileMaker paths and plugin paths are not the same thing.**

The paths used by the plugin are Operating System paths.  On the Mac, they usually start with /Volumes or /Users and on windows will be something like C:\Folder\Path.

FileMaker paths often start with file: or filemac: and these WILL NOT work in the plugin.

These file: prefixes that you get from things like the Get ( FilePath ) function and others, are unique to FileMaker and don't work anywhere else other than FileMaker itself, including not in the BE plugin.  So if you're mixing and matching FileMaker paths and plugin paths, you will need to convert them between the types in order to use them in other places.

**Plugin Paths Examples**

To get an example of the sorts of paths that the plugin uses, use the **BE_SelectFile** function in the Data Viewer, and choose a file with a known path.  This will return an example of the sort of path used by the function.

Mac :
/Users/John Smith/Documents/test.xlsx

Windows :
C:\Users\John Smith\Documents\test.xlsx

Linux :
/etc/hosts

**FileMaker Path Examples**

Mac :
file:/Volumes/Clients/contacts.txt
filemac:/Volumes/Clients/contacts.txt

Windows :
file:/C:/Users/John%20Smith/Documents/test.xlsx
filewin:/C:/Users/John%20Smith/Documents/test.xlsx

Linux :
file:/etc/hosts

Note the consistency of the type of slash used, vs the difference in \ vs / in Windows/Mac.

**Conversion of paths between types**

The best way to convert paths is to use the native FileMaker functions **ConvertFromFileMakerPath** and **ConvertToFileMakerPath**.  A "plugin" path is the same as the PosixPath for Mac or Linux, and the WinPath for windows operating systems.

    ConvertFromFileMakerPath ( $filemakerPath ; PosixPath ) //Convert FileMaker to Mac/Linux Plugin Path
    ConvertFromFileMakerPath ( $filemakerPath ; WinPath ) //Convert FileMaker to Windows Plugin Path
    ConvertToFileMakerPath ( $pluginPath ; PosixPath ) //Convert Mac/Linux Plugin Path to file://Mac path.
    ConvertToFileMakerPath ( $pluginPath ; WinPath ) //Convert Windows Plugin Path to file://Windows path.

## BaseElements Plugin and threads

The BaseElements plugin is currently thread safe as of version 4.0.4 for all functions.

**If you're using older versions of the plugin prior to 4.0.4**

In previous versions of the plugin, an instance of the plugin had a single store of temporary storage that it was using during and in between operations ( things like storing the SMTP settings before the send, or the HTTP headers before a call ).

Because of this, if you're running two instances of the plugin on a single machine, they can interfere with each other and step on each other's toes in terms of threads and operations, and therefore cause issues.  Again, running in this way is not documented or tested, and is not recommended.

If you're calling scripts via PSOS or Schedules that use the BE plugin, we recommend that you do this in a way that limits the scripting to one process at a time.  So either you a system to flag when other processes are active and pauses until the flag is cleared.  Or you run a single process via schedule, that repeats and waits for a queue of "event" records to process.  Each one can be different actions to run, but they run in sequence as they come in.

Either of these methods work to keep the plugin to a single instance, and therefore reduce the dependency issues that may come up and so prevent crashes or other issues.

Upgrading to version 4.0.4 will negate the need for any of these precautions.
