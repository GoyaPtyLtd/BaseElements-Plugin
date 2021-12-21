/*
 BEMailRecipient.h
 BaseElements Plug-In
 
 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEMAILRECIPIENT__
#define __BaseElements__BEMAILRECIPIENT__


#include "BEPluginGlobalDefines.h"


#include <string>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <Poco/Net/MailRecipient.h>
#pragma GCC diagnostic pop


class BEMailRecipient : public Poco::Net::MailRecipient {
	
public:
	
	explicit BEMailRecipient ( ) : Poco::Net::MailRecipient ( ) { }
	BEMailRecipient ( RecipientType type, const std::string& name_and_address );

	const std::string bracketed_address() { return "<" + getAddress() + ">"; }

protected:
	
};



#endif /* defined(__BaseElements__BEMAILRECIPIENT__) */
