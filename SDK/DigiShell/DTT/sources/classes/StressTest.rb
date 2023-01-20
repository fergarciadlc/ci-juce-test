#
#   StressTest.rb
#
#   Created by Sergii Anisienko
#
#   Copyright (c) 2020 Avid Technology. All rights reserved.
#

module DishTestTool
  class StressTest

    # pi stress test vars
    DEFAULT_PLUGINS = {
      #      :dsp    => ["Digi", "Sign", "11SG"],   # sig gen for audible test
      :dsp    => ["AVID", "DGin", "DGDT"],   # for ordinary test
      :native => ["AVID", "DGin", "DGDR"]
    }

    # user cpu stress test vars
    CONN_TYPE = "ssh" #"/usr/local/bin/telnet"
    USER = "root"
    APP = "dsh"

    # controls stress vars
    CONTROL_ID = 2
    CONTROL_VAL = 33
    INTERVAL_SAMPLES = 24000
    CONTROLS_NUM = 5

    # common stress test vars
    WAIT_TIME = 0.137
    TIMER_Q = 0.2  #timer increments

    # constructor
    def initialize(dsh: nil, host: nil, time: nil, stress_type: "ABC", dsp_plugin: DEFAULT_PLUGINS[:dsp], nat_plugin: DEFAULT_PLUGINS[:native], interval_samples: INTERVAL_SAMPLES, controls_num: CONTROLS_NUM, multi_dsp: false)
      @dsh = dsh
      @host = host
      @time = time
      @stress_type = stress_type
      @dsp_plugin = dsp_plugin
      @nat_plugin = nat_plugin
      @interval_samples = interval_samples
      @controls_num = controls_num
      @multi_dsp = multi_dsp

      @threads = Array.new
      @instances = Array.new
      @sub_dsh = Array.new
      @control_pi = Array.new
      @failures = Array.new
      @force_stop = false
      @wait4main = true

      "Initialising stress engine:".log_status

      usrcpu_stresst(host) if @stress_type.include?("A")
      pi_load_stresst(set_plugin) if @stress_type.include?("B")
      pi_control_stresst(set_plugin) if @stress_type.include?("C")

      "DONE!".log_status

    end

    # A - stress loading user space CPU with multiple threads of continuous buffer comparison
    def usrcpu_stresst(host, num_threads = 8, silent = true)

      "   Preparing CPU stress logic...".log_status
      begin

        #create dsh's on remote machine
        num_threads.times do |i|
          print "      "
          @instances << RemoteDSH.new(host: host["ipv6"], user: USER, connection_type: CONN_TYPE, app_path: APP)
          @instances[i].start_ssh
          @res_a = @instances[i].bnew(100)["buffer_ref"]
          @res_b = @instances[i].bnew(100)["buffer_ref"]
        end

        #create threads
        num_threads.times do |i|
          @threads << Thread.new {

            #wait for main test to fire stress
            sleep(TIMER_Q) while @wait4main

            #fire in the hole
            until @force_stop do
              @instances[i].bcmp(@res_a, @res_b)
              print "Stressing CPU complete: #{((time-@time_left)*100/time)}% \r" if !silent #completeness indicator
            end
          }
        end

      rescue Exception => e
        p e
        raise "Stress test logic failed."
      end

    end # A

    # B - stress by loading/unloading pi to reserved DSP
    def pi_load_stresst(plugin, silent = true)

      "   Preparing PI-load stress logic...".log_status
      begin

        sub_dsh = SubDSH.new(@dsh.new_shell)
        print "      SubDSH: "
        puts sub_dsh

        @threads << Thread.new {

          #wait for main test to fire stress
          sleep(TIMER_Q) while @wait4main

          # loading/unloading plug-in
          until @force_stop do
            sub_dsh.run("spec" => plugin, "num_insts" => 1, "DSP_hint" => @num_dsps-1)
            sleep(WAIT_TIME)
            pi_index = sub_dsh.getcurrentinstance
            print "Loading/unloading plug-ins: #{pi_index} \r" if !silent
            sub_dsh.unload(pi_index)
          end

          puts "#{pi_index} plug-ins loaded/unloaded"
        }

      rescue Exception => err
        raise "PI stress test logic failed. #{err}"
      end

    end # B

    # C - stress by rapidly changing several pi's controls in multiple threads
    def pi_control_stresst(plugin, silent = true)

      "   Preparing PI-controls stress logic...".log_status
      begin

        # create pipes
        "      Creating pipes for MULTI control stress:".log_status
        @controls_num.times do |k|
          @sub_dsh << SubDSH.new(@dsh.new_shell)
          print "      #{k}"
          puts @sub_dsh[k]
#          sleep(WAIT_TIME)
        end

        # instantiate control plug-ins
        if @multi_dsp
          npi = 0
          @controls_num.times.each_slice(@num_dsps) do |x|
            
            for i in 0..x.size-1 do
            @dsh.run("spec" => plugin, "num_insts" => 1, "DSP_hint" => i)
            @control_pi << @dsh.getcurrentinstance
            puts "      Instantiating plug-ins for control stress: #{npi} @DSP#{i} "
            npi+=1
            sleep(WAIT_TIME)
            end
          end               
        else
          @controls_num.times do |x|
            @dsh.run("spec" => plugin, "num_insts" => 1, "DSP_hint" => @num_dsps-1)
            @control_pi << @dsh.getcurrentinstance
            puts "      Instantiating plug-ins for control stress: #{x} "
            sleep(WAIT_TIME)
          end        
        end

        # fire control changes to previously instantiated plug-in in parallel threads
        
        @control_pi.each do |pi|
          @threads << Thread.new {
            #wait for main test to fire stress
            sleep(TIMER_Q) while @wait4main

            puts "Running control stresst for plug-in by idx #{pi}" if !silent  
            res = @sub_dsh[pi].control_stress(pi, CONTROL_ID, 0, CONTROL_VAL, @interval_samples, get_itterations_num)["value_current"]

            unless res == CONTROL_VAL
              @failures << "Control stress failed for plug-in idx #{pi}, expected value: #{CONTROL_VAL}, actual value: #{res}"
            else
              sleep(1)
              puts "Plug-in idx: #{pi}, Result: #{res}, Times triggered: #{get_itterations_num}"
            end
            @sub_dsh[pi].exit #gracefully close sub dsh shell
          }
        end

      rescue Exception => err
        raise "Control stress test logic failed. #{err}"
      end

    end # C

    def start

      @wait4main = false

    end

    def stop

      @force_stop = true

      "Shutting down stress engine...".log_status

      @threads.each {|thr| thr.join } if !@threads.empty?
      @sub_dsh.each {|subd| subd.close } if !@sub_dsh.empty? #gracefully close pipes and streams
      @instances.each {|inst| inst.close_ssh } if !@instances.empty?

      if @failures.size != 0
        puts @failures
        raise "Stress logic failed."
      end

      "DONE.".log_status

    end

    def get_itterations_num

      itterations_num = ((@time*@sample_rate) / (@interval_samples*2)).to_i

    end

    # choose test plug-in based on deck_mode parameter
    def set_plugin

      @deck = @dsh.getdeckproperties["deck_type"]
      @sample_rate = @dsh.getdeckproperties["hw_sample_rate"]

      case @deck
      when "hdx"
        pi = @dsp_plugin
        @num_dsps = 18
      when "zeta"
        pi = @dsp_plugin
        @num_dsps = 8
      when "native"
        pi = @nat_plugin
        @num_dsps = nil
      else raise "Incorrect deck_mode parameter, should be hdx, zeta or native"
      end
      
      
      return pi

    end

  end
end
