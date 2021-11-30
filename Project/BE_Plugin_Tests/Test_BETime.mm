//
//  Test_BETime.m
//  BE_Plugin_Tests
//
//  Created by Mark Banks on 30/11/21.
//  Copyright © 2021 Goya. All rights reserved.
//


#import <XCTest/XCTest.h>


#include "BETime.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop


@interface Test_BETime : XCTestCase


@end


@implementation Test_BETime


- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testPtimeToMilliseconds {
	
	const boost::posix_time::ptime when ( boost::gregorian::date ( 2021, boost::gregorian::Nov, 11 ) );
	XCTAssertEqual ( ptime_to_milliseconds ( when ), 63772185600000 );

}


- (void)testStdTimeToTimestamp {
	
	XCTAssertEqual ( std_time_to_timestamp ( 0 ), 62135632800 );
	XCTAssertEqual ( std_time_to_timestamp ( 63772185600 ), 125907822000 );

}



- (void)testTimestampToEpoch {
	
	XCTAssertEqual ( timestamp_to_epoch ( 0 ), -62135636400 );
	XCTAssertEqual ( timestamp_to_epoch ( 63772185600 ), 1636549200 );

}


@end
