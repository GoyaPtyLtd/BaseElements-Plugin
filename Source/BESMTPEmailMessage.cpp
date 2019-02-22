/*
 BESMTPEmailMessage.cpp
 BaseElements Plug-In
 
 Copyright 2014-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTPEmailMessage.h"
#include "BEPluginException.h"
#include "BECurl.h"
#include "Crypto/BEBase64.h"

#include <Poco/Timestamp.h>
#include <Poco/Timezone.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>

#include <sstream>


using namespace std;


thread_local CustomHeaders g_smtp_custom_headers;


#pragma mark -
#pragma mark Constructors
#pragma mark -


BESMTPEmailMessage::BESMTPEmailMessage ( const std::string& from, const std::string& to, const std::string& subject, const std::string& message_body )
{
	
	message = new BEMimeticMIMEEntity;

	unique_ptr< BEValueList<string> > mail_to ( new BEValueList<string> ( to ) );
	message->header().to ( mail_to->get_as_comma_separated() );

	message->header().from ( from );
	
	auto base64_subject = Base64_Encode ( subject );
	auto encoded_subject = "=?utf-8?B?" + base64_subject + "?=";
	message->header().subject ( subject );

	text = new mimetic::MimeEntity;
	auto message_text = message_body;
	boost::replace_all ( message_text, FILEMAKER_END_OF_LINE, "\r\n" );
	text->body().assign ( message_text + "\r\n\r\n" );
	text->header().contentType() = "text/plain; charset=\"utf-8\"";
	
	// MIME Version
	mimetic::MimeVersion mime_version = mimetic::MimeVersion ( "1.0" );
	message->header().mimeVersion ( mime_version );
	
	// rfc 1123 (rfc 822) date header
	Poco::LocalDateTime now;
	auto rfc1123_date = Poco::DateTimeFormatter::format ( now.timestamp(), Poco::DateTimeFormat::RFC1123_FORMAT, Poco::Timezone::tzd() );
	g_smtp_custom_headers [ "Date" ] = rfc1123_date;
	
	// custom headers
	CustomHeaders::const_iterator it = g_smtp_custom_headers.begin();
	while ( it != g_smtp_custom_headers.end() ) {
		
		mimetic::Field smtp_header;
		smtp_header.name ( it->first );
		smtp_header.value ( it->second );
		message->header().push_back ( smtp_header );
	
		++it;
	}
	
	std::shared_ptr<BEValueList<std::string> > bcc ( new BEValueList<std::string> ( "" ) );
	bcc_address_list = bcc;

}


BESMTPEmailMessage::~BESMTPEmailMessage ( )
{
	g_smtp_custom_headers.clear();
	delete message;
}


#pragma mark -
#pragma mark Methods
#pragma mark -


std::unique_ptr< BEValueList<std::string> > BESMTPEmailMessage::get_address_list ( const std::string& addresses )
{
	std::unique_ptr< BEValueList<std::string> > addresses_list ( new BEValueList<string> ( addresses, ",", true, false ) );
	addresses_list->trim_values();
	
	return addresses_list;
};


void BESMTPEmailMessage::set_cc_addresses ( const std::string& email_addresses )
{
	unique_ptr< BEValueList<string> > cc ( new BEValueList<string> ( email_addresses ) );
	if ( cc->not_empty() ) {
		message->header().cc ( cc->get_as_comma_separated() );
	}
}


void BESMTPEmailMessage::set_bcc_addresses ( const std::string& email_addresses )
{
	bcc_address_list->clear();
	unique_ptr< BEValueList<string> > bcc ( new BEValueList<string> ( email_addresses ) );
	bcc_address_list->append ( *bcc );
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
	auto html_text = html_part;
	boost::replace_all ( html_text, FILEMAKER_END_OF_LINE, "\r\n" );
	html->body().assign ( html_text );
	html->header().contentType() = "text/html; charset=\"utf-8\"";
}


void BESMTPEmailMessage::add_attachment ( const BESMTPContainerAttachment new_attachment )
{
	if ( exists ( new_attachment.first ) ) {

		boost::filesystem::path attachment = new_attachment.first;
		attachment.make_preferred();

		mimetic::Attachment * attach_this = new mimetic::Attachment (
																	 attachment.string(),
																	 mimetic::ContentType ( new_attachment.second ),
																	 mimetic::Base64::Encoder()
																	 );
		
		message->body().parts().push_back ( attach_this );
		
	} else {
		throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
	}
}


void BESMTPEmailMessage::add_attachments ( void )
{
		
	for ( size_t i = 0 ; i < attachments.size() ; i++ ) {
		add_attachment ( attachments.at ( i ) );
	}

}


void BESMTPEmailMessage::set_attachments ( const BESMTPContainerAttachmentVector& attachment_list )
{
	attachments = attachment_list;
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

	add_attachments();
	
} // build_message


