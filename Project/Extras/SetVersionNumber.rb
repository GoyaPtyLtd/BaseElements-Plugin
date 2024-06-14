#/usr/bin/ruby

########################################################################
# SetVersionNumber
# 
# Automatically set the version number/string wherever it's needed
#
########################################################################


########################################################################
# settings
########################################################################

project_directory = ARGV[0]

testing = false
if testing then
  project_directory = '/Users/mark/Dropbox/Development/BaseElements-Plugin/'
end

win = project_directory + 'Resources/BEPluginVersion.rc'
win_project = project_directory + 'Resources/BaseElements.rc'
linux_project = project_directory + 'Project/BaseElements.cbp'

version_file = project_directory + 'Source/BEPluginVersion.h'
info_plist_file = project_directory + 'Resources/Base.lproj/Info.plist'


########################################################################
# classes
########################################################################

class Version

  def initialize version_string, version_number_string, version_autoupdate

      @string = version_string
      @version_number = version_number_string
      @autoupdate = version_autoupdate
      
      @eol = '\n'
      
   end
   
   attr_reader :string, :version_number, :autoupdate


   def description
     "Version: #{@version_number} - This plug-in provides additional functionality in FileMaker Pro. Visit goya.com.au for more info."
   end
   
   def short_version_string
     '\${PRODUCT_NAME} version ' + @version_number
   end
   
   def copyright_string
     'Copyright 2010-' + Time.new.year.to_s + ' by Goya. All rights reserved.'
   end
   
   def get_info_string
     short_version_string + ', ' + copyright_string
   end

   
end


class WindowsVersion < Version

  def initialize version_string, version_number_string, version_autoupdate
    super version_string, version_number, version_autoupdate 
    @eol = '\r\n'    
  end


  def file_version
    [ autoupdate[0..1].to_i.to_s, autoupdate[2..3].to_i.to_s, autoupdate[4..5].to_i.to_s, autoupdate[6..7].to_i.to_s ].join ','
  end

  def file_version_value     
    file_version.gsub ',', '.'
  end

  def version_file
     
     file_text = <<-VERSION_FILE
// Microsoft Visual C++ generated resource script.
//


/////////////////////////////////////////////////////////////////////////////
//  WARNING: this file is generated automatically. do not edit!
/////////////////////////////////////////////////////////////////////////////


#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "windows.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS/////////////////////////////////////////////////////////////////////////////
// English (Australia) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENA)
LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_AUS


/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
FILEVERSION #{file_version}
PRODUCTVERSION #{file_version}
FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
FILEFLAGS 0x1L
#else
FILEFLAGS 0x0L
#endif
FILEOS 0x40004L
FILETYPE 0x2L
FILESUBTYPE 0x0L
BEGIN
   BLOCK "StringFileInfo"
   BEGIN
       BLOCK "0c0904b0"
       BEGIN
           VALUE "CompanyName", "Goya Pty Ltd"
           VALUE "FileDescription", "Base Elements Plug-In #{string}"
           VALUE "FileVersion", "#{file_version_value}"
           VALUE "InternalName", "Base Elements Plug-In"
           VALUE "LegalCopyright", "#{copyright_string}"
           VALUE "OriginalFilename", "BaseElements.fmx"
           VALUE "ProductVersion", "#{file_version_value}"
       END
   END
   BLOCK "VarFileInfo"
   BEGIN
       VALUE "Translation", 0xc09, 1200
   END
END


#endif    // English (Australia) resources
/////////////////////////////////////////////////////////////////////////////


VERSION_FILE

    file_text.gsub("\n", "\r\n")
  
  end

  def write_version_file path
    File.write path, version_file, :encoding => Encoding::UTF_16LE
  end

 end
 
########################################################################
# needed when running from within Xcode
########################################################################

Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8


########################################################################
# read the macOS strings file
########################################################################

version = nil
version_string = ''
version_number_string = ''
version_autoupdate = ''  

File.open version_file do | macos_strings_file |

  macos_strings_file.each do | line |
    
    fields = line.split ' ', 3
    
    if fields.first == '#define' then
      
      if fields[1] == 'VERSION_STRING'
        version_string = fields.last.scan(/"([^"\\]*(?:\\.[^"\\]*)*)"/).flatten.first
      elsif fields[1] == 'VERSION_NUMBER_STRING'
        version_number_string = fields.last.scan(/"([^"\\]*(?:\\.[^"\\]*)*)"/).flatten.first
      elsif fields[1] == 'AUTO_UPDATE_VERSION'
        version_autoupdate = fields.last.scan(/"([^"\\]*(?:\\.[^"\\]*)*)"/).flatten.first
      end
        
    end
      
  end

  version = Version.new version_string, version_number_string, version_autoupdate
  
end

`/usr/libexec/PlistBuddy -c "Set :CFBundleShortVersionString #{version.string}" #{info_plist_file}`
`/usr/libexec/PlistBuddy -c "Set :CFBundleGetInfoString #{version.get_info_string}" #{info_plist_file}`
`/usr/libexec/PlistBuddy -c "Set :CFBundleVersion #{version.string}" #{info_plist_file}`


########################################################################
# windows
########################################################################

win_version = WindowsVersion.new version.string, version.version_number, version.autoupdate
win_version.write_version_file win


########################################################################
# linux
########################################################################

# except for the project file... just works... it's done anyway when the function strings file is built

lines = ''

File.open linux_project do | linux_project_file |
  
  linux_project_file.each do | line |
        
    if line['-soname'] then
            lines += '			<Add option="-Wl,-soname=' + version.string + '" />' + "\n"
    else
      lines += line
    end
      
  end
  
end

File.write linux_project, lines, :encoding => Encoding::UTF_8


########################################################################


__END__
