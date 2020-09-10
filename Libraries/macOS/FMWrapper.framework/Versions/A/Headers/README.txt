FMMiniPlugIn Sample Plug-In
---------------------------

A sample plug-in is now included with the FileMaker Plug-In SDK. The environments used to build this small sample plug-in include:

macOS & iOS: macOS Version 10.14 with Xcode 10.2, targeting iOS 12.2
Win: OS Version 10 Visual Studio 2017 15.8
Linux: CentOS 7.3 Code::Blocks 16.01

Digitally Signing Plug-Ins
--------------------------

Check out these two blogs for how one can sign plug-ins

https://www.troi.com/plug-in-development/code-signing-filemaker-plugins/
http://fmplugins.idma.nz/index.php?title=Code_Signing

Plug-in version returned by Get(InstalledFMPlugins)
---------------------------------------------------

macOS & iOS: the CFBundleVersion string of the bundle's plist
Win: the FILEVERSION string of the DLL's VERSIONINFO resource
Linux: the soname field of the ELF shared library


Plug-in XML UI definition
-------------------------

This XML is the text that will be passed to the scriptStepDefinition parameter of the RegisterScriptStep function. Up to ten script parameters can be specified in addition to the optional target parameter. All the parameters are defined with <Parameter> tags in a <PluginStep> grouping.

The attributes for a <Parameter> tag include:

  * Type - if not one of the following four types, the parameter is ignored
      1. Calc - a standard Specify button that brings up the calculation dialog. When the script step is executed, the calculation will be evaluated and its results passed to the plug-in
      2. Bool - simple check box that returns the value of 0 or 1
      3. List - a static drop-down or pop-up list in which the id of the item selected is returned. The size limit of this list is limited by the capabilities of the UI widgets used to display it. A List type parameter expects to contain <Value> tags as specified below
      4. Target - will include a specify button that uses the new  Insert From Target field targeting dialog that allows a developer to put the results of a script step into a field (whether or not it is on a layout), into a variable, or insert into the current active field on a layout. If no Target is defined then the result Data object is ignored. If there are multiple Target definitions, only the first one will be honored.

  * ID - A value in the range of 0 to 9 which is used as an index into the DataVect& parms object for the plug-in to retrieve the value of the parameter. Indexes that are not in range or duplicated will cause the parameter to be ignored. A parameter of type Target ignores this attribute if specified

  * Label - The name of parameter or control that is displayed in the UI

  * DataType - only used by the Calc and Target parameter types. If not specified or not one of the six data types, the type Text will be used
      1. Text
      2. Number
      3. Date
      4. Time
      5. Timestamp
      6. Container 

  * ShowInline - value is either true or false. If defined and true, will cause the parameter to show up inlined with the script step in the Scripting Workspace

  * Default - either the numeric index of the default list item or the true/false value for a bool item. Ignored for calc and target parameters

Parameters of type List are expected to contain <Value> tags whose values are used to construct the drop-down or pop-up list. The id of a value starts at zero but specific id can be given to a value by defining an "ID" attribute. If later values do not have an "ID" attributes the id will be set to the previous values id plus one.

Sample XML description:

  <PluginStep>
    <Parameter ID="0" Type="Calc" DataType="text" ShowInline="true" Label="Mood"/>
    <Parameter ID="1" Type="List" ShowInline="true" Label="Color">
      <Value ID="0">Red</Value>
      <Value ID="1">Green</Value>
      <Value ID="2">Blue</Value>
    </Parameter>
    <Parameter ID="2" Type="Bool" Label="Beep when happy"/>
  </PluginStep>


Unified plug-in bundle format
-----------------------------

A unified plug-in bundle is now supported so that only one folder of items need to be distributed for the Mac, Win, and Linux platforms. The iOS bundles will still be independent since they cannot be installed by the FileMaker script step and instead are part of the iOS SDK build process. The bundle folder format matches the Mac plug-in bundle with the addition of a Windows and Linux folder inside the Resources folder. The format of the actual Windows or Linux dynamic libraries does not change, now you can just place the Windows .fmx and .fmx64 files into the Windows folder and the Linux .fmx file into Linux folder. The name (minus extension) of the dynamic library must match the name of the bundle folder (minus the extension). An example disk layout of a "fat" plug-in would look like:

FMMiniPlugIn.fmplugin
  Contents
    _CodeSignature
      CodeResources
    MacOS
      FMMiniPlugIn
    Resources
      Linux
        FMMiniPlugIn.fmx
      Windows
        FMMiniPlugIn.fmx
        FMMiniPlugIn.fmx64
    Info.plist
    PkgInfo

