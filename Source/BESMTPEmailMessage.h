/*
 BESMTPEmailMessage.h
 BaseElements Plug-In
 
 Copyright 2014-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BESMTPEmailMessage__
#define __BaseElements__BESMTPEmailMessage__


#include "BEPluginGlobalDefines.h"
#include "BEValueList.h"

#include <boost/filesystem.hpp>

#include <mimetic/mimetic.h>

#include <string>
#include <vector>



class BESMTPEmailMessage {

public:
	
	BESMTPEmailMessage ( const std::string& from, const std::string& to, const std::string& subject, const std::string& message_body );
	~BESMTPEmailMessage ( );
	
	BEValueList<std::string> to_address ( ) { return message->header().to().str(); };
	std::string from_address ( ) { return message->header().from().str(); };

	std::auto_ptr< BEValueList<std::string> > cc_addresses ( ) { return get_address_list ( message->header().cc().str() ); };
	std::auto_ptr< BEValueList<std::string> > bcc_addresses ( ) { return get_address_list ( message->header().bcc().str() ); };

	void set_cc_addresses ( const std::string& email_addresses );
	void set_bcc_addresses ( const std::string& email_addresses );
	void set_reply_to ( const std::string& reply_to_address );
	void set_html_alternative ( const std::string& html_part );
	void add_attachment ( const boost::filesystem::path path_to_attachment );
	void add_attachments ( const BEValueList<std::wstring>& attachment_list );
	
	std::string as_string ( );
	
	
protected:

	mimetic::MimeEntity * message;
	mimetic::MimeEntity * text;
	mimetic::MimeEntity * html;

	BEValueList<boost::filesystem::path> attachments;
	
	std::auto_ptr< BEValueList<std::string> > get_address_list ( const std::string& addresses );
	void build_message ( );
	
};


#endif /* defined(__BaseElements__BESMTPEmailMessage__) */
