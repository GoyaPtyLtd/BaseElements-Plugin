macos-debug: 
	rm -r /Users/$(USER)/Library/Application\ Support/FileMaker/Extensions/BaseElements.fmplugin
	xcodebuild -project BaseElements.xcodeproj -target macOS -configuration Debug  

macos-debug-custom-sign: 
	rm -r /Users/$(USER)/Library/Application\ Support/FileMaker/Extensions/BaseElements.fmplugin
	xcodebuild -project BaseElements.xcodeproj -target macOS -configuration Debug CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO 
	codesign -s "$(CODESIGN_CERTIFICATE_NAME)" /Users/$(USER)/Library/Application\ Support/FileMaker/Extensions/BaseElements.fmplugin


macos-debug-clean:
	rm -r  /tmp/Xcode/Build/BaseElements.build/Debug

macos-release: 
	rm -r /Users/$(USER)/Library/Application\ Support/FileMaker/Extensions/BaseElements.fmplugin
	xcodebuild -project BaseElements.xcodeproj -target macOS -configuration Release 

macos-release-custom-sign: 
	rm -r /Users/$(USER)/Library/Application\ Support/FileMaker/Extensions/BaseElements.fmplugin
	xcodebuild -project BaseElements.xcodeproj -target macOS -configuration Release CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO 
	codesign -s "$(CODESIGN_CERTIFICATE_NAME)" /Users/$(USER)/Library/Application\ Support/FileMaker/Extensions/BaseElements.fmplugin


macos-release-clean:
	rm -r /tmp/Xcode/Build/BaseElements.build/Release

mac-clean: macos-release-clean macos-debug-clean
	
ios-debug: ios-plugin-clean
	xcodebuild -project BaseElements.xcodeproj -target iOS -configuration Debug

ios-plugin-clean:
	rm -r Extras/BaseElements_iOS_Test/BaseElements_iOS_Test/Plug-Ins/BaseElementsIOS.fmplugin || true
	
ios-debug-clean:
	rm -r /tmp/Xcode/Build/BaseElements.build/Debug-iphoneos

ios-release: ios-plugin-clean
	xcodebuild -project BaseElements.xcodeproj -target iOS -configuration Release

ios-release-clean:
	rm -r /tmp/Xcode/Build/BaseElements.build/Release-iphoneos

ios-clean: ios-debug-clean ios-release-clean

ios-simulator-debug: ios-plugin-clean
	rm -r Extras/BaseElementsIOSPluginTest/BaseElementsIOSPluginTest/Plug-Ins/BaseElementsIOSSimulator.fmplugin || true
	xcodebuild -project BaseElements.xcodeproj -target iOSSimulator -configuration Debug -sdk iphonesimulator13.2 

ios-simulator-release: ios-plugin-clean
	rm -r Extras/BaseElementsIOSPluginTest/BaseElementsIOSPluginTest/Plug-Ins/BaseElementsIOSSimulator.fmplugin || true
	xcodebuild -project BaseElements.xcodeproj -target iOSSimulator -configuration Release -sdk iphonesimulator13.2 

windows-release:
	msbuild /p:Configuration=Release
windows-debug:
	msbuild /p:Configuration=Debug

	
	