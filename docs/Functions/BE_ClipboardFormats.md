## BE_ClipboardFormats

    BE_ClipboardFormats

**Description**  

Returns a return value list of the format types of the content of the current clipboard.

Once you've retrieved this list, get one of the types of this output using the native *GetValue* function, and pass that as the parameter to *BE_ClipboardGetText* or *BE_ClipboardGetFile* to get clipboard content.

You can then also use *BE_ClipboardSetText* or *BE_ClipboardSetFile* when setting onto the clipboard using the same type value.

**Parameters**

**Keywords**  

Clipboard Formats

**Version History**

* 1.0.0 : First Release
* 4.0.0 : Returns error 3, command not available on iOS and Linux

**Notes**

The clipboard can copy one type of content, but will often have multiple copies in different formats stored.  For example, copying FileMaker Script Steps on the mac will give the formats :

	dyn.ah62d4rv4gk8zuxnxnq
	CorePasteboardFlavorType 0x584D5353

or copying plain text from FileMaker on the Mac will give :

	com.apple.traditional-mac-plain-text
	CorePasteboardFlavorType 0x54455854
	public.utf16-plain-text
	CorePasteboardFlavorType 0x75747874
	public.utf8-plain-text
	NSStringPboardType
	dyn.ah62d4rv4gk8yqxpdsq
	CorePasteboardFlavorType 0x464D6373
	public.rtf
	NeXT Rich Text Format v1.0 pasteboard type

So it's possible to utilise different types depending on your intended use.  The different formats can be quite different, content wise, for example if you copied RichText with formatting, you clipboard may contain types of :

	public.rtf
	public.utf8-plain-text
	
Only the first type would retain the format information.

**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | No |  
| iOS | No  |  
| Linux | No  |  

**Example Code**

