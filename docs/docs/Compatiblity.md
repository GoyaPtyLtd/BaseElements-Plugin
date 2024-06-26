## Operating System and FileMaker Compatibility

This document assumes the latest version, and will sometimes also include beta versions.  Some functions won't work on some platforms and some functions will still run but give you false results - we don't trap for platform within the plugin itself, so it's up to the developer to call appropriate functionality for the platform they're developing for.

### OS Compatibility

Short version : BaseElements plugin runs in

- FileMaker Pro on Mac ( Intel or Apple Silicon ) and Windows x86.
- FileMaker Server for Mac ( Intel or Apple Silicon ), Windows x86, Linux ( x86 only ).
- Compiled FileMaker apps for iOS built with the iOS SDK, and under Xcode in the simulator on MacOS.

Plugins in general, ( including BE ) DO NOT run in these platforms as they don't have plugin support as an option :

- FileMaker Go for iOS.
- FileMaker Cloud 2.0 or later.

Other things to note :

Version 4.0 and later is 64 bit only.  So you can't run BE 4.x in FileMaker Pro 32bit for Windows or older versions of Mac that are 32 bit.  Downloads of 32bit BE 3.x are available on the download page.

The BE plugin does not run in Windows XP as of BE version 3.1 and later.

BE plugin versions 4.0.4 and later require Mac OS 10.13 or later.

As of version 4.0.4 the plugin is now thread safe, and will run on FileMaker Server where there are multiple instances of scripts being called at the same time.  Versions prior to 4.0.4 are NOT thread safe and caution should be taken to only use a single instance of the plugin on FMS at one time.

FileMaker version 18 and later require signed ( and notarised on Mac ) versions of the plugin to install without warnings.  All releases from 4.0.5 and later are signed, and older versions can be signed on request.

BE Plugins prior to 4.2.0 run just fine under Apple Silicon such as the M1 under rosetta ( in FileMaker Pro for intel processors ).  BE versions 4.2.0 are also compiled for native compatibility on Apple Silicon.

Version 3.0.x was the last release to run under Windows XP.

BE Plugin Version 1.2 and later requires FMP 11 or later because of the new SQL engine. BE 1.1 will run in ffm Pro 10 and earlier.

## Function Compatibility

Definitions for the table below :

✔︎ : Tested and functional.
✕ : Does not work on this platform.
NA : Does not apply.
No value - untested or functionality is indeterminate.
Active : still in development and supported

Also used :

**Obsolete** : has been made redundant by native FMP functionality, but we haven't yet removed it from the plugin.  Some functions like the HTTP functions can be removed but are in use heavily so we recommend switching to native functions where you can. There are still some small uses cases that the BE function covers that the internal functions don't, so they will remain in the plugin for the foreseeable future.  However, other functions that have exact matching functionality might still have a use : eg the ValuesSort function in BE has shown to be faster in some situations than the internal one.

**Deprecated** : has been made redundant or otherwise no longer supported, and will be removed in a future version, likely in the next major release.  These functions have native alternatives in the current BE release and will no longer appear in the function list, but do still work if used.

**Beta** : new functionality with an as yet indeterminate feature set, so may change, or may even be removed.  Likely to change based on feedback.

**Internal** : a very specific and custom function designed for use in BaseElements and likely not very applicable to the general public.  May not appear in function lists.

