//
//  Test_JSON_jq.mm
//  BE_Plugin_Tests
//
//  Created by Mark Banks on 27/7/2022.
//  Copyright © 2022 Goya. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "BEJSON.h"



@interface Test_JSON_jq : XCTestCase

@end



@implementation Test_JSON_jq


std::string json;
std::string default_filter;

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
	
	json = "[{\"label\":\"375ml\",\"value\":\"196\"},{\"label\":\"750ml\",\"value\":\"197\"},{\"label\":\"Magnum\",\"value\":\"198\"}]";
	default_filter = ".";

}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testInvalidInput {

	auto result = JSON_jq ( "", "", "" );
	XCTAssertEqual ( result, "parse error: Expected JSON value (while parsing '')" );

	result = JSON_jq ( json.substr ( 0, 10 ), "", "" );
	XCTAssertEqual ( result, "parse error: Unfinished JSON term at EOF at line 1, column 10 (while parsing '[{\"label\":')" );

	result = JSON_jq ( json, "?", "" );
	XCTAssertEqual ( result, "compile error" );

}


- (void)testJqVersion {

	auto result = JSON_jq ( json, ".", "--version" );
	XCTAssertEqual ( result, "jq-1.6\n" );

}


- (void)testFormatCompact {

	auto result = JSON_jq ( json, default_filter, "-c" );
	auto wanted = json + "\n";
	XCTAssertEqual ( result, wanted );

	auto long_result = JSON_jq ( json, default_filter, "--compact-output" );
	XCTAssertEqual ( result, long_result );

}


- (void)testFormatCompactRawNoLF {
	
	auto result = JSON_jq ( json, default_filter, "-c -j" );
	XCTAssertEqual ( result, json );

	auto long_result = JSON_jq ( json, default_filter, "--compact-output --join-output" );
	XCTAssertEqual ( result, long_result );

}


- (void)testGetValueForLabel {

	auto filter = std::string ( ".[] | select(.label == \"375ml\") | .value" );
	auto result = JSON_jq ( json, filter, "" );
	auto wanted = std::string ( "\"196\"\n" );
	XCTAssertEqual ( result, wanted );

}

- (void)testGetValueForLabelRaw {

	auto filter = std::string ( ".[] | select(.label == \"375ml\") | .value" );
	auto result = JSON_jq ( json, filter, "-r" );
	XCTAssertEqual ( result, "196\n" );
	
	auto long_result = JSON_jq ( json, filter, "--raw-output" );
	XCTAssertEqual ( result, long_result );}


- (void)testGetValueForLabelRawNoLF {

	auto filter = std::string ( ".[] | select(.label == \"375ml\") | .value" );
	
	auto result = JSON_jq ( json, filter, "-j" );
	XCTAssertEqual ( result, "196" );
	
	auto long_result = JSON_jq ( json, filter, "--join-output" );
	XCTAssertEqual ( result, long_result );

}




//- (void)testPerformanceExample {
//    // This is an example of a performance test case.
//    [self measureBlock:^{
//        // Put the code you want to measure the time of here.
//    }];
//}


@end
