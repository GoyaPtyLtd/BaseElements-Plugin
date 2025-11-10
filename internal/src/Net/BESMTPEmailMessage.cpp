/*
 BESMTPEmailMessage.cpp
 BaseElements Plug-In
 
 Copyright 2014-2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESMTPEmailMessage.h"
#include "BEPluginException.h"
#include "BECurl.h"
#include "Net/BEMailRecipient.h"
#include "BESMTP.h"

#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/MediaType.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Timestamp.h>

#include <sstream>


using namespace std;


thread_local CustomHeaders g_smtp_custom_headers;


#pragma mark -
#pragma mark Constructors
#pragma mark -


BESMTPEmailMessage::BESMTPEmailMessage ( const std::string& from, const BEValueListStringSharedPtr to, const std::string& subject, const std::string& message_body, const std::string& html_alternative )
{
	
	set_addresses ( Poco::Net::MailRecipient::PRIMARY_RECIPIENT, to );
	message.setSender ( from );
	message.setSubject ( Poco::Net::MailMessage::encodeWord ( subject, UTF8 ) );

	text = message_body;
	boost::replace_all ( text, LINE_FEED, FILEMAKER_END_OF_LINE );
	boost::replace_all ( text, FILEMAKER_END_OF_LINE, NETWORK_ENDL );

	html = html_alternative;
	boost::replace_all ( html, LINE_FEED, FILEMAKER_END_OF_LINE );
	boost::replace_all ( html, FILEMAKER_END_OF_LINE, NETWORK_ENDL );

	// headers
	message.setDate ( Poco::Timestamp() ); // rfc 1123 (rfc 822) date header
	message.set ( Poco::Net::MailMessage::HEADER_MIME_VERSION, "1.0" ); // MIME Version
	message.set ( "X-Mailer", string ( USER_AGENT_STRING ) + " ( " + string ( AUTO_UPDATE_VERSION ) + " )" ); // who we are
	
	// custom headers
	for ( auto it = g_smtp_custom_headers.begin() ; it != g_smtp_custom_headers.end() ; it++ ) {
		message.set ( it->first, it->second );
	}
	
	
} // BESMTPEmailMessage


BESMTPEmailMessage::~BESMTPEmailMessage ( )
{
	g_smtp_custom_headers.clear();
}


#pragma mark -
#pragma mark Methods
#pragma mark -


BEValueListStringUniquePtr BESMTPEmailMessage::recipients()
{
	BEValueListStringUniquePtr addresses_list ( new BEValueList<string> ( "", ",", true, false ) );

	auto send_to = message.recipients();
	
	
	for ( auto const& recipient : send_to ) {
		addresses_list->append ( recipient.getAddress() );
	}
	
	addresses_list->trim_values();

	return addresses_list;
	
} // recipients


void BESMTPEmailMessage::set_addresses ( const Poco::Net::MailRecipient::RecipientType recipient_type, const BEValueListStringSharedPtr email_addresses )
{

	for ( auto const& recipient : *email_addresses ) {
		message.addRecipient ( BEMailRecipient ( recipient_type, recipient ) );
	}
	
} // set_addresses


void BESMTPEmailMessage::set_cc_addresses ( const BEValueListStringSharedPtr email_addresses )
{
	set_addresses ( Poco::Net::MailRecipient::CC_RECIPIENT, email_addresses );
}


void BESMTPEmailMessage::set_bcc_addresses ( const BEValueListStringSharedPtr email_addresses )
{
	set_addresses ( Poco::Net::MailRecipient::BCC_RECIPIENT, email_addresses );
}


void BESMTPEmailMessage::set_reply_to ( const std::string& reply_to_address )
{
	if ( ! reply_to_address.empty() ) {
		message.set ( "Reply-To", reply_to_address );
	}

}


void BESMTPEmailMessage::set_attachments ( const BESMTPContainerAttachmentVector& the_attachments )
{
	attachments = the_attachments;
	
} // set_attachments


void BESMTPEmailMessage::add_attachments ( )
{

	for ( size_t i = 0 ; i < attachments.size() ; i++ ) {
		
		auto new_attachment = attachments.at ( i );
		
		if ( exists ( new_attachment.first ) ) {
			
			auto attachment = new_attachment.first;
			attachment.make_preferred();
			auto file_name = Poco::Net::MailMessage::encodeWord ( attachment.filename().string() );
			auto path = Poco::Net::MailMessage::encodeWord ( attachment.string() );
			message.addAttachment ( file_name, new Poco::Net::FilePartSource ( path, new_attachment.second ) );
			
		} else {
			throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
		}

	}

} // add_attachments


const std::string BESMTPEmailMessage::as_string()
{
	
	auto charset = Poco::Net::MailMessage::encodeWord ( "charset=\"utf-8\"" );
	auto type_text = "text/plain; " + charset;
	auto type_html = "text/html; " + charset;
	
	if ( !text.empty() && !html.empty() ) {

		auto multipart_alternative = new Poco::Net::MultipartSource();
		multipart_alternative->addPart ( "", new Poco::Net::StringPartSource ( text, type_text ), Poco::Net::MailMessage::CONTENT_INLINE, Poco::Net::MailMessage::ENCODING_QUOTED_PRINTABLE );
		multipart_alternative->addPart ( "", new Poco::Net::StringPartSource ( html, type_html ), Poco::Net::MailMessage::CONTENT_INLINE, Poco::Net::MailMessage::ENCODING_QUOTED_PRINTABLE );
		message.addContent ( multipart_alternative, Poco::Net::MailMessage::ENCODING_8BIT );

	} else if ( !text.empty() && html.empty() ) {
		
		if ( attachments.empty() ) {
			message.setContent ( text );
			message.setContentType ( type_text );
		} else {
			message.addPart ( "", new Poco::Net::StringPartSource ( text, type_text ), Poco::Net::MailMessage::CONTENT_INLINE, Poco::Net::MailMessage::ENCODING_QUOTED_PRINTABLE );
		}

	} else if ( text.empty() && !html.empty() ) {

		if ( attachments.empty() ) {
			message.setContent ( html );
			message.setContentType ( type_html );
		} else {
			message.addPart ( "", new Poco::Net::StringPartSource ( html, type_html ), Poco::Net::MailMessage::CONTENT_INLINE, Poco::Net::MailMessage::ENCODING_QUOTED_PRINTABLE );
		}

	}

	add_attachments();

	ostringstream meassage_stream;
	message.write ( meassage_stream );
	return meassage_stream.str();

} // as_string

