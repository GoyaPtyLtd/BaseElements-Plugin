/*
 BETask.cpp
 BaseElements Plug-In

 Copyright 2021-2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

*/

#include "BETask.h"
#include "BEXSLT.h"


#include <future>


const BETaskResult xslt_task ( std::vector<boost::filesystem::path>& csv_paths, std::vector<std::string>& xslt, const std::string& xml, const boost::filesystem::path& xml_path, const std::string& script_name, const std::string& database_name )
{

	auto output_path = csv_paths.front();
	csv_paths.erase ( csv_paths.begin() );

	auto stylesheet = xslt.front();
	xslt.erase ( xslt.begin() );

	std::promise<BETaskResult> thread_result;
	auto task_reply = thread_result.get_future();

	std::thread task ( [xml_path, output_path, stylesheet, xml, script_name, database_name, &thread_result] {

		if ( !output_path.empty() || !stylesheet.size() ) {
			
			auto csv = ApplyXSLTInMemory ( xml, stylesheet, output_path, xml_path );

			if ( script_name.empty() ) {
				thread_result.set_value_at_thread_exit ( BETaskResult { kNoError, csv } );
			} else {
				ExecuteScript ( script_name, database_name, output_path.string(), kFMXT_Pause ); // auto error =
			}

		} else {
			thread_result.set_value_at_thread_exit ( BETaskResult { kErrorParameterMissing, "" } );
		}
		
	} );
	
	BETaskResult task_response;
	
	while ( xslt.size() > 0 && csv_paths.size() > 0 ) {
		
		auto result = xslt_task ( csv_paths, xslt, xml, xml_path, script_name, database_name );
		if ( kNoError == task_response.first ) {
			task_response.first = result.first;
		}
		task_response.second += result.second;
		
	}
	
	if ( script_name.empty() ) {

		// we wait rather than go into the background
		
		task.join();
		auto result = task_reply.get();
		if ( kNoError == task_response.first ) {
			task_response.first = result.first;
		}
		task_response.second += result.second;

	} else {
		task.detach();
	}

	return task_response;

}


