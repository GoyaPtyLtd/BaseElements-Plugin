/*
 BESMTPEmailMessage.cpp
 BaseElements Plug-In
 
 Copyright 2014-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTPEmailMessage.h"
#include "BEPluginException.h"

#include <sstream>


using namespace std;


#pragma mark -
#pragma mark Constructors
#pragma mark -


BESMTPEmailMessage::BESMTPEmailMessage ( std::string _from, std::string _to, std::string _subject, std::string _text )
{
	
	from = _from;
	to = BEValueList<string> ( _to );
	subject = _subject;

	text = new mimetic::MimeEntity;
	text->body().assign ( _text );
	text->header().contentType() = "text/plain";
	
}


#pragma mark -
#pragma mark Methods
#pragma mark -


void BESMTPEmailMessage::set_html_alternative ( std::string html_part )
{
	html = new mimetic::MimeEntity;
	html->body().assign ( html_part );
	html->header().contentType() = "text/html";
}


void BESMTPEmailMessage::add_attachment ( const boost::filesystem::path path_to_attachment )
{
	boost::filesystem::path attachment = path_to_attachment;
	if ( exists ( attachment ) ) {
		attachments.append ( attachment.string() );
	} else {
		throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
	}
}


void BESMTPEmailMessage::add_attachments ( BEValueList<wstring> attachment_list )
{
		
	for ( size_t i = 0 ; i < attachment_list.size() ; i++ ) {
		
		boost::filesystem::path attachment = attachment_list.at ( i );
		if ( exists ( attachment ) ) {
			attachments.append ( attachment.string() );
		} else {
			throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
		}

	}

}


string BESMTPEmailMessage::as_string()
{
	build_message();
	return body;
}


#pragma mark -
#pragma mark Protected Methods
#pragma mark -


void BESMTPEmailMessage::build_message ( )
{
	
	auto_ptr<mimetic::MimeEntity> message ( new mimetic::MultipartMixed );

	if ( cc.not_empty() ) {
		message->header().cc ( cc.get_as_comma_separated() );
	}
	
	if ( bcc.not_empty() ) {
		message->header().bcc ( bcc.get_as_comma_separated() );
	}
	

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
	
	for ( size_t i = 0 ; i < attachments.size() ; i++ ) {
		
		string file = attachments.at ( i ).string();
		mimetic::Attachment * attach_this = new mimetic::Attachment ( file );
		message->body().parts().push_back ( attach_this );

	}

	message->header().from ( from );
	message->header().subject ( subject );
	message->header().to ( to.get_as_comma_separated() );
	
	ostringstream meassage_stream;
	meassage_stream << *message << endl;
	body = meassage_stream.str();

//	delete message;

} // build_payload



