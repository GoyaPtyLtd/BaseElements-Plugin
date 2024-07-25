/*
 BELinuxFunctions.cpp
 BaseElements Plug-In

 Copyright 2016-2023 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEPluginGlobalDefines.h"
#include "BELinuxFunctions.h"
#include "BELinuxFunctionStrings.h"
#include "BEPluginUtilities.h"
#include "BEPluginException.h"

#include <map>

#include <unistd.h>
#include <limits.h>


using namespace std;


void InitialiseForPlatform ( )
{
}


const std::wstring ClipboardFormats ( void )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


const std::string ClipboardText ( const std::wstring& atype )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return "";
}


const bool SetClipboardText ( const std::string& data, const std::wstring& atype )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


const std::vector<unsigned char> ClipboardFile ( const std::wstring& atype )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

    // should never get here
    const std::vector<unsigned char> empty;

	return empty;
}


const bool SetClipboardFile ( const std::vector<unsigned char>& data, const std::wstring& atype )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


std::wstring SelectFile ( std::wstring& prompt, std::wstring& in_folder )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


std::wstring SelectFolder ( std::wstring& prompt, std::wstring& in_folder )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


std::wstring SaveFileDialog ( std::wstring& prompt, std::wstring& fileName, std::wstring& inFolder )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


int DisplayDialog ( std::wstring& title, std::wstring& message, std::wstring& ok_button, std::wstring& cancel_button, std::wstring& alternate_button )
{
	return kCommandIsUnavailableError;
}


// Progress Dialog

fmx::errcode DisplayProgressDialog ( const std::wstring& title, const std::wstring& description, const long maximum, const bool can_cancel )
{
	return kCommandIsUnavailableError;
}


fmx::errcode UpdateProgressDialog ( const long value, const std::wstring& description )
{
	return kCommandIsUnavailableError;
}



// user preferences

bool SetPreference ( std::string& key, std::string& value, std::string& domain )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


std::string GetPreference ( std::string& key, std::string& domain )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return "";
}


void GetPreference ( std::wstring& key, std::wstring& domain )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );
}


void DeletePreference ( std::string& key, std::string& domain )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );
}



// other

bool OpenURL ( std::wstring& url )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


bool OpenFile ( std::wstring& path )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


const std::wstring get_machine_name ( )
{

	char host_name [ HOST_NAME_MAX ];
	const size_t error = gethostname ( host_name, HOST_NAME_MAX );

	wchar_t w_host_name [ HOST_NAME_MAX * 4 ];
	if ( error != kErrorUnknown ) {
		const size_t length = mbstowcs ( w_host_name, host_name, HOST_NAME_MAX * 4 );
		if ( length == kErrorUnknown ) {
			throw BEPlugin_Exception ( errno );
		}
	} else {
		throw BEPlugin_Exception ( errno );
	}

	return w_host_name;

}

const std::string get_system_drive ( )
{
	return ""; // matching Get ( SystemDrive )
}


unsigned long Sub_LoadString ( const unsigned long string_id, FMX_Unichar * intoHere, const long intoHereMax )
{
	unsigned long returnResult = 0;

	std::string wanted;
	try {
		wanted = function_strings.at ( string_id );

		if ( kFMXT_AppConfigStr == string_id  || PLUGIN_DESCRIPTION_STRING_ID == string_id ) {
            wanted.replace ( wanted.find ( "%@" ), 2, PLUGIN_DESCRIPTION_STRING_ID_STRING );
		}

	} catch ( exception& /* e */ ) {
		returnResult = kRequestedDataIsMissingError;
	}

	if( (intoHere != NULL) && (intoHereMax > 1) ) {

		fmx::TextUniquePtr text;
		text->Assign ( wanted.c_str() );
		text->GetUnicode ( intoHere, 0, fmx::Text::kSize_End );
		intoHere[wanted.size()] = 0x0000;

	} else {
		returnResult = -123;
	}


	return returnResult;

} // Sub_LinuxLoadString

