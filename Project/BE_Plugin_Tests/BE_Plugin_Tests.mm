//
//  Test_BESMTPEmailMessage.mm
//  BE_Plugin_Tests
//
//  Created by Mark Banks on 23/6/21.
//  Copyright © 2021 Goya. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "BESMTPEmailMessage.h"


@interface Test_BESMTPEmailMessage : XCTestCase

@end

@implementation Test_BESMTPEmailMessage

std::string from;
std::string subject;
std::string body_text;
std::string html_aternative;
std::string one_recipient;

BEValueListStringSharedPtr to;
BEValueListStringSharedPtr cc;
BEValueListStringSharedPtr bcc;

std::unique_ptr<BESMTPEmailMessage> message;
std::unique_ptr<BESMTPEmailMessage> html_message;


- (void)setUp {
	
	// Put setup code here. This method is called before the invocation of each test method in the class.
	
	from = "from@address";
	subject = "the naming of parts";
	body_text = "body text";
	one_recipient = "going@nowhere";

	BEValueListStringSharedPtr send_to ( new BEValueList<std::string> ( one_recipient + FILEMAKER_END_OF_LINE_CHAR + "un@deliverable", true, false ) );
	to = send_to;
	
	std::unique_ptr<BESMTPEmailMessage> default_message ( new BESMTPEmailMessage ( from, to, subject, body_text ) );
	message.swap ( default_message );

	html_aternative = "<html>\r<head>\r<title>HTML Aternative</title>\r</head>\r<body>\r<h1>HTML Alternative Heading</h1>\r<p>The html to use instead of plain text!</p>\r</body>\r</html>\n";
	std::unique_ptr<BESMTPEmailMessage> default_html_message ( new BESMTPEmailMessage ( from, to, subject, body_text + LINE_FEED, html_aternative ) );
	html_message.swap ( default_html_message );

	auto cc_recipients = std::string ( "first@carbon" ) + FILEMAKER_END_OF_LINE_CHAR + "second@carbon";
	BEValueListStringSharedPtr copy ( new BEValueList<std::string> ( cc_recipients, true, false ) );
	cc.swap ( copy );

	auto bcc_recipients = std::string ( "first@blind" ) + FILEMAKER_END_OF_LINE_CHAR + "second@blind";
	BEValueListStringSharedPtr blind_copy ( new BEValueList<std::string> ( cc_recipients, true, false ) );
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

}


- (void)testSubject {

	auto mime = message->as_string();
	XCTAssertTrue ( mime.find ( subject ) != std::string::npos );

}


- (void)testToOneRecipient {

	BEValueListStringSharedPtr send_to ( new BEValueList<std::string> ( one_recipient, true, false ) );
	std::unique_ptr<BESMTPEmailMessage> test_message ( new BESMTPEmailMessage ( from, send_to, subject, body_text ) );
	
	auto message_recipients = test_message->recipients();
	XCTAssertTrue ( one_recipient == message_recipients->first() );
	XCTAssertTrue ( 1 == message_recipients->size() );

}


- (void)testToTwoRecipients {

	auto message_recipients = message->recipients();

	XCTAssertTrue ( to->first() == message_recipients->first() );
	XCTAssertTrue ( to->at(1) == message_recipients->at(1) );
	XCTAssertTrue ( 2 == message_recipients->size() );

}


- (void)testRemoveEmptyRecipients {

	auto recipients = one_recipient + FILEMAKER_END_OF_LINE_CHAR + FILEMAKER_END_OF_LINE_CHAR + "un@deliverable";
	BEValueListStringSharedPtr send_to ( new BEValueList<std::string> ( recipients, true, false ) );
	std::unique_ptr<BESMTPEmailMessage> test_message ( new BESMTPEmailMessage ( from, to, subject, body_text ) );

	auto message_recipients = test_message->recipients();
	XCTAssertTrue ( 2 == message_recipients->size() );

}


- (void)testCC {

	message->set_cc_addresses ( cc );
	auto message_recipients = message->recipients();

	XCTAssertTrue ( to->first() == message_recipients->first() );
	XCTAssertTrue ( to->at(1) == message_recipients->at(1) );
	XCTAssertTrue ( cc->first() == message_recipients->at(2) );
	XCTAssertTrue ( cc->at(1) == message_recipients->at(3) );
	XCTAssertTrue ( 4 == message_recipients->size() );

}


