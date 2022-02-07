#/usr/bin/ruby

########################################################################
# GenerateStringsFiles
# 
# Write out/modify the (English/German language) Linux & Windows strings  
#     files based on the macOS one.
#
########################################################################


########################################################################
# settings
########################################################################

project_directory = ARGV[0]

testing = false
if testing then
  project_directory = '/Users/mark/Dropbox/Development/Plugin_Build_Folder/BaseElements/'
end

macOS_en = project_directory + 'Resources/en.lproj/Localizable.strings'
macOS_de = project_directory + 'Resources/de.lproj/Localizable.strings'

win_en = project_directory + 'Resources/BaseElements_en.rc'
win_de = project_directory + 'Resources/BaseElements_de.rc'

linux = project_directory + 'Source/linux/BELinuxFunctionStrings.h'

version_file = project_directory + 'Source/BEPluginVersion.h'


########################################################################
# classes
########################################################################

class FunctionString

  def initialize string_id, string_signature, string_keywords, string_description

      @id = string_id
      @signature = string_signature
      @keywords = string_keywords
      @description = string_description

   end
   
   def windows_text
     
     function_line = @signature
     unless @keywords.empty? and @description.empty?
       function_line += '|' + @keywords + '|' + @description.gsub('\"', '""').gsub('@', '\@')
     end
     
     return function_line
     
   end
   
   def linux_text
     @signature
   end
   
end


########################################################################
# needed when running from within Xcode
########################################################################

Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8


########################################################################
# read the macOS strings file(s)
########################################################################


def read_strings_file path

  strings = Hash.new

  File.open path do | strings_file |

    strings_file.each do | line |
    
      fields = line.split '=', 2 # descriptions may contain =
    
      if fields.count == 2 then

        id = fields.first.tr('^0-9', '').to_i
        parts = fields.last.chomp.chomp(';').chomp('"').split '|', 3
              
        signature = parts.first.strip
        signature = signature[1..-1] # lose the "

        if parts.count == 3 then # it's a function
          keywords = parts[1].strip
          description = parts.last.strip
        else
          keywords = ''
          description = ''
        end
        
        function = FunctionString.new id, signature, keywords, description
        strings [ id ] = function
      
      end
      
    end

  end
  
  strings

end


strings_en = read_strings_file macOS_en
strings_de = read_strings_file macOS_de


########################################################################
# windows
########################################################################


def write_rc_file language, country, lang_id, path, functions
  
  rc_header = <<~STRINGTABLE_BEGIN
  // Microsoft Visual C++ generated resource script. 
  // 


  #define APSTUDIO_READONLY_SYMBOLS 
  ///////////////////////////////////////////////////////////////////////////// 
  // 
  // Generated from the TEXTINCLUDE 2 resource. 
  // 
  #include "windows.h" 

  ///////////////////////////////////////////////////////////////////////////// 
  #undef APSTUDIO_READONLY_SYMBOLS 

  ///////////////////////////////////////////////////////////////////////////// 
  // #{language} (#{country}) resources 

  #if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENA) 
  LANGUAGE #{lang_id}

  ///////////////////////////////////////////////////////////////////////////// 
  // 
  // String Table 
  // 

  STRINGTABLE 
  BEGIN
STRINGTABLE_BEGIN

  rc_footer = <<~STRINGTABLE_END
  END

  #endif // #{language} (#{country}) resources 
  ///////////////////////////////////////////////////////////////////////////// 

STRINGTABLE_END
  
  
  File.open path, 'wt', :encoding => Encoding::UTF_16LE do | rc_file |
  
    rc_file.puts rc_header
  
    functions.each do | id, function |

      # puts id
      # function = line
      rc_file.puts "    %-21d\"%s\"\r\n" % [id, function.windows_text]
    
    end
  
    rc_file.puts rc_footer
  
  end

  
end


write_rc_file "English", "Australia", "LANG_ENGLISH, SUBLANG_ENGLISH_AUS", win_en, strings_en
write_rc_file "German", "Germany", "LANG_GERMAN, SUBLANG_GERMAN", win_de, strings_de


# exit


########################################################################
# linux
########################################################################

linux_strings = ''

strings_en.each do |id, function|
  
  map_entry = "\t{ " + id.to_s + ", " + '"' + function.linux_text + '" },' + "\n"
  linux_strings += map_entry
  
end


copyright_header = DATA.read
copyright_start_year = 2019
unless Time.new.year == copyright_start_year then
  puts copyright_header.gsub! copyright_start_year.to_s, copyright_start_year.to_s + '-' + Time.new.year.to_s
end

File.open linux, 'wt' do | linux_strings_file |
  
  linux_strings_file.puts copyright_header
  linux_strings_file.puts linux_strings
  linux_strings_file.puts "\n};\n\n#endif // BELINUXFUNCTIONSTRINGS_H\n\n"

end


########################################################################


__END__
/*
 BELinuxFunctionStrings.h
 BaseElements Plug-In

 Copyright 2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

   http://www.goya.com.au/baseelements/plugin
 
 IMPORTANT: this file is automatically generated! Do not edit by hand.

 */


#if !defined(BELINUXFUNCTIONSTRINGS_H)
	#define BELINUXFUNCTIONSTRINGS_H

#include "BEPluginGlobalDefines.h"

#include <map>
#include <string>

const std::map<unsigned long, std::string> function_strings = {

