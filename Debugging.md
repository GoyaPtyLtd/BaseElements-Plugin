

## Debugging on macOS

To debug on macOS you need to compile the plugin in XCode as a debug version, and then enable debugging in the FileMaker Pro app itself.

**Quit FMP**

You can't have any copies of FileMaker Pro running.  You need a clean slate to start with.  Also Xcode will have a "default" version of FMP that it tries to open, so you'll need to know which one this is.  In my case it's in the Applications folder, so the paths below will use that.

**Delete ALL old plugins**

Remove all the old plugins.  These days you can put plugins in quite a few folders and chances are FMP will load one you're not expecting, so it's easier to delete all old ones now so you know that FMP can only load the one you build.

**Build plugin with cert**

The Xcode project is set with a bunch of Goya and "nick" specific settings which you'll need to go through and replace.  You'll need your own Apple Developer Certificate as well, which you'll sign the plugin with.

( If you've got advice about how we can make the project settings work better for other users, please let us know. )

To check that the signing works properly, run the command below, but with the path to your built, signed copy of the plugin:

	codesign -dv --verbose=4 "/Users/nick/Library/Application Support/FileMaker/Extensions/BaseElements.fmplugin"

The output should include a line like the below, but with your certificate info:

	Authority=Apple Development: Goya Pty Ltd (GUZF844KMZ)

Make note of this as you'll need it in the next step.


**Replace FMP app certificate**

Run the two commands below, again replacing the paths with the path to the copy of the app that defaults when you double click a FileMaker file, and the name of the certificate you found above:

	codesign  --remove-signature "/Applications/FileMaker Pro.app"
	codesign --force --verbose --sign "Developer ID Application: Goya Pty Ltd (GUZF844KMZ)" "/Applications/FileMaker Pro.app"

You should get back a response like:

	/Applications/FileMaker Pro.app: signed app bundle with Mach-O universal (x86_64 arm64) [com.filemaker.client.pro12]

If you get that, then it's worked.

This replaces the certificate that Claris provides with one that matches the plugin certificate, so that you can attach the debugger in Xcode.

Note that this will break all sorts of things, like updates etc so once you're done, the best way to revert to the original is to delete the app and replace it with a clean version.

**Run in Xcode**

Now when you click Run in Xcode you should get a dialog authorising Xcode to attach to the process, plus a bunch of macOS security questions about allowing cameras etc ( because to macOS it's a "new" app ), but once it's up and running you should be able to go to the Debug menu and the option "Detach from FileMaker Pro" is active because it's now attached.

The final check is to look for some code to set a breakpoint in.  For example if you're wanting to debug the **BE_PDFAppend** function, search for that in the code and look for the opening function.  For Append it looks like:

	fmx::errcode BE_PDFAppend ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )

If you click the line number to the left of this function name, it should highlight in dark blue as you set a breakpoint.  If it remains dark blue ( and doesn't fade after a second or two ) then it's all worked and when you try to call that function in FMP, Xcode will catch it and pause there.  You can step through a view the details one line at a time.

Getting useful info out of Xcode at this point is a whole other thing, but at least now you're up and debuggging.
