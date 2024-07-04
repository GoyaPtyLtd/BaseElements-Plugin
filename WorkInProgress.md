## Work in Progress

This is mostly a note taking section where we document what is happening and where things are at so that the other documentation can be mostly around having a current state document.

The big thing we started a while back was to resolve an issue with the Boost library that needed to be updated.  However the instructions we had were incomplete and outdated.  We needed a full set of documents for anyone to be able to compile any library on any platform, and then incorporate their new library version into the main plugin repo.

So we started [https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries](https://github.com/GoyaPtyLtd/BaseElements-Plugin-Libraries) which is still a work in progress.

The documentation for the compiling on the Mac works 100% to get to the "compiled library" stage.  However each new version of the library has changes in it that may require code changes in the main BE plugin.

## Everything starts with the Mac version

So we've currently managed to bring across and test these new libraries into the Mac version, and have them compile a working plugin :

* Boost

We currently have guessed that these libraries may be able to be copied across from libraries to main plugin without issue, it just requires someone to do each one at a time, then test locally, and confirm that the plugin passes tests once compiled.

* duktape
* images ( jpeg, libde, libheif, openjp, ImageMagick )
* jq
* xml ( iconv, xml, xslt )

We currently know that these libraries will require work to adapt the current code to changes in the library :

* curl ( openssl, libssh, curl, poco.  openssl has known changes when going from 1.x to 3.x )
* font ( freetype, fontconfig, podofo. Podofo has had major changes to it's API )

So these libraries can't be updated until the changes are made in our plugin code to adapt to the changed library.

## After Mac is Linux

The second platform it should be relatively easy to work on will be the linux one, as the scripts to compile on Mac and on Linux are going to be very similar.

There are already shell scripts to compile on linux but they are just copies of bits and pieces from other places and are incomplete.  They need to be mapped out the same as the Mac versions, then updated with the same compile options until everything compiles.

Then we can push any updated compiled libraries ( boost ) into the linux branch of the main plugin.

Plus we can work on compiling all the libraries on arm64, so that we can build an arm64 version of the plugin.

## Finally comes Windows

At this point I haven't even considered how to best script the library compilation on Windows, it's not my forte and I'll love some help from someone who knows Windows better than I do.

