/*
 Test_BEMailRecipient.mm
 BaseElements Plug-In
 
 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import <XCTest/XCTest.h>


#include "BEMailRecipient.h"


@interface Test_BEMailRecipient : XCTestCase

@end


@implementation Test_BEMailRecipient


std::string empty_string;
std::string email_address;
std::string bracketed_address;
std::string real_name;
std::string full_name;
BEMailRecipient recipient;

- (void)setUp {

	empty_string = "";
	email_address = "from@somewhere";
	bracketed_address = "<" + email_address + ">";
	real_name = "Japson Gizbottle";
	full_name = real_name + " " + bracketed_address;

	recipient = BEMailRecipient ( Poco::Net::MailRecipient::PRIMARY_RECIPIENT, email_address );

}


- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}


- (void)testType {
	
	XCTAssertEqual ( Poco::Net::MailRecipient::PRIMARY_RECIPIENT, recipient.getType() );

}


- (void)testEmailAddressWithoutName {

	XCTAssertEqual ( email_address, recipient.getAddress() );

}


- (void)testEmptyName {
	
	XCTAssertEqual ( empty_string, recipient.getRealName() );

}


- (void)testEmailAddressWithBrackets {

	recipient = BEMailRecipient ( Poco::Net::MailRecipient::PRIMARY_RECIPIENT,  bracketed_address );
	
	XCTAssertEqual ( bracketed_address, recipient.bracketed_address() );
	XCTAssertNotEqual ( bracketed_address, recipient.getAddress() );
	XCTAssertEqual ( email_address, recipient.getAddress() );

}


- (void)testRealName {

	recipient = BEMailRecipient ( Poco::Net::MailRecipient::PRIMARY_RECIPIENT,  full_name );
	
	XCTAssertEqual ( real_name, recipient.getRealName() );

}



@end
