module DishTestTool
  class SuiteResult

    PASSED = 0
    FAILED = 1

    LOST = 2
    TIMEOUT = 3
    ABORTED = 4
    CANCELED = 5

    RESULTSTATUS_UNKNOWN = 6

    attr_accessor :suite_result, :suite_errors, :pb_host, :pb_port
    attr_reader :total, :failed, :successful, :errors, :not_run, :warnings, :suite_warnings, :started_time, :elapsed_time

    def initialize(export_logs = false)
      @suite_result = nil
      @suite_errors = Array.new
      @script_results = Array.new
      @suite_warnings = Array.new
      @backtrace = Array.new
      @pb_result = nil
      @pb_host = 'localhost'
      @pb_port = PROTOBUFF_PORT
      @export_logs = export_logs

      @total = 0
      @failed = 0
      @errors = 0
      @not_run = 0
      @warnings = 0
      @successful = 0

      @started_time = Time.new
      @elapsed_time = Time.new
    end

    def add_suite_error(error)
      @suite_errors.push(error)
    end

    def add_suite_warning(warning)
      @suite_warnings.push(warning)
    end

    def add_script_result(result)
      @script_results.push(result)
    end

    def backtrace(backtrace)
      @backtrace += backtrace
    end

    def write_result
      result = ''
      result += "!!!!!\n"

      @suite_errors.each{|err| result += "#{err}\n"}
      @backtrace.each {|msg| result += "#{msg}\n"}

      passed = @script_results.select {|r| r.result == :pass}
      failed = @script_results.select {|r| r.result == :fail}
      aborted = @script_results.select {|r| r.result == :abort}
      warnings = @script_results.select {|r| !r.warnings.empty?}
      cancelled = @script_results.select {|r| r.result == :cancel}
      @script_results.each {|r| result += "#{r.to_s}\n"}

      result += "#{passed.size} passed, #{failed.size} failed, #{warnings.size} warnings, #{cancelled.size} cancelled, #{aborted.size} failed to complete.\n"
      result += "Total suite time: #{@started_time.since.elapsed_time_string}"
      puts result
      
      if @export_logs
        #creating file with results log for further Git interaction
        f = File.new(SOURCE_DIR + "../../../dtt_test_report.txt", "w")
        f << result
        f.close
      end

      update_protobuff_result
      send_protobuff_result
    end

    def init_protobuff_result(uniq_id)
      @pb_result = AAXValResult::Result.new

      @pb_result.connection_id = uniq_id
      init_protobuff_connection
      send_protobuff_result
    end

    private

    def init_protobuff_connection
      begin
        @pb_connection = ProtoBuffConnection.new(@pb_host, @pb_port)
      rescue SystemCallError, IOError, SocketError => e
        add_suite_error("Failed to connect with Protocol Buffer Host: #{e.message}")
        @pb_connection = nil
      end
    end

    def send_protobuff_result
      return if (@pb_connection.nil? || @pb_result.nil?)
      @pb_connection.send_result(@pb_result)
    end

    def update_protobuff_result
      return if (@pb_result.nil? || @pb_connection.nil?)

      pb_summary = @pb_result.summary

      @script_results.first.sub_test_results.each { |sub_result|
        pb_summary.total += 1
        pb_summary.failed += 1 if sub_result.failed?
        pb_summary.successful = pb_summary.total - pb_summary.failed

        @pb_result.result.push(sub_result.result)
      }

      #Assuming that validator runs only one DTT script
      @pb_result.result_status = pb_result_status(@script_results.first.result)
    end

    def pb_result_status(status = :fail)
      case status
        when :pass, 0
          AAXValResult::EResultStatus::E_COMPLETED_PASS
        when :fail, 1
          AAXValResult::EResultStatus::E_COMPLETED_FAIL
        when :lost, 2
          AAXValResult::EResultStatus::E_LOST
        when :timeout, 3
          AAXValResult::EResultStatus::E_TIMEOUT
        when :abort, 4
          AAXValResult::EResultStatus::E_ABORTED
        when :cancel, 5
          AAXValResult::EResultStatus::E_CANCELED
        else
          AAXValResult::EResultStatus::E_RESULTSTATUS_UNKNOWN
      end
    end
  end

  class SingleTestResult
    def initialize
      @single_test_result = AAXValResult::SingleTestResult.new
      @single_test_failed = false
    end

    def failed
      @single_test_failed = true
    end

    def failed?
      @single_test_failed
    end

    def result
      @single_test_result
    end

    def score=(score)
      @single_test_result.score = score
    end

    def score
      @single_test_result.score
    end

    def effect_id=(id)
      @single_test_result.effect_id = id
    end

    def effect_id
      @single_test_result.effect_id
    end

    def triad_id=(triad)
      raise "Triad had to have 3 elements: manufacture_id, product_id and plugin_id" if triad.size != 3

      triad_msg = AAXValResult::SingleTestResult::Triad.new
      triad_msg.manufacture_id = triad[0]
      triad_msg.product_id = triad[1]
      triad_msg.plugin_id = triad[2]

      @single_test_result.triad_id = triad_msg
    end

    def start_time(time = Time.new)
      time = time.to_i if time.instance_of?(Time)

      @single_test_result.performance_data.started_wall_clock_sec = time
    end

    def elapsed_time(sec = nil)
      time_in_sec = sec.nil? ? (Time.new.to_i - @single_test_result.performance_data.started_wall_clock_sec) : sec
      @single_test_result.performance_data.elapsed_time_sec = time_in_sec
    end

    def log_elapsed_time(e_time = nil)
      e_time = e_time.nil? ? (Time.new.to_i - @single_test_result.performance_data.started_wall_clock_sec) : e_time

      hours = e_time % (60 * 60 * 24) / (60 * 60)
      minutes = e_time % (60 * 60) / 60
      seconds = e_time % 60
      add_log(sprintf("Duration: %02d:%02d:%02d", hours, minutes, seconds))
    end

    def average_ms_per_iteration(ms = 0)
      @single_test_result.performance_data.average_ms_per_iteration = ms
    end

    def max_ms_per_iteration(ms = 0)
      @single_test_result.performance_data.max_ms_per_iteration = ms
    end

    def create_tree_node(name)
      node = @single_test_result.tree.find{|n| n.data == name}

      unless node
        node = AAXValResult::DataTree.new(:data => name)
        @single_test_result.tree.push(node)
      end

      return node
    end

    def add_data_to_node(data, node)
      node.tree.push(create_data_tree(data))
    end

    def add_log(msg = '')
      msg = msg.to_s unless msg.instance_of?(String)

      @single_test_result.logs.push(msg)
    end

    def log_tree(tree = '')
      @single_test_result.tree.push(create_data_tree(tree))
    end

    def create_data_tree(data = nil, node = nil)
      node = AAXValResult::DataTree.new unless node

      return node if data.nil?

      if (data.instance_of?(Array) or data.instance_of?(Set))
        data.each {|elem| node.tree.push(create_data_tree(elem))}
      elsif (data.instance_of?(Hash) and data.size == 1)
        node.data = data.keys.first.to_s
        value = data.values.first

        value = [value] unless (value.instance_of?(Array) or value.instance_of?(Set) or value.instance_of?(Hash))

        create_data_tree(value, node)
      elsif(data.instance_of?(Hash))
        data.each do |key, value|
          sub_node = AAXValResult::DataTree.new(:data => key.to_s)
          value = [value] unless (value.instance_of?(Array) or value.instance_of?(Set) or value.instance_of?(Hash))
          node.tree.push(create_data_tree(value, sub_node))
        end
      else
        node.data = data.to_s
      end
      return node
    end

    def log_test_config(cofig = {})
      test_config = AAXValResult::SingleTestResult::TestConfig.new

      cofig.each do |id, val|
        root = AAXValResult::DataTree.new(:data => id.to_s)
        node = create_data_tree([val], root)

        test_config.tree.push(node)
      end

      @single_test_result.test_config = test_config
    end

    private

    def find_node(name, node)
      if node.data = name
        return node
      else
        node.tree.each { |data_tree| find_node(name, data_tree)}
      end
    end

  end

  class ScriptResult # :nodoc:
    attr_accessor :args, :log_file_path, :target
    attr_reader :result, :single_test_results, :warnings, :description, :sub_test_results

    def initialize(script_name, target, result, description, data, args = {}, sub_test_results = [])
      @script_name = script_name
      @result = result
      @target = target
      @notifier = Notifier.new
      @description = description
      @data = data
      @args = args

      @sub_test_results = sub_test_results

      @log_file_path = ""
      @warnings = []
    end

    def add_warnings(w)
      @warnings += w
    end

    def num_warnings
      return @warnings.size
    end

    def passed?
      return (@result == :pass)
    end

    def failed?
      return (@result == :fail)
    end

    def aborted?
      return (@result == :abort)
    end

    def canceled?
      return (@result == :cancel)
    end

    def pf_string
      case @result
        when :pass
          (num_warnings > 0) ? 'PASSED WITH WARNINGS' : 'PASSED'
        when :fail
          'FAILED'
        when :abort
          'ABORTED'
        when :cancel
          'CANCELED'
        else
          'RESULTSTATUS_UNKNOWN'
      end
    end

    def to_s
      warnings = @warnings.empty? ? '' : "WARNINGS:\n" + @warnings.join("\n") + "\n"
      description = @description.instance_of?(Array) ? @description.join("\n") : @description

      return "#@script_name #{pf_string}: #{description}\n#{warnings}"
    end

    def log
      self.to_s.log
      unless @target.nil?
        target_name = @target.computer_name.to_s
      else
        target_name = ["Unknown Target Name"]
      end
      @notifier.notify(target_name, self.to_s)
    end

  end # ScriptResult
end
