//
//  BERegularExpression.h
//  BaseElements
//
//  Created by Mark Banks on 13/08/2015.
//  Copyright (c) 2015 Goya. All rights reserved.
//

#ifndef BaseElements_BERegularExpression_h
#define BaseElements_BERegularExpression_h


#include "BEPluginException.h"

#include <Poco/Exception.h>
#include <Poco/RegularExpression.h>
#include <Poco/String.h>



/*
 notes
 
 the options are a string consisting of, in any order
 i	case insensitive
 m	multiline
 s	dot matches all characters, including newline
 x	ignore whitespace
 g	replace all
 
 if the replaceString parameter is present ( can be empty ) then a replace is performed
 otherwise a find
 */


template <typename T>
T regular_expression ( const T& text, const T& expression, const std::string options = "", const T& replace_with = "", const bool replace = false )
{
    
    int constructor_options = 0;
    int replace_options = 0;
    
    T regex_options = options;
    Poco::toLowerInPlace ( regex_options );
    
    std::size_t found = regex_options.find ( "i" );
    if ( found != std::string::npos ) {
        constructor_options |= Poco::RegularExpression::RE_CASELESS;
    }

    found = regex_options.find ( "m" );
    if ( found != std::string::npos ) {
        constructor_options |= Poco::RegularExpression::RE_MULTILINE;
    }
        
    found = regex_options.find ( "s" );
    if ( found != std::string::npos ) {
        constructor_options |= Poco::RegularExpression::RE_DOTALL;
    }
        
    found = regex_options.find ( "x" );
    if ( found != std::string::npos ) {
        constructor_options |= Poco::RegularExpression::RE_EXTENDED;
    }
        
    found = regex_options.find ( "g" );
    if ( found != std::string::npos ) {
        replace_options |= Poco::RegularExpression::RE_GLOBAL;
    }
        
    T matched;
    
    try {

		if ( ! expression.empty() ) { // poco.re hangs when this is true
			
			Poco::RegularExpression re ( expression, constructor_options, false );
			
			if ( ! replace ) {
				int match_options = 0;
				re.extract ( text, matched, match_options );
			} else {
				matched = text;
				re.subst ( matched, replace_with, replace_options ); // int now_many =
			}
			
		}
		
    } catch ( Poco::RegularExpressionException& e ) {
        throw BEPlugin_Exception ( e.code() );
    }
        
    return matched;
    
} // BE_RegularExpression


#endif
