The BaseElements plugin is a free and open source plugin, available to everyone to extend the built in functions of the Claris FileMaker Platform.

It is open source and has no registration requirements, or anything within it that reports usage, so is entirely private and unrestricted.

## Functionality

The BaseElements plugin has functions for file manipulation, dialogs, XSLT operations, clipboard manipulation, performing shell scripts and FileMaker scripts, internal SQL operations, MD5 and SHA hashing and more. There is a full list of the functions and the details on this site.

## History

Originally the plugin was built as a small plugin to do three things ( a custom dialog, file writing, and clipboard set/get ) in the BaseElements developer tool. There were already other plugins in the FileMaker world that did these things, but not in a single plugin, it would have required at least 2 or 3 to work. And because at the time, that was all we needed, it was simpler and easier to write our own.

On top of that we were already licensing the BaseElements developer tool itself, it made no sense to licence the plugin as well, so we made it free. And because we didn’t see why not, we made it open source too as it was mostly based on a couple of small open source libraries anyway.

Over time we found more and more things we’d like to do with it, and once you had implemented a particular library, like curl for HTTP GET then you could use the same code for other curl functions like POST, SMTP or SFTP. So over time the plugin functionality grew.

Without much effort from us in terms of marketing, and with minimal support, the BaseElements plugin became a sort of defacto add on for any FileMaker solution, and a very widely used plugin in lots of free and commercial FileMaker solutions.

## Future

For a long time, we’ve had a small “sponsorship” option available on the website. In theory this gave you access to new builds quickly and a support channel. In reality we mostly made all the builds available and pretty much answered everyone’s questions, regardless of sponsor status.

The sponsor options haven’t been widely taken up and don’t cover even a small fraction of the ongoing cost of development and support. Most development has been done by a single contractor, and there’s only been one significant outside code contribution. Depsite the widespread use of the plugin, there hasn't been much in the way of other code or development contributions.

We're hoping that by making the plugin easier to build by non advanced developers, and moving all the infrastructure ( docs, help, sponsorships ) to open platforms here in GitHub that more people see fit to contribute, and in doing that the plugin becomes more community owned.