Function Name Status Mac FMP Win FMP Mac FMS Win FMS Linux FMS iOS Thread Safe prior to 4.0.4
ArrayChangeValue Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕
ArrayDelete Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕
ArrayFind Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕
ArrayGetSize Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
ArrayGetValue Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
ArraySetFromValueList Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
ArrayDelete Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
CipherDecrypt Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
CipherEncrypt Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
ClipboardFormats Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
ClipboardGetText Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
ClipboardSetText Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
ClipboardGetFile Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
ClipboardSetFile Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
ContainerCompress Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ContainerIsCompressed Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ContainerUncompress Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ContainerConvert Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ContainerGetType Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ContainerListTypes Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
DialogDisplay Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
DialogProgress Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
DialogProgressUpdate Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
Decrypt_AES Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
Encrypt_AES Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
EvaluateJavaScript Beta ✔︎ ✔︎ ✕ ✕ ✔︎ ✔︎ NA
ExecuteSystemCommand Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕ ✕
ExportFieldContents Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileCopy Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileDelete Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileExists Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileImport Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FilesListInFolder Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileMakerSQL Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
FileModificationTimestamp Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileMove Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileOpen Active ✔︎ ✔︎ ✔︎ ✔︎ ✕ ✕ ✔︎  
FileReadText Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileSaveDialog Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
FileSelectDialog Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
FileSize Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FileWriteText Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FolderCreate Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FolderSelectDialog Active ✔︎ ✔︎ ✕ ✕ ✕ ✕ NA
GetLastDDLError Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
GetLastError Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
GetMachineName Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
GetSystemDrive Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
Gzip Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
JPEG_Recompress Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
OAuth_RequestAccessToken Beta ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
JSON_ArraySize Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
OpenURL Active ✔︎ ✔︎ ✕ ✕ ✕ ✔︎ NA
MessageDigest Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
Pause Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
PDFAppend Active ✔︎ ✔︎ ✕ ✕ ✔︎ NA
PDFPageCount Active ✔︎ ✔︎ ✕ ✕ ✔︎ NA
PDFGetPages Active ✔︎ ✔︎ ✕ ✕ ✔︎ NA
PreferenceGet Active ✔︎ ✔︎ ✔︎ ✔︎ ✕ ✔︎ ✔︎  
PreferenceSet Active ✔︎ ✔︎ ✔︎ ✔︎ ✕ ✔︎ ✔︎  
PreferenceDelete Active ✔︎ ✔︎ ✔︎ ✔︎ ✕ ✔︎ ✔︎  
RegularExpression Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ScriptExecute Active ✔︎ ✔︎ ✕ ✕ ✔︎ ✔︎ NA
ScriptStepInstall Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ScriptStepRemove Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ScriptStepPerform Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
SetTextEncoding Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
SignatureGenerate_RSA Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
SignatureVerify_RSA Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
StackPush Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
StackPop Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
StackCount Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
StackDelete Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
SMTP_AddAttachment Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
SMTP_Send Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
SMTP_Server Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
SMTP_SetHeader Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
TimeZoneOffset Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
TimeCurrentMilliseconds Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
UnGzip Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
Unzip Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
TimeUTCMilliseconds Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ValuesContainsDuplicates Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ValuesFilterOut Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ValuesSort Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ValuesTimesDuplicated Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ValuesTrim Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
ValuesUnique Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
VariableGet Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
VariableSet Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ NA
Vector_DotProduct Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Vector_EuclideanDistance Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Version Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
VersionAutoUpdate Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Xero_GenerateKeys Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
Xero_SetTokens Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✕  
XML_Parse Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
XML_Validate Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
XOR Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
XPath Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
XPathAll Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
XSLTApply Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
XSLTApplyInMemory Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Zip Active ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎

SplitBEFileNodes Internal ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
StripXMLNodes Internal ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
ExtractScriptVariables Internal ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎

FTP_Delete Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
FTP_Upload Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎  
JSONPath Deprecated ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
JSON_Encode Deprecated ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
JSON_Error_Description Deprecated ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Curl_SetOption Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Curl_Trace Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HMAC Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_DELETE Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_GET Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_GET_File Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_PATCH Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_POST Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_PUT_DATA Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_PUT_FILE Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_Response_Code Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_Response_Headers Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_Set_Custom_Header Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
HTTP_Set_Proxy Obsolete ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Base64_Decode Deprecated ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Base64_Encode Deprecated ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
Base64_URL_Encode Deprecated ✔︎ ✔︎ ✔︎ ✔︎ ✔︎ ✔︎