- (void)testBCC {

	message->set_bcc_addresses ( bcc );
	auto message_recipients = message->recipients();

	XCTAssertTrue ( to->first() == message_recipients->first() );
	XCTAssertTrue ( to->at(1) == message_recipients->at(1) );
	XCTAssertTrue ( bcc->first() == message_recipients->at(2) );
	XCTAssertTrue ( bcc->at(1) == message_recipients->at(3) );
	XCTAssertTrue ( 4 == message_recipients->size() );

}


- (void)testCCAndBCC {

	message->set_cc_addresses ( cc );
	message->set_bcc_addresses ( bcc );
	auto message_recipients = message->recipients();

	XCTAssertTrue ( to->first() == message_recipients->first() );
	XCTAssertTrue ( to->at(1) == message_recipients->at(1) );
	XCTAssertTrue ( cc->first() == message_recipients->at(2) );
	XCTAssertTrue ( cc->at(1) == message_recipients->at(3) );
	XCTAssertTrue ( bcc->first() == message_recipients->at(4) );
	XCTAssertTrue ( bcc->at(1) == message_recipients->at(5) );
	XCTAssertTrue ( 6 == message_recipients->size() );

}


- (void)testReplyTo {

	auto reply_to = "reply@too";
	message->set_reply_to ( reply_to );
	
	auto mime = message->as_string();
	auto find_this = std::string ( "Reply-To: " ) + reply_to;
	
	XCTAssertTrue ( mime.find ( find_this ) != std::string::npos );

}


- (void)testPlainText {

	auto mime = message->as_string();

	XCTAssertFalse ( mime.find ( body_text ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=TEXT" ) == std::string::npos );

}


- (void)testPlainTextAndAttachment {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	message->set_attachments ( attachments.get_file_list() );
	auto mime = message->as_string();

	XCTAssertFalse ( mime.find ( body_text ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: multipart/mixed;" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=TEXT" ) == std::string::npos );
	XCTAssertTrue ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=TEXT" ) > mime.find ( "Content-Type: multipart/mixed;" ) );

}


- (void)testPlainTextAndHTML {

	auto mime = html_message->as_string();

	XCTAssertFalse ( mime.find ( body_text ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "html to use instead" ) == std::string::npos );

}


- (void)testHTMLWithNoPlainText {

	std::unique_ptr<BESMTPEmailMessage> html_only_message ( new BESMTPEmailMessage ( from, to, subject, "", html_aternative ) );
	auto mime = html_only_message->as_string();

	XCTAssertTrue ( mime.find ( "Content-Type: text/plain;" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: text/html; charset=utf-8; name=HTML" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "html to use instead" ) == std::string::npos );

}


- (void)testHTMLWithAttachmentAndWithNoPlainText {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	std::unique_ptr<BESMTPEmailMessage> html_only_message ( new BESMTPEmailMessage ( from, to, subject, "", html_aternative ) );
	html_only_message->set_attachments ( attachments.get_file_list() );
	auto mime = html_only_message->as_string();

	XCTAssertFalse ( mime.find ( "html to use instead" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: multipart/mixed;" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: text/html; charset=utf-8; name=HTML" ) == std::string::npos );
	XCTAssertTrue ( mime.find ( "Content-Type: text/html; charset=utf-8; name=HTML" ) > mime.find ( "Content-Type: multipart/mixed;" ) );

}


- (void)testPlainTextHTMLAndAttachment {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	html_message->set_attachments ( attachments.get_file_list() );
	auto mime = html_message->as_string();

	XCTAssertFalse ( mime.find ( body_text ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "html to use instead" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: multipart/mixed;" ) == std::string::npos );
	XCTAssertFalse ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=TEXT" ) == std::string::npos );
	XCTAssertTrue ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=TEXT" ) > mime.find ( "Content-Type: multipart/mixed;" ) );
	XCTAssertTrue ( mime.find ( "Content-Type: text/plain; charset=utf-8; name=HTML" ) > mime.find ( "Content-Type: multipart/mixed;" ) );
	XCTAssertFalse ( mime.find ( "Content-Type: text/html; charset=utf-8; name=HTML" ) == std::string::npos );

}


- (void)testBareLineFeeds {

	BESMTPContainerAttachments attachments;
	attachments.add ( std::string ( "/Users/mark/.CFUserTextEncoding" ) );
	html_message->set_attachments ( attachments.get_file_list() );
	auto mime = html_message->as_string();

	boost::replace_all ( mime, NETWORK_ENDL, "" );

	XCTAssertTrue ( mime.find ( "\n" ) == std::string::npos );
	XCTAssertTrue ( mime.find ( "\r" ) == std::string::npos );

}


@end