The reason the Linux and Windows folders are inside the Resources folder is due to how Mac bundles are signed by Xcode. Note that plug-ins are now required to be signed by their development environments and the bundle has to be signed after the non-Mac plug-ins are added to it. Currently only the Mac can import this bundle format into a container folder directly with the Insert File command. Insert from URL can be used on other platforms by creating a properly compressed Mac plug-in bundle via these two commands that come with the macOS:

  xar -cf MyCoolPlugin.fmplugin.xar MyCoolPlugin.fmplugin
  gzip -c MyCoolPlugin.fmplugin.xar > MyCoolPlugin.fmplugin.gz


List of changes for 19
----------------------

FMXCalcEngine.h
  New fmx::ExprEnv::EvaluateConvertToFileMakerPath and fmx::ExprEnv::EvaluateConvertFromFileMakerPath methods plus fmx::ExprEnv::kConvert_XXX enums to be able to call the new ConvertToFileMakerPath and ConvertFromFileMakerPath calculation functions directly

List of changes for 18
----------------------

FMXCalcEngine.h
  New fmx::ExprEnv::EvaluateGetFunction method and many fmx::ExprEnv::kGet_XXX enums for each FileMaker Get() calculation function

Plug-in loading
  The existence and validity of the digital signature for Mac and Windows plug-ins will be validated before it is loaded. MacOS and Windows versions of the Pro Advanced or Runtime client will ask the user if they want to trust an unsigned plug-in and add it to to the user preference's permitted list if specified. In all other cases unsigned or invalid/expired/revoked/etc signatures will prevent the plug-in from loading.

Install Plug-In File script step errors
  For error 1550 and 1551 the Get(LastExternalErrorDetail) function will return the following information for why a plug-in could not load, including the new reasons of having an invalid signature.

	Error 1550
		Unrecognized plug-in file
		Plug-in not digitally signed
		Invalid plug-in signature, OSStatus error: <number from macOS> Certificate name: <name from macOS>
		Invalid plug-in signature, Windows error: <hex number from Windows> Certificate name: <name from Windows>
		Plug-in with duplicate id already loaded
		Could not load plug-in code, macOS error: <text from macOS>
		Could not load plug-in code, GetLastError: <number from Windows>
		Could not load plug-in code, dlerror: <text from Linux>
		Plug-in entry point not found
		Invalid plug-in options
		Missing plug-in name
	
	Error 1551
		Compressed contents contain incorrect plug-in extension
		Could not unpack compressed plug-in contents
		Could not create plug-in decompression folder
		Incorrect plug-in extension
		Incorrect plug-in container format

FMXExtern.h
  If C++17 mode is enabled in Xcode or Visual C++, the various ...AutoPtr helper classes based on std::auto_ptr<> will be disabled, as C++17 fully deprecated std::auto_ptr<>.

FMMiniPlugIn.xcodeproj
  Xcode 10 has deprecated 32-bit builds, so the project file has removed the i386 architecture.
  Our switch to Xcode 10 also means that the FMWrapper.framework that plug-ins link against is also now 64-bit only.
  Use a prior release of the Plug-in SDK if support for 32-bit architectures on macOS is required.

List of changes for 17
----------------------

iOS plug-ins can now hook the App delegate
  See the iOS App SDK for more information
External script-steps
  "Select entire contents" option works more consistently with fields and variables
  Missing external script steps display more information in Script Workspace
FMXFixPt.h
  Added 64 bit integer methods AssignInt64, Increment64, Decrement64, and AsLong64
Unified plug-in bundle format
  See above


List of changes for 16
----------------------

FMXCalcEngine.h
  fmx::ExprEnv display and compatibility flag changes
  Support for external script steps via RegisterScriptStep and UnRegisterScriptStep
FMXExtern.h
  kFMXT_HelpURLStr in FMXExtern.h used to return URL to your help web site
FMXText.h
  Added GetBytesEx to fmx::Text


List of changes for 15
----------------------

All headers
  Deprecating all xxxAutoPtr classes in favor of xxxUniquePtr classes
FMXECalcEngine.h
  New SessionID and FileID state information methods for the fmx:ExprEnv class
  New RegisterExternalFunctionEx method adds function description parameter
FMXExtern.h
  New kFMXT_SessionShutdown and kFMXT_FileShutdown main entry point messages


List of earlier changes
-----------------------

Search header files for "New to FileMaker Pro"

