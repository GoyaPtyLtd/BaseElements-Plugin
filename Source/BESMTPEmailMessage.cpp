/*
 BESMTPEmailMessage.cpp
 BaseElements Plug-In
 
 Copyright 2014-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTPEmailMessage.h"
#include "BEPluginException.h"
#include "BECurl.h"

#include <sstream>


using namespace std;


#pragma mark -
#pragma mark Constructors
#pragma mark -


BESMTPEmailMessage::BESMTPEmailMessage ( const std::string& from, const std::string& to, const std::string& subject, const std::string& message_body )
{
	
	message = new mimetic::MultipartMixed;

	auto_ptr< BEValueList<string> > mail_to ( new BEValueList<string> ( to ) );
	message->header().to ( mail_to->get_as_comma_separated() );

	message->header().from ( from );
	message->header().subject ( subject );

	text = new mimetic::MimeEntity;
	text->body().assign ( message_body );
	text->header().contentType() = "text/plain";

}


BESMTPEmailMessage::~BESMTPEmailMessage ( )
{
	delete message;
}


#pragma mark -
#pragma mark Methods
#pragma mark -


std::auto_ptr< BEValueList<std::string> > BESMTPEmailMessage::get_address_list ( const std::string& addresses )
{
	std::auto_ptr< BEValueList<std::string> > addresses_list ( new BEValueList<string> ( addresses, ", ", true ) );
	return addresses_list;
};


void BESMTPEmailMessage::set_cc_addresses ( const std::string& email_addresses )
{
	auto_ptr< BEValueList<string> > cc ( new BEValueList<string> ( email_addresses ) );
	if ( cc->not_empty() ) {
		message->header().cc ( cc->get_as_comma_separated() );
	}
}


void BESMTPEmailMessage::set_bcc_addresses ( const std::string& email_addresses )
{
	auto_ptr< BEValueList<string> > bcc ( new BEValueList<string> ( email_addresses ) );
	if ( bcc->not_empty() ) {
		message->header().bcc ( bcc->get_as_comma_separated() );
	}

}


void BESMTPEmailMessage::set_reply_to ( const std::string& reply_to_address )
{
	if ( ! reply_to_address.empty() ) {
		message->header().replyto ( reply_to_address );
	}

}


void BESMTPEmailMessage::set_html_alternative ( const std::string& html_part )
{
	html = new mimetic::MimeEntity;
	html->body().assign ( html_part );
	html->header().contentType() = "text/html";
}


void BESMTPEmailMessage::add_attachment ( const boost::filesystem::path path_to_attachment )
{
	if ( exists ( path_to_attachment ) ) {
		mimetic::Attachment * attach_this = new mimetic::Attachment ( path_to_attachment.string() );
		message->body().parts().push_back ( attach_this );
	} else {
		throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
	}
}


void BESMTPEmailMessage::add_attachments ( const BEValueList<wstring>& attachment_list )
{
		
	for ( size_t i = 0 ; i < attachment_list.size() ; i++ ) {
		boost::filesystem::path attachment = attachment_list.at ( i );
		add_attachment ( attachment );
	}

}


string BESMTPEmailMessage::as_string()
{
	build_message();

	ostringstream meassage_stream;
	meassage_stream << *message << endl;
	return meassage_stream.str();

}


#pragma mark -
#pragma mark Protected Methods
#pragma mark -


void BESMTPEmailMessage::build_message ( )
{
	
	if ( !text->body().empty() && !html->body().empty() ) {

		mimetic::MultipartAlternative * multipart_alternative = new mimetic::MultipartAlternative;
		multipart_alternative->body().parts().push_back ( text );
		multipart_alternative->body().parts().push_back ( html );
		message->body().parts().push_back ( multipart_alternative );

	} else if ( !text->body().empty() ) {
		message->body().parts().push_back ( text );
	} else if ( !html->body().empty() ) {
		message->body().parts().push_back ( html );
	}
	
} // build_payload

