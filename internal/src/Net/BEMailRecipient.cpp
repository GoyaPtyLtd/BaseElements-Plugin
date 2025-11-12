/*
 BEMailRecipient.cpp
 BaseElements Plug-In
 
 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEMailRecipient.h"


#include<boost/algorithm/string.hpp>

/*
*/


BEMailRecipient::BEMailRecipient ( RecipientType type, const std::string& name_and_address )
{
	
	setType ( type );


	const size_t open_bracket = name_and_address.find ( "<" );
		
	std::string real_name;
	std::string email_address;
	
	if ( open_bracket != std::string::npos ) {

		real_name = name_and_address.substr ( 0, open_bracket );
		boost::algorithm::trim ( real_name );

		const size_t close_bracket = name_and_address.find ( ">" );
		email_address = name_and_address.substr ( open_bracket + 1, close_bracket - open_bracket - 1 );
		
	} else {
		email_address = name_and_address;
	}

	setRealName ( real_name );
	setAddress ( email_address );
	
}
