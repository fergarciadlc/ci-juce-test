#
#   DTTUtils.rb - most common methods to make test scripts informative and readable
#
#   Created by Sergii Anisienko in 2020
#   Copyright (c) 2020 Avid Technology. All rights reserved.
#

module DTTUtils
  
  CC_ASYNC_DELAY = 0.2 # 200 milliseconds
  
  #Acquiring any deck with printed params and error handling
  def acquiredeck(flag, sample_rate, deck_mode, block_size)

    case flag
    when 1
      begin
        print "Acquiring deck [#{flag}, #{sample_rate}, #{deck_mode}, #{block_size}]... "
        puts @dsh.acquiredeck(flag, sample_rate, deck_mode, block_size)
        @deck = deck_mode
      rescue Exception => e
        puts e
        raise "Unable to acquire deck."
      end
    when 0
      begin
        print "De-acquiring deck [#{flag}, #{sample_rate}, #{deck_mode}, #{block_size}]... "
        puts @dsh.acquiredeck(flag, sample_rate, deck_mode, block_size)
        @deck = nil
      rescue Exception => e
        puts e
        raise "Unable to de-acquire deck."
      end
    else
      raise "Incorrect parameter: #{flag}"
    end

  end

  def open_dsh
    "Starting new dsh".log_status
    throw '@dsh already opened' unless @dsh.nil?
    @dsh = DigiShell.new(target)
  end

  def close_dsh
    "Closing dsh".log_status
    @dsh.close
  end

  def load_dishes (dishes)
    print "Loading dishes #{dishes}... "
    begin
      @dsh.load_dish(dishes)
    rescue Exception => e
      puts e
      raise "Unable to load dishes"
    end  
    puts "done"
  end
  
end
