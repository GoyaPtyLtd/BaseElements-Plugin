//
//  BEValueList.h
//  BaseElements
//
//  Created by Mark Banks on 20/03/13.
//  Copyright (c) 2013 Goya. All rights reserved.
//

#ifndef __BaseElements__BEValueList__
#define __BaseElements__BEValueList__

#include <iostream>
#include <string>
#include <vector>


class BEValueList {
	
	
public:
	
	BEValueList ( const std::string value_list );
	

	std::string unique ( );
	std::string filter_out ( std::auto_ptr<BEValueList> filter_out );
	std::string list_union ( std::auto_ptr<BEValueList> additional_values );
	std::string sort ( );
	
	std::vector<std::string> get_values ( void ) { return values; }

protected:
	
	std::vector<std::string> values;
	
};


#endif /* defined(__BaseElements__BEValueList__) */
