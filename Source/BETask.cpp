/*
 BETask.cpp
 BaseElements Plug-In

 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

*/

#include "BETask.h"
#include "BEXSLT.h"


#include <future>


const BETaskResult xslt_task ( const std::vector<std::wstring>& parameters, const fmx::uint32 first_parameter, const std::string& xml, const std::string& script_name, const std::string& database_name )
{
	
	std::promise<BETaskResult> thread_result;
	auto task_reply = thread_result.get_future();

	std::thread task ( [parameters, first_parameter, xml, script_name, database_name, &thread_result] {

		try {
			
			auto xml_path = parameters.front();  // same each time

			auto xslt = parameters.at ( first_parameter + 1 );
			const std::string utf8_xslt ( xslt.begin(), xslt.end() );

			auto csv_path = parameters.at ( first_parameter + 2 );

			auto csv = ApplyXSLTInMemory ( xml, utf8_xslt, csv_path, xml_path );

			if ( script_name.empty() ) {
				thread_result.set_value_at_thread_exit ( BETaskResult { kNoError, csv } );
			} else {

				const std::string utf8_csv_path ( csv_path.begin(), csv_path.end() );
				auto script_parameter = csv.empty() ? utf8_csv_path : csv;
				ExecuteScript ( script_name, database_name, script_parameter, kFMXT_Pause ); // auto error =

			}
			
		} catch ( const std::out_of_range& e ) {
			thread_result.set_value_at_thread_exit ( BETaskResult { kErrorParameterMissing, e.what() } );
		}
					  
	} );
		
		
	BETaskResult task_response;
	
	auto last_parameter = parameters.size() - 1;
	auto next_parameter = first_parameter == 0 ? 4 : first_parameter + 2;
	if ( next_parameter < last_parameter ) {
		auto result = xslt_task ( parameters, next_parameter, xml, script_name, database_name );
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


