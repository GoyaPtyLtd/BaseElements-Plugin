### BaseElements Plugin Versions

As well as different numerical versions ( 1.0.0 etc ) and different platform versions ( macOS and Windows etc ), the plugin is available in a **Pro** and a **Free** version.

#### What is in the Pro version?

The Pro version contains functionality that is not yet in the Free version, and the Pro version will ordinarily have new functionality added to it before it goes into the Free version.

So as of 5.0.0 the Pro version adds :

* BE_BackgroundTaskAdd: a new function to perform a HTTPPost in a background thread
* BE_BackgroundTaskList: return a value list of ids for pending background tasks
* BE_ContainerConvertImage: convert images between JPG, PNG, GIF and HEIC (read-only) format
* BE_SMTPServer: new parameter to control reuse of the SMTP connection when sending bulk email

**BE_BackgroundTaskAdd**

This is a way to HTTP POST request in a background thread, where you can record the result in a table via an SQL command.  So this will let you upload logs or sync data to a server in the background while the user continues with their other tasks.

**BE_ContainerConvertImage**

This lets you convert image types, so will convert the iOS native HEIC format into JPEG, for example for sending to other services.

**BE_SMTPServer connection re-use**

This lets you open the connection to an SMTP server once when you are sending multiple emails, improving speed and preventing reconnections on every email.

#### Wait, what?  I thought the plugin was free?

Yes it is.  And you don't need to pay if you don't want to.

To explain : when we compile the plugin, we choose the output version we want, from Mac, iOS, Windows, but in the 5.0 branch we now also have Free and Pro version for each platform.  So for every OS there is a Pro version and a Free version.  After we've compiled all the versions, and put them into our share folder, we copy the Free versions to the usual public download links.

Anyone who is a sponsor gets invited to a separate Dropbox share folder that has the Pro versions and the Free versions, and they're able to use either version.

If you really want a Pro version and you're happy to pay, you can become a sponsor and you get a copy automatically.  

If want the Pro version and you don't want to pay, but have some time on your hands then you can build the Pro versions of plugin yourself - it's not that hard.

If you don't have the time or capability to compile a plugin, and can't or don't want to pay, then the additional new functionality in the Pro versions will come to the the free versions at some point in the future.

*Technically also the Pro version is not locked to anyone, it still has no registration, and the sponsors have no legal obligation on them to keep their copies of the Pro plugin to themselves, and not to distribute it publicly.*

We do request that sponsors not put the Pro version on a public website, but we can't stop them if they do.

#### Why do this?

In reality, despite its wide spread use across the FileMaker ecosystem, very few people sponsor.  What little sponsorship there is has never covered the cost of coding the plugin or doing support, and all the time that takes.  Most of the work to make the plugin was done by a single outside contractor so every bit of work was an expense and not just an "outside work hours" project.

We wanted to come up with an approach that didn't detract from the free and open source nature of the plugin, but offered incentives to those who were considering sponsoring, but who never managed to in the past, and also incentivised others to be come involved by directing more people to compiling and coding.

And, honestly we'd love to have others involved in compiling, testing, and coding for the plugin, and any sort of nudge that encourages others to compile the plugins would be fantastic.  It's easier than you think, anyone who has worked in XCode or Visual Studio has the skills to do this already, and you don't need to know how to code c++ to compile an existing working plugin.

We are open to the idea of having other committers with access to the BE plugin repo, and therefore others who benefit from the Sponsor income if the workload starts to be shared around.

#### Lets do more Open Source

So part of this change is now to put all of the documentation, [support channels](https://github.com/GoyaPtyLtd/BaseElements-Plugin/issues), and [community Discussion pages](https://github.com/GoyaPtyLtd/BaseElements-Plugin/discussions) into the GitHub repo here so that everyone can see that this is an open source collaboration and not something "owned" by Goya.

If you find an issue with a function document page, you can submit changes, and we'll incorporate them ( pull requests for those familiar with git ).

All the support will now be done with the [GutHub issues pages](https://github.com/GoyaPtyLtd/BaseElements-Plugin/issues), so others can read and learn from the questions.

We hope this new, more open direction, gives confidence to people to become more involved.

#### What we hope to get out of this

The best outcome would be more widespread adoption of the plugin, with more people involved in coding for it, to reduce things like the delay when new features in the FileMaker platform get adopted but cannot be introduced ( eg linux arm64 support ).

If that translated to more income from Sponsors so that we could dedicate more resources to it, then that would be a great outcome for everyone.

It doesn't matter to us if that income is solely with Goya, or is widespread across the community because of widespread assistance.

#### Corporate Sponsor Options

We've removed for now the $399 company sponsor option and are considering a broader Corporate Sponsor package.  This would be much more aligned with a small number of companies, at a higher financial level, who are more closely aligned with the goals of the plugin.

There's a whole lot of possible option we're considering here :

* New custom dedicated functions.
* Sponsor thanks in the code repo here.
* Custom builds of the plugin, either for branding purposes, or locked down functionality.
* What else ???

If any of this interests you, please contact us.

