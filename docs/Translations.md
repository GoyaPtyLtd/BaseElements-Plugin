## Translations and Localisation

The BaseElements plugin includes English language function names, and also defaults to English function descriptions that appear whenever you view them in FileMaker 16 or later.

If you would like to assist us in translating the functions, or descriptions for the BE plugin, then the file to translate is called "Localizable.strings" and can be found on GitHub here :

https://github.com/GoyaPtyLtd/BaseElements-Plugin/blob/master/Resources/Base.lproj/Localizable.strings

### How can I help translate these?

The way it works is that each function has a line associated with it, and comes in three parts, separated by a pipe | character.  For example :

	"118" = "BE_MoveFile ( fromPath ; toPath )|File Move|Moves the file specified in the from parameter, to the location in the to parameter.\n\nNote : on Mac OS X, the Move operation only works if the source and Destination are on the same volume. To move files across volumes, use a Copy and then Delete the original.\n";

 The first part, e.g. **BE_MoveFile ( fromPath ; toPath )** is the function name and parameters. It can be localised, but obviously needs to keep the same number of parameters and use similar descriptors.  We try to follow similar patterns across all functions for parameter names, so calling things "container" when referring to a container field, or appending "Boolean" such as retainEmptyBoolean for True/False type parameters.  In short, use what makes sense for the language, but keep them consistent across the whole set.
 
 The second part **File Move** is a space separated list of words that you can use for type ahead for the function in the calculation engine.  So type "Mo" or "FM" and it will find this function based on the order of these words.  You may leave the English words intact and add the other language version, or replace them altogether depending on how your users expect this to behave.   
 
 The third part is the description that appears in the bottom right hand corner of the calculation dialog.  This is the most important part to translate.  You can add return characters with \n and have it all appear.  So just replace the English version with the equivalent translated text.
 
Contact us for details via Issues for adding a translation to GitHub, and we'll suggest path and location for the addition.  

We will happily accept any translations our users are willing to offer.
