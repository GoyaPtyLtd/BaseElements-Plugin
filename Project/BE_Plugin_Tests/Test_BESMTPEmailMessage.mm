/*
 Test_BESMTPEmailMessage.mm
 BaseElements Plug-In
 
 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import <XCTest/XCTest.h>


#include "BESMTPEmailMessage.h"


@interface Test_BESMTPEmailMessage : XCTestCase

@end


@implementation Test_BESMTPEmailMessage


std::string from;
std::string subject;
std::string body_text;
std::string html_aternative;

std::string first_recipient;
std::string first_recipient_address;
std::string second_recipient;
std::string second_recipient_address;

std::string first_carbon;
std::string first_carbon_address;
std::string second_carbon;
std::string second_carbon_address;

std::string first_blind;
std::string first_blind_address;
std::string second_blind;
std::string second_blind_address;

BEValueListStringSharedPtr to;
BEValueListStringSharedPtr cc;
BEValueListStringSharedPtr bcc;

std::unique_ptr<BESMTPEmailMessage> message;
std::unique_ptr<BESMTPEmailMessage> html_message;


- (void)setUp {
	
	// Put setup code here. This method is called before the invocation of each test method in the class.
	
	from = "Boo Boo <from@address>";
	subject = "the naming of parts";
	body_text = "body text";
	first_recipient_address = "going@nowhere";
	first_recipient = "Japson Gizbottle <" + first_recipient_address + ">";
	second_recipient_address = "un@deliverable";
	second_recipient = second_recipient_address;

	BEValueListStringSharedPtr send_to ( new BEValueList<std::string> ( first_recipient + FILEMAKER_END_OF_LINE_CHAR + second_recipient, true, false ) );
	to = send_to;
	
	std::unique_ptr<BESMTPEmailMessage> default_message ( new BESMTPEmailMessage ( from, to, subject, body_text ) );
	message.swap ( default_message );

	html_aternative = "<html>\r<head>\r<title>HTML Aternative</title>\r</head>\r<body>\r<h1>HTML Alternative Heading</h1>\r<p>The html to use instead of plain text!</p>\r</body>\r</html>\n";
	std::unique_ptr<BESMTPEmailMessage> default_html_message ( new BESMTPEmailMessage ( from, to, subject, body_text + LINE_FEED, html_aternative ) );
	html_message.swap ( default_html_message );

	first_carbon_address = "first@carbon";
	first_carbon = "First Carbon <" + first_carbon_address + ">";
	second_carbon_address = "second@carbon";
	second_carbon = second_carbon_address;
	auto cc_recipients = first_carbon + FILEMAKER_END_OF_LINE_CHAR + second_carbon;
	BEValueListStringSharedPtr copy ( new BEValueList<std::string> ( cc_recipients, true, false ) );
	cc.swap ( copy );

	first_blind_address = "first@blind";
	first_blind = "First Blind <" + first_blind_address + ">";
	second_blind_address = "second@blind";
	second_blind = second_blind_address;
	auto bcc_recipients = first_blind + FILEMAKER_END_OF_LINE_CHAR + second_blind;
	BEValueListStringSharedPtr blind_copy ( new BEValueList<std::string> ( bcc_recipients, true, false ) );
	bcc.swap ( blind_copy );

}

- (void)tearDown {
	// Put teardown code here. This method is called after the invocation of each test method in the class.
}


//- (void)testPerformanceExample {
//	// This is an example of a performance test case.
//	[self measureBlock:^{
//		// Put the code you want to measure the time of here.
//		std::unique_ptr<BESMTPEmailMessage> message ( new BESMTPEmailMessage ( from, to, subject, body_text ) );
//
//		XCTAssertTrue ( from == message->from_address() );
//
//	}];
//}


- (void)testFromAddress {
	
	XCTAssertTrue ( from == message->from_address() );

	auto mime = message->as_string();
	XCTAssertNotEqual ( mime.find ( "From: " + from ), std::string::npos );

}


- (void)testSubject {

	auto mime = message->as_string();
	XCTAssertNotEqual ( mime.find ( subject ), std::string::npos );

}


- (void)testToOneRecipient {

	BEValueListStringSharedPtr send_to ( new BEValueList<std::string> ( first_recipient, true, false ) );
	std::unique_ptr<BESMTPEmailMessage> test_message ( new BESMTPEmailMessage ( from, send_to, subject, body_text ) );
	
	auto message_recipients = test_message->recipients();
	
	XCTAssertEqual ( first_recipient_address, message_recipients->first() );
	XCTAssertEqual ( 1UL, message_recipients->size() );
	
	auto mime = test_message->as_string();
	XCTAssertNotEqual ( mime.find ( "To: " + first_recipient ), std::string::npos );
	
	// unbracketed
	
	BEValueListStringSharedPtr send_unbracketed ( new BEValueList<std::string> ( second_recipient, true, false ) );
	std::unique_ptr<BESMTPEmailMessage> unbracketed_message ( new BESMTPEmailMessage ( from, send_unbracketed, subject, body_text ) );

	mime = unbracketed_message->as_string();
	if ( mime.find ( "To: <" ) == std::string::npos ) {
		XCTAssertNotEqual ( mime.find ( "To: " + second_recipient ), std::string::npos );
		XCTAssertEqual ( mime.find ( "To: <" + second_recipient + ">" ), std::string::npos );
	} else {
		XCTExpectFailure ( @"Bracketing naked To: addresses." );
		XCTAssertEqual ( mime.find ( "To: <" + second_recipient + ">" ), std::string::npos );
//		XCTSkip ( @"Bracketing naked To: addresses." );
		
	}

}


- (void)testToTwoRecipients {

	auto message_recipients = message->recipients();

	XCTAssertEqual ( first_recipient_address, message_recipients->first() );
	XCTAssertEqual ( second_recipient_address, message_recipients->at(1) );
	XCTAssertEqual ( 2UL, message_recipients->size() );

}


- (void)testRemoveEmptyRecipients {

	auto recipients = first_recipient + FILEMAKER_END_OF_LINE_CHAR + FILEMAKER_END_OF_LINE_CHAR + second_recipient;
	BEValueListStringSharedPtr send_to ( new BEValueList<std::string> ( recipients, true, false ) );
	std::unique_ptr<BESMTPEmailMessage> test_message ( new BESMTPEmailMessage ( from, to, subject, body_text ) );

	auto message_recipients = test_message->recipients();
	XCTAssertEqual ( 2UL, message_recipients->size() );

}


- (void)testCC {

	message->set_cc_addresses ( cc );
	auto message_recipients = message->recipients();

	XCTAssertEqual ( first_carbon_address, message_recipients->at(2) );
	XCTAssertEqual ( second_carbon_address, message_recipients->at(3) );
	XCTAssertEqual ( 4UL, message_recipients->size() );

}


- (void)testBCC {

	message->set_bcc_addresses ( bcc );
	auto message_recipients = message->recipients();

	XCTAssertEqual ( first_blind_address, message_recipients->at(2) );
	XCTAssertEqual ( second_blind_address, message_recipients->at(3) );
	XCTAssertEqual ( 4UL, message_recipients->size() );

}


- (void)testCCAndBCC {

	message->set_cc_addresses ( cc );
	message->set_bcc_addresses ( bcc );
	auto message_recipients = message->recipients();

	XCTAssertEqual ( first_recipient_address, message_recipients->first() );
	XCTAssertEqual ( second_recipient_address, message_recipients->at(1) );
	XCTAssertEqual ( first_carbon_address, message_recipients->at(2) );
	XCTAssertEqual ( second_carbon_address, message_recipients->at(3) );
	XCTAssertEqual ( first_blind_address, message_recipients->at(4) );
	XCTAssertEqual ( second_blind_address, message_recipients->at(5) );
	XCTAssertEqual ( 6UL, message_recipients->size() );

}


- (void)testReplyTo {

	auto reply_to = "reply@too";
	message->set_reply_to ( reply_to );
	
	auto mime = message->as_string();
	auto find_this = std::string ( "Reply-To: " ) + reply_to;
	
	XCTAssertNotEqual ( mime.find ( find_this ), std::string::npos );

}


- (void)testPlainText {

	auto mime = message->as_string();

	XCTAssertNotEqual ( mime.find ( body_text ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: text/plain; charset=utf-8" ), std::string::npos );

}


- (void)testPlainTextAndAttachment {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	message->set_attachments ( attachments.get_file_list() );
	auto mime = message->as_string();

	XCTAssertNotEqual ( mime.find ( body_text ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: multipart/mixed;" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: text/plain; charset=utf-8" ), std::string::npos );
	XCTAssertGreaterThan ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=TEXT" ), mime.find ( "Content-Type: multipart/mixed;" ) );

}


- (void)testPlainTextAndHTML {

	auto mime = html_message->as_string();

	XCTAssertNotEqual ( mime.find ( body_text ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "html to use instead" ), std::string::npos );

}


- (void)testHTMLWithNoPlainText {

	std::unique_ptr<BESMTPEmailMessage> html_only_message ( new BESMTPEmailMessage ( from, to, subject, "", html_aternative ) );
	auto mime = html_only_message->as_string();

	XCTAssertEqual ( mime.find ( "Content-Type: text/plain;" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: text/html; charset=utf-8" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "html to use instead" ), std::string::npos );

}


- (void)testHTMLWithAttachmentAndWithNoPlainText {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	std::unique_ptr<BESMTPEmailMessage> html_only_message ( new BESMTPEmailMessage ( from, to, subject, "", html_aternative ) );
	html_only_message->set_attachments ( attachments.get_file_list() );
	auto mime = html_only_message->as_string();

	XCTAssertNotEqual ( mime.find ( "html to use instead" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: multipart/mixed;" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: text/html; charset=utf-8" ), std::string::npos );
	XCTAssertGreaterThan ( mime.find ( "Content-Type: text/html; charset=utf-8" ), mime.find ( "Content-Type: multipart/mixed;" ) );

}


- (void)testPlainTextHTMLAndAttachment {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	html_message->set_attachments ( attachments.get_file_list() );
	auto mime = html_message->as_string();

	XCTAssertNotEqual ( mime.find ( body_text ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "html to use instead" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: multipart/mixed;" ), std::string::npos );
	XCTAssertNotEqual ( mime.find ( "Content-Type: text/plain; charset=utf-8" ), std::string::npos );
	XCTAssertGreaterThan ( mime.find ( "Content-Type: text/plain; charset=utf-8" ), mime.find ( "Content-Type: multipart/mixed;" ) );
	XCTAssertGreaterThan ( mime.find ( "Content-Type: text/plain; charset=utf-8" ), mime.find ( "Content-Type: multipart/mixed;" ) );
	XCTAssertNotEqual ( mime.find ( "Content-Type: text/html; charset=utf-8" ), std::string::npos );

}


- (void)testBareLineFeeds {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	html_message->set_attachments ( attachments.get_file_list() );
	auto mime = html_message->as_string();

	boost::replace_all ( mime, NETWORK_ENDL, "" );

	XCTAssertEqual ( mime.find ( "\n" ), std::string::npos );
	XCTAssertEqual ( mime.find ( "\r" ), std::string::npos );

}


@end
