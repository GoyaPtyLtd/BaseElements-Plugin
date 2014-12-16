/*
 BESMTPEmailMessage.h
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
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
	
	BESMTPEmailMessage ( std::string host, std::string port, std::string username, std::string password );
	
	BEValueList<std::string> to_address ( ) { return to; };
	std::string from_address ( ) { return from; };
	BEValueList<std::string> cc_addresses ( ) { return cc; };
	BEValueList<std::string> bcc_addresses ( ) { return bcc; };
	
	void set_cc_addresses ( std::string email_addresses ) { cc = BEValueList<std::string> ( email_addresses ); };
	void set_bcc_addresses ( std::string email_addresses ) { bcc = BEValueList<std::string> ( email_addresses ); };
	
	void set_html_alternative ( std::string html_part );
	void add_attachment ( std::string path_to_attachment );
	void add_attachments ( BEValueList<std::string> attachment_list );
	
	std::string as_string ( );
	
	
protected:
	
	std::string from;
	BEValueList<std::string> to;
	std::string subject;
	std::string body;
	
	BEValueList<std::string> cc;
	BEValueList<std::string> bcc;
	
	mimetic::MimeEntity text;
	mimetic::MimeEntity html;

	BEValueList<boost::filesystem::path> attachments;
	
	void build_message ( );
	
};


#endif /* defined(__BaseElements__BESMTPEmailMessage__) */
