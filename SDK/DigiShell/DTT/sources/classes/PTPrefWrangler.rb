#
#  PTPrefWrangler.rb
#  Goldsmith
#
#  Created by Tim Walters on 4/25/08.
#  Copyright (c) 2008. All rights reserved.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool
module DishTestTool

=begin rdoc
  Utility for deleting PT prefs.
=end

class PTPrefWrangler

  PREF_PATH = ["$HOME/Library/Preferences/Avid/Pro*Tools*",
               "$HOME/Library/Preferences/Avid/zeta-expanded"]
  # old locations
  # ["$HOME/Library/Preferences/DAE\\ Prefs",
  #  "$HOME/Library/Preferences/DigiSetup.OSX"
  #  "$HOME/Library/Preferences/Pro*Tools*"]

  def self.delete_prefs(sys_info)
    volumes = sys_info.volumes
    volumes.each do
      |volume|
      db_folders = [volume.path + 'Digidesign Databases', volume.path + 'Avid Databases']
      db_folders.each { |db_folder| FileUtils.rm_r(db_folder) if db_folder.exist? }
    end
    begin
      if RUNTIME_INFO.win? then
        homeDir = ENV['HOMEPATH']
        systemDir = ENV['SYSTEMDRIVE']
        [ 
          %Q("#{systemDir}#{homeDir}\\Application Data\\Digidesign"), 
          %Q("#{systemDir}#{homeDir}\\AppData\\Roaming\\Digidesign"),
          %Q("#{systemDir}\\Program Files\\Common Files\\Digidesign\\DAE\\DAE Prefs"),
          %Q("#{systemDir}\\Program Files\\Common Files\\Digidesign\\DAE\\IO Settings"),
          %Q("#{systemDir}\\Program Files (x86)\\Common Files\\Digidesign\\DAE\\DAE Prefs"),
          %Q("#{systemDir}\\Program Files (x86)\\Common Files\\Digidesign\\DAE\\IO Settings"),
          %Q("#{systemDir}#{homeDir}\\Application Data\\Avid"),
          %Q("#{systemDir}#{homeDir}\\AppData\\Roaming\\Avid"),
          %Q("#{systemDir}\\Program Files\\Common Files\\Avid\\DAE\\DAE Prefs"),
          %Q("#{systemDir}\\Program Files\\Common Files\\Avid\\DAE\\IO Settings"),
          %Q("#{systemDir}\\Program Files (x86)\\Common Files\\Avid\\DAE\\DAE Prefs"),
          %Q("#{systemDir}\\Program Files (x86)\\Common Files\\Avid\\DAE\\IO Settings")
        ].each do
          |pref|
          system("if exist #{pref} rmdir /S /Q #{pref}")
        end
        [ 
          %Q("#{systemDir}#{homeDir}\\AppData\\Roaming\\Digidesign*"),
          %Q("#{systemDir}#{homeDir}\\AppData\\Roaming\\Avid*")
        ].each do
          |pref|
          system("if exist #{pref} del /S /Q #{pref}")
        end
        
        # can't delete whole DSI key, as that removes auth
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\chtd /f 2> nul`   # delete DSI registry items
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\Cnfg /f 2> nul`
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\DflD /f 2> nul`
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\K016 /f 2> nul`
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\SyWi /f 2> nul`
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\MBox /f 2> nul`
        result = `reg delete HKLM\\Software\\Digidesign\\DSI\\AnyVersion\\chxd /f 2> nul`
	  elsif RUNTIME_INFO.linux?
		
      else
        PREF_PATH.each do |pref|
            result = `rm -rf #{pref} >& /dev/null`   # using [ -e #{pref} ] doesn't work because the expansion contains spaces
          end
      end
    rescue SystemCallError, IOError => err
      "Failed to delete PT prefs: #{err}.".abort(self)
    end
    return true
  end
end # PTPrefWrangler

end # module
