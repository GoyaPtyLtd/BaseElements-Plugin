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

### Compatibility Testing

You can help test FileMaker Pro and Server compatibility for us by using our built in test suite.  Download the [BaseElements Plugin Test](https://github.com/GoyaPtyLtd/BaseElements-Plugin/blob/master/Project/Extras/BaseElements%20Plugin%20Tests.fmp12) file and open it either in FMP or FMP from FMS.  You can then run the tests either locally or on the server, and report back to us which versions and OS etc were in use, and we will document them [on our issues page](https://github.com/GoyaPtyLtd/BaseElements-Plugin/issues).

For a production release of the plugin, all tests should either pass or be skipped ( incompatible mostly ) so anything else is noteworthy.

### Other things to note :

Version 4.0 and later is 64 bit only.  So you can't run BE 4.x in FileMaker Pro 32bit for Windows or older versions of Mac that are 32 bit.  Downloads of 32bit BE 3.x are available on the download page.

The BE plugin does not run in Windows XP as of BE version 3.1 and later.  Version 3.0.x was the last release to run under Windows XP.

BE plugin versions 4.0.4 and later require Mac OS 10.13 or later.

As of version 4.0.4 the plugin is now thread safe, and will run on FileMaker Server where there are multiple instances of scripts being called at the same time.  Versions prior to 4.0.4 are NOT thread safe and caution should be taken to only use a single instance of the plugin on FMS at one time.

FileMaker version 18 and later require signed ( and notarised on Mac ) versions of the plugin to install without warnings, and all releases from 4.0.5 and later are signed.

BE Plugins prior to 4.2.0 run just fine under Apple Silicon such as the M1 under rosetta ( in FileMaker Pro for intel processors ).  BE versions 4.2.0 are also compiled for native compatibility on Apple Silicon.

BE Plugin Version 1.2 and later requires FMP 11 or later because of the new SQL engine. BE 1.1 will run in FileMaker Pro 10 and earlier.
