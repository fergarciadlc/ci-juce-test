#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

module DishTestTool

=begin rdoc
  Used to send script results to Growl.
=end
  class Notifier

    def initialize
      #determine Mac (growl) or PC (unknown)
      if RUBY_PLATFORM.include?("darwin")
        @platform = "mac"
        @growl_path = Pathname.new($0).parent.parent + 'bin' + 'growlnotify'
      else
        @platform = "pc"
        @growl_path = "C:\\Program Files\\Growl for Windows\\growlnotify.com"
      end
    end

=begin rdoc
Sends a notification with +title+ and +message+. Uses growlnotifier on Mac and Windows.
=end
    def notify(title, message)
      #pass message along to notify_mac or notify_pc
      if @platform == "mac"
        notify_mac(title, message)
      elsif @platform == "pc"
        notify_pc(title, message)
      end
    end

    def notify_mac(title, message) # :nodoc:
      if message.include?("FAIL")
        priority = 1
      else
        priority = 0
      end

      unless !File.exist?(@growl_path) || !File.exist?("/Library/PreferencePanes/Growl.prefPane")
        `#{@growl_path} -n DishTestTool -s -p#{priority} -t \"#{title}\" -m \"#{message}\"`
      end
    end

    def notify_pc(title, message) # :nodoc:
      if message.include?("FAIL")
        priority = 1
      else
        priority = 0
      end

      unless !File.exist?(@growl_path)
        `\"#{@growl_path}\" \"#{message}\" /s:true /p:#{priority} /t:\"#{title}\"`
      end
    end

  end

end
