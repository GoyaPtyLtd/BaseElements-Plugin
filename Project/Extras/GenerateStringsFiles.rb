#/usr/bin/ruby

########################################################################
# GenerateStringsFiles
# 
# Write out/modify the (English language) Windows strings file
#     based on the macOS one.
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

macOS = project_directory + 'Resources/Base.lproj/Localizable.strings'
win = project_directory + 'Resources/BaseElements.rc'

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
   
end


########################################################################
# needed when running from within Xcode
########################################################################

Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8


########################################################################
# read the macOS strings file
########################################################################

strings = Hash.new

File.open macOS do | macos_strings_file |

  macos_strings_file.each do | line |
    
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


########################################################################
# windows
########################################################################

windows_strings = ''

File.open win, :encoding => Encoding::UTF_16LE, :binmode => true do | strings_file |

  strings_file.each do | line |

    utf8 = line.encode Encoding::UTF_8
    fields = utf8.split '"'
  
    unless utf8.start_with? '//' then

      begin
      
        id = fields.first.tr('^0-9', '').to_i

        function = strings[id]
        new_line = "    %-21d\"%s\"\r\n" % [id, function.windows_text]
        windows_strings += new_line
      
      rescue => e

        unless id.is_a? Fixnum and id > 0 then # function removed
          windows_strings += line
        end
    
      end

    else
      windows_strings += line
    end # if
    
  end

end

File.open win, 'wt', :encoding => Encoding::UTF_16LE do | windows_strings_file |
  windows_strings_file.puts windows_strings
end


########################################################################


__END__

