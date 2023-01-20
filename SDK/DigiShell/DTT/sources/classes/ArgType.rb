#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool

module DishTestTool

=begin rdoc
  Abstract parent class for argument types, which are used to constrain argument values.
  ArgTypes "know" several things: the set of acceptable values for a given argument;
  the default value; the strings representing those values in app communication; the strings
  representing those values to the user in the Goldsmith UI; and how to convert among these
  representations.
  
  Most of the functionality is provided in the base class (or in the subclass DialogArgType), 
  so that subclasses need only supply class-specific data. At minimum, a subclass must override
  class variable SPEC in order to have meaningful functionality. Here's an example of a simple 
  argtype:
  
    class ExampleArgType < ArgType
      SPEC = {
        :foo => 'eProToolsEnum_Foo',
        :bar => 'eProToolsEnum_Bar'
      }
    end
  
  Values should not have spaces; use underlines instead. They will be converted when necessary.
  
  Now you can create instances by using the hash keys as pseudo-methods, and
  set up a script argument by creating an entry in the script's inputs hash:
  
    :example => [ExampleArgType.foo]
  
  This means that (1) when you select the script in the GUI, it will create an Example pop-up, with "Foo"
  and "Bar" as the options, and "Foo" the default, and (2) you can pass values on the command line
  using -a, e.g.
  
    runsuite myscript -a example=foo
  
  In the script you would then read the value as desired, e.g.
  
    if example == ExampleArgType.foo
      ...
    end
=end
  class ArgType
  
    # override for non-string types
    CONVERSION = :to_s
  
    # Must be overridden with symbol/string pairs
    SPEC = {}
  
    # When using numeric keys, which require special syntax, this allows for
    # alternative, easier creation. See SampleRate for an example.
    ALIASES = {}

    # Override if numeric sort is desired
    NUMERIC_SORT = false
  
=begin rdoc
  Typically, you should create ArgType instances by using the key as a pseudo-method (e.g. ExampleArgType.foo),
  but the initialize method is flexible and allows several other options: 
  
    ExampleArgType.new(:foo)
    ExampleArgType.new("foo")
    ExampleArgType.new(ExampleArgType.foo)
  
  This allows you to be generous when defining methods for scripters. If the initialize method can't figure
  out what you mean, it throws a GoldsmithError.
=end
    def initialize(key, default = nil)
      "Can't create #{self.class} with nil value.".abort(self) if (key.nil? && default.nil?)
      key_to_use = key.nil? ? default : key
      key_sym = case key_to_use
      when self.class
        key_to_use.key
      when Symbol
        key_to_use
      else
        key_to_use.to_s.downcase.to_sym
      end
      final_key = self.class.spec.has_key?(key_sym) ? key_sym : self.class::ALIASES[key_sym]
      "No such #{self.class}: #{key} (#{key.class}). Valid keys: #{self.class.dump_keys}.".abort(self) if final_key.nil?
      @key = final_key
    end
  
=begin rdoc
  Returns value as a string, regardless of native type.
=end
    def to_s
      return self.value.to_s
    end
    
=begin rdoc
  Returns key (not value) as a symbol.
=end
    def to_sym
      return @key
    end
  
=begin rdoc
  Returns a sorted list of the valid keys for this ArgType. See also ArgType#all.
=end
    def self.dump_keys
      return self.spec.keys.collect { |k| k.to_s }.sort.join(', ')
    end
  
=begin rdoc
  This is the mechanism that implements pseudo-methods like ExampleArgType.foo.
  Don't override it!
=end
    def self.method_missing(method)
      return self.new(method)
    end
    
=begin rdoc
  Two ArgTypes are equal if they are of the same class and have the same key.
=end
    def ==(arg)
      return (arg.class == self.class) && (arg.key == @key)
    end
  
=begin rdoc
  Compares numerically or alphabetically, depending on value of NUMERIC_SORT. :highest and :lowest
  are special cases; the former always comes last, the latter first. This allows correct menu 
  setup in the GUI.
=end
    def <=>(arg)
      return 1 if arg.key == :highest
      return -1 if @key == :highest
      return 1 if arg.key == :lowest
      return -1 if @key == :lowest
      return self.class::NUMERIC_SORT ? (@key.to_s.to_i <=> arg.key.to_s.to_i) : (@key.to_s <=> arg.key.to_s)
    end
  
=begin rdoc
  Returns a sorted ist of all valid instantiations, e.g. [ExampleArgType.foo, ExampleArgType.bar]. Useful
  for iteration, or for random selection (ExampleArgType.all.choose).
=end
    def self.all
      return self.spec.keys.collect { |key| self.new(key) }.sort
    end

 
=begin rdoc
  Returns an ArgType whose key matches the input value, e.g. ExampleArgType.new('eProToolsEnum_Foo').
  Used when converting app output to our representation--you shouldn't need to call it yourself.
=end
    def self.newFromValue(value)
      key = self.spec.keys.find { |k| self.spec[k].downcase == value.to_s.downcase }
      "Can't create a #{self.short_name} with value #{value}.".abort(self) if key.nil?
      return self.new(key)
    end
  
=begin rdoc
  Returns an ArgType whose key matches the input display value, e.g. PluginName.new('EQ 3 7-Band').
  Used when converting GUI output to our representation--you shouldn't need to call it yourself.
=end  
    def self.newFromDisplay(displayValue)
      arg = self.all.select {
        |arg_val|
        arg_val.display == displayValue.respace || arg_val.display == displayValue
      }[0]
      "Can't create a #{self.short_name} with display value #{displayValue}.".abort(self) if arg.nil?
      return arg
    end
  
=begin rdoc
  Returns value, applying method specified by CONVERSION (the default is :to_s). Other useful possibilities:
  :to_i, :to_f, :to_boolean.
=end
    def value
      return self.class.spec[@key].send(self.class::CONVERSION)
    end

=begin rdoc
  Returns the desired menu item for Pro Tools. By default, just capitalizes the key, so ExampleArgType.foo
  would return "Foo". Override if you need different behavior.
=end
    def to_menu_item
      return @key.to_s.capitalize
    end
  
=begin rdoc
  Returns a "nice" value for GUI display. By default, returns the key with underlines replaced with spaces
  and each word capitalized. Override if you need different behavior.
=end
    def display
      @key.to_s.split('_').collect { |s| s.capitalize }.join(' ')
    end
 
    protected
  
    def key
      return @key
    end
  
    def self.spec
      return self::SPEC
    end
  
  end

  class TestMachineConfig < ArgType

    SPEC = {
      :kyiv_test_pc1 => 'KyivTestPC1',
      :kyiv_test_mac1 => 'KyivTestMac1',
      :kyiv_test_mac2 => 'KyivTestMac2',
      :default_machine => 'DefaultMachine',
      :berlin => 'Berlin',
      :berlin3 => 'Berlin3',
      :berlin5 => 'Berlin5',
	  :berlin7 => 'Berlin7',
      :mandy => 'Mandy',
	  :Zeta8 => 'Zeta8'
    }

    def display
      return value
    end

  end

end
