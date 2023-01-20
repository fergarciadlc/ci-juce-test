#
#  Logger.rb
#  Goldsmith
#
#  Created by Tim Walters on 4/23/08.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

=begin rdoc
  Takes charge of writing strings to various log files. Loggers are created as needed by the
  LogManager, and you shouldn't need to create them yourself; calling String#log at any time
  should log to the correct destination.
=end
  class Logger  # :nodoc:

    @@endings = {
      :log => '_l.txt',
      :verbose => '_v.txt',
      :info => '_i.txt',
      :dsh => '_d.txt',
      :dsh_cmd => '_c.txt'
    }

    attr_reader :verbose, :outputs, :log_dir
    attr_writer :verbose, :log_elapsed_time

    def initialize(log_dir, verbose = false, log_elapsed_time = true, console_io = nil)

      prefix = 'Error initializing Logger:'
      @log_dir = log_dir
      @verbose = verbose
      @start_time = Time.now
      @log_elapsed_time = log_elapsed_time
      @outputs = {}
      @suspend_console = false
      @suspended_consoleDev = Tempfile.new('devnulFile')

      if console_io.nil?
        @console_io = $stderr
      else
        begin
          @console_io = File.open(console_io, 'w')
        rescue Exception => err
          $stderr.puts("Couldn't open console log file #{console_io}: #{err}. Using standard error.")
          @console_io = $stderr
        end
      end
      "#{prefix} no log directory specified.".abort(self) if @log_dir.nil?

      if @log_dir.exist? then
        "#{prefix} #{@log_dir} is not a directory.".abort(self) if not @log_dir.directory?
      else
        @log_dir.mkdir rescue "#{prefix} could not create log directory #{@log_dir}.".abort(self)
      end

      begin
        @@endings.keys.each do
          |key|
          @outputs[key] = get_log_file_path(key).open('a+')
        end
      rescue SystemCallError => err
        close
        "#{prefix} at #{@log_dir}: #{err}".abort(self)
      end

    end

    def suspend_console(suspend)
      if @suspend_console != suspend
        @suspend_console = suspend
        if @suspend_console
          @origConsoleDev = @console_io
          @origstdOut = $stdout
          @origstdErr = $stderr

          #reassign both our console io and the system stdout
          @console_io = @suspended_consoleDev
          $stdout = @suspended_consoleDev
          $stderr = @suspended_consoleDev
        else
          @console_io = @origConsoleDev
          $stdout = @origstdOut
          $stderr = @origstdErr
        end
      end
    end

    def close
      @outputs.keys.each { |key| @outputs[key].close }
    end

    def get_log_file_path(key)
      if key === :console then
        return "standard err"
      else
        return Pathname.new(@log_dir.to_s + File::SEPARATOR + log_root + @@endings[key])
      end
    end

    def log_root
      return @log_dir.basename.to_s.chomp('/')
    end

    def log(text)
      write_to_file(:log, text, false)
      write_to_file(:verbose, text, true)
    end

    def log_status(text)
      [:console, :log].each { |key| write_to_file(key, text, false) }
      write_to_file(:verbose, text, true)
    end

    def log_verbose(text)
      outputs = @verbose ? [:log, :verbose, :console] : [:verbose]
      outputs.each { |key| write_to_file(key, text, true) }
    end

    def log_info(text)
      write_to_file(:info, text, false)
    end

    def log_dsh(text)
      write_to_file(:dsh, text, false)
    end

    def log_dsh_cmd(text)
      write_to_file(:dsh_cmd, text, false)
    end

    def can_log_elapsed_time(key)
      return ![:dsh, :info, :result, :console, :dsh_cmd].include?(key)
    end

    def write_to_file(key, text, time_stamped = false)
      prefix = time_stamped ? TIME_INFO.time_log + ' ' : ''
      if @log_elapsed_time && can_log_elapsed_time(key)
        prefix += get_elapsed_time + ' '
      end
      text_to_write = text.split("\n").collect { |line| prefix + line }.join("\n")
      output = (key == :console) ? @console_io : @outputs[key]
      begin
        output.puts(text_to_write)
        output.flush
      rescue IOError, SystemCallError => err
        "Error writing log to #{get_log_file_path(key)}: #{err}.".abort(self)
      end
    end

    def get_elapsed_time
      e_time = (Time.now - @start_time).to_i
      days = e_time / (60 * 60 * 24)
      hours = e_time % (60 * 60 * 24) / (60 * 60)
      minutes = e_time % (60 * 60) / 60
      seconds = e_time % 60
      return sprintf("[%02d:%02d:%02d:%02d]", days, hours, minutes, seconds)
    end

    def write_master_log(script, start_time, result_in, export_logs)
      result = result_in.nil? ? "This script did not log a result." : result_in.to_s
      logs = {}
      [:log, :verbose, :dsh].each { |type| logs[type] = get_log_file_path(type) }
      t = Time.now
      time_strings = ["Start: #{start_time}", "End: #{t}",  "Duration: #{(t - start_time).elapsed_time_string}"]
      cgi = CGI.new("html3")
      raw_sys_info = File.read(get_log_file_path(:info))
      html = cgi.html {
        cgi.head { "\n" + cgi.title(log_root) } +
        cgi.body {
          "\n" + cgi.h1 { script.class.short_name } +
          "\n" + time_strings.collect { |s| s + cgi.br }.join("\n") +
          "\n" + cgi.h2 { "Result" } +
          "\n" + cgi.pre { result } +
          "\n" + cgi.h2 { "Arguments" } +
          "\n" + cgi.table("border" => "1") {
            script.class.gui_order.collect { |arg|
              cgi.tr { cgi.td { arg.to_s } + cgi.td { script.options[arg].to_s } }
            }.join("\n")
          } +
          "\n" + cgi.h2 { "Target System Info" } +
          "\n" + cgi.table("border" => "1") {
            raw_sys_info.split("\n").collect { |line|
              cgi.tr { line.split(": ").collect { |item| cgi.td { item } }.join("\n") }
            }.join("\n")
          } +
          "\n" + cgi.h2 { "Logs" } +
          "\n" + cgi.p { "Standard: " + cgi.a(logs[:log].basename.to_s) { logs[:log].basename.to_s } } + "\n" +
          "\n" + cgi.p { "Verbose: " + cgi.a(logs[:verbose].basename.to_s) { logs[:verbose].basename.to_s } } + "\n" +
          "\n" + cgi.p { "DSH output: " + cgi.a(logs[:dsh].basename.to_s) { logs[:dsh].basename.to_s } } + "\n"
        }
      }
      begin
        p = @log_dir + (@log_dir.basename.to_s + ".html")
        f = File.new(p, "w")
        f.print(html)
        f.close
      rescue
        "Error writing index page to #{p}.".warn
      end
      #When we create instance of CGI (.new('html3')), CGI define dynamically CGI::Html3 module(with each own methods ) which live with program.
      #To avoid warnings(redefined method) before further call write_master_log we need undef these methods.
      CGI::Html3.module_eval do
        # instance_methods(false).each { |meth| undef_method(meth) unless meth.to_s =~ /(element_init|doctype)/ }
      end

      #Exporting compressed logs to HOME dir, so they are not wiped by consequent test run
      if export_logs
        begin
          
          #checking/creating a folder for each suite run to organize relevant logs by suite start time
          ex_logs_dir = ENV['HOME'] + "/" + TIME_INFO.start_stamp + "_logs/"
          unless File.directory?(ex_logs_dir)
            FileUtils.mkdir_p(ex_logs_dir)
          end
          
          #creating file with log folder path for further Git interaction
          f = File.new(SOURCE_DIR + "../../../export_logs_path.txt", "w")
          f << ex_logs_dir
          f.close
          
          #adding DSH logs to DTT log dir
          FileUtils.copy(RUNTIME_INFO.dsh_log_files, @log_dir)
          
          #creating log acrhive
          file_path = ex_logs_dir + @log_dir.basename.to_s + ".tar.gz"
          print "Exporting logs to #{file_path}... "
          system("tar -czf #{file_path} -C #{@log_dir} .")
          puts "DONE."
        rescue Exception => err
          puts "Exporting logs failed. #{err}"
        end
      end

    end
  end # Logger

end # module
