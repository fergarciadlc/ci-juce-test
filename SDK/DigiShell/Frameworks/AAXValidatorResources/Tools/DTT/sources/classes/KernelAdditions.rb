#
#  KernelAdditions.rb
#  Goldsmith
#
#  Created by Tim Walters on 4/23/08.
#  Copyright 2014 by Avid Technology, Inc.
#
#  Refactored by Sergii Dekhtiarenko 03/22/2011 for Dish Test Tool#

include DishTestTool

=begin rdoc
  Methods added to Object class for Goldsmith convenience.
=end
class Object

=begin rdoc
Returns +alt+ if receiver is nil, receiver otherwise. Useful for
setting default arguments. Standard Ruby practice is to
use ||=, but this fails if receiver is +false+ and +alt+ is
+true+.
=end
  def or_if_nil(alt)
    return self.nil? ? alt : self
  end

=begin rdoc
Returns true if all of +o+'s instance variables are equal
to those of receiver.
=end
  def var_match(o)
    return self.instance_variables.collect { |v| self.instance_variable_get(v) == o.instance_variable_get(v) }.inject { |a, b| a && b }
  end

end # Object additions


=begin rdoc
  Methods added to Array class for Goldsmith convenience.
=end
class Array

=begin rdoc
Returns array of all elements of receiver not equal to +item+.
=end
  def reject_if(item)
    return self.reject { |x| x == item }
  end

=begin rdoc
Returns randomly chosen element of receiver.
=end
  def choose
    return self[rand(self.length)]
  end

=begin rdoc
Returns last index of receiver, or nil if receiver is empty.
=end
  def last_index
    return self.empty? ? nil : self.length - 1
  end

=begin rdoc
  Use to convert key/value array returned from Pro Tools into a hash.
=end
  def pairs_to_hash(conversion)
    hash = {}
    "Invalid hash received from Pro Tools.".abort(self) if self.length.odd?
    (self.length / 2).times do
      |i|
      key = self[2 * i].to_sym
      value = self[2 * i + 1]
      hash[key] = value.send(conversion.or_if_nil(:to_s))
    end
    return hash
  end

  unless self.method_defined?(:sum)
    def sum
      inject(0) { |sum, x| sum+x }
    end
  end

end # Array additions

=begin rdoc
  Methods added to String class for Goldsmith convenience.
=end
class String

=begin rdoc
Prints receiver (plus a newline) to current log. Returns
the receiver.
=end
  def log
    LOG_MANAGER.log(self)
    return self
  end

=begin rdoc
Prints receiver (plus a newline) to current verbose log.
Returns the receiver.
=end
  def log_verbose
    LOG_MANAGER.log_verbose(self)
    return self
  end

=begin rdoc
Prints receiver (plus a newline) to current info log.
Returns the receiver.
=end
  def log_info
    LOG_MANAGER.log_info(self)
    return self
  end

  def log_dsh
    LOG_MANAGER.log_dsh(self)
    return self
  end

  def log_status
    LOG_MANAGER.log_status(self)
    return self
  end

  def log_dsh_cmd
    LOG_MANAGER.log_dsh_cmd(self)
    return self
  end

=begin rdoc
Prints receiver (plus a newline) to current log as a warning.
Returns the receiver.
=end
  def warn
    LOG_MANAGER.warn(self)
    return self
  end

=begin rdoc
Prints receiver (plus a newline) to current log as an error,
and increments non-fatal error count. Use when you want a
strong warning, but are willing to roll the dice and continue
without throwing a GoldsmithError.
=end
  def non_fatal_error
    LOG_MANAGER.non_fatal_error(self)
    return self
  end

=begin rdoc
Throws a GoldsmithError with the receiver as text. This will
abort the current script.
=end
  def abort(calling_obj = nil)
    state_dump = calling_obj.nil? ? '' : "Calling object: #{calling_obj.inspect}"
    return raise_dtt_err(self, [state_dump] + caller)
  end

=begin rdoc
Returns +true+ if receiver contains "true" or "yes" (case-insensitive) as a substring.
Returns +false+ if receiver contains "false" or "no". Returns nil otherwise.
=end
  def to_boolean
    true_re = Regexp.new('true|yes', Regexp::IGNORECASE)
    false_re = Regexp.new('false|no', Regexp::IGNORECASE)
    return true if true_re.match(self)
    return false if false_re.match(self)
    return nil
  end

=begin rdoc
  Returns an array with the receiver as its only element. Useful when you want to accept either
  a list of strings or a string.
=end
  def to_a
    return [self]
  end

=begin rdoc
Returns a new String with spaces in receiver replaced by underscores.
=end
  def despace
    return self.gsub(' ', '_')
  end

=begin rdoc
Returns a new String with underscores in receiver replaced by spaces.
=end
  def respace
    return self.gsub('_', ' ')
  end

=begin rdoc
Splits receiver on +split_str+, then returns array with empty strings
omitted. Example: +"x//y/z".split_without_blanks('/') => ["x", "y", "z"]+.
=end
  def split_without_blanks(split_str)
    return self.split(split_str).reject { |s| s == "" }
  end

=begin rdoc
Splits receiver on Goldsmith::SEPARATOR.
=end
  def separate
    return self.split_without_blanks(SEPARATOR)
  end

=begin rdoc
Splits receiver on Goldsmith::SUB_SEPARATOR.
=end
  def sub_separate
    return self.split_without_blanks(SUB_SEPARATOR)
  end

=begin rdoc
  Added here to support polymorphism with arg types.
=end
  def to_menu_item
    return self
  end

=begin rdoc
  Duck typing so that you can use a manually constructed String
  instead of a Pathname.
=end
  def tcl_path
    return self
  end

  def to_uint32
    return self.reverse.unpack('L')[0]
  end

=begin rdoc
  Decrypts a String that contains encrypted Digitrace data. Must start at beginning of digitrace file.
=end
  def decrypt
    munge_seed = 0x42

    munger = (self[0].ord * munge_seed) & 0xFF

    decrypted_data = []

    for i in 1..(self.length - 1) do
      mult = (i & 0xFF) * munger
      source = self[i].ord
      result = mult ^ source
      decrypted_data.push(result)
    end

    return decrypted_data.pack('C*')
  end

end # String additions

=begin rdoc
  Methods added to Numeric class for Goldsmith convenience.
=end
class Numeric

=begin rdoc
Returns +1+ if receiver is positive, +-1+ if receiver is negative, and
+0+ if receiver is zero.
=end
  def sign
    return self.<=>(0)
  end

=begin rdoc
Returns maximum of receiver and +x+.
=end
  def max(x)
    return (self >= x) ? self : x
  end

=begin rdoc
Returns minimum of receiver and +x+.
=end
  def min(x)
    return (self <= x) ? self : x
  end

=begin rdoc
Returns true if +x+ <= receiver <= +y+ or if +y+ <= receiver <= +x+.
=end
  def within(x, y)
    return ((self <= x.max(y)) && (self >= x.min(y)))
  end

=begin rdoc
  Converts number of seconds to a nicely formatted string for log.
=end
  def elapsed_time_string
    i_seconds = self.to_i
    days = i_seconds / 86400
    i_seconds = i_seconds % 86400
    hours = i_seconds / 3600
    i_seconds = i_seconds % 3600
    minutes = i_seconds / 60
    i_seconds = i_seconds % 60

    day_string = (days == 1) ? 'day' : 'days'
    hour_string = (hours == 1) ? 'hour' : 'hours'
    minute_string = (minutes == 1) ? 'minute' : 'minutes'
    second_string = (i_seconds == 1) ? 'second' : 'seconds'

    time_string = ""
    time_string += "#{days} #{day_string}, " if days > 0
    time_string += "#{hours} #{hour_string}, " if (days > 0) or (hours > 0)
    time_string += "#{minutes} #{minute_string}, " if (days > 0) or (hours > 0) or (minutes > 0)
    time_string += "#{i_seconds} #{second_string}."
    return time_string
  end

end # Numeric additions

=begin rdoc
  Methods added to Fixnum class for Goldsmith convenience.
=end
class Fixnum

=begin rdoc
  Returns true if receiver is even, false otherwise. Already
  defined in Ruby 1.8.7 and later, so we check first to
  avoid a warning.
=end
  if !1.respond_to?(:even?)
    def even?
      return (self % 2) == 0
    end
  end

=begin rdoc
  Returns true if receiver is odd, false otherwise. Already
  defined in Ruby 1.8.7 and later, so we check first to
  avoid a warning.
=end
  if !1.respond_to?(:odd?)
    def odd?
      return !self.even?
    end
  end

=begin rdoc
Returns random string of length +receiver+ with only alphabetic characters.
=end
  def random_alpha_string
    chars = (65..90).to_a + (97..122).to_a
    return Array.new(self) { chars.choose.chr }.join('')
  end

=begin rdoc
Creates random string of length +receiver+ drawn from all printable ASCII characters.
=end
  def random_string
    chars = (32..126).to_a
    return Array.new(self) { chars.choose.chr }.join('')
  end

=begin rdoc
  Returns a string represent the hexadecimal value.
=end
  def to_h
    return sprintf("0x%x", self)
  end

end # Fixnum additions

=begin rdoc
  Methods added to Symbol class for Goldsmith convenience.
=end
class Symbol

=begin rdoc
Converts internal symbol key to string for display use.
=end
  def key_to_string
    return self.to_s.gsub('_', ' ').split.collect { |w| w.capitalize.gsub(/^Pt$/, 'Pro Tools') }.join(' ')
  end

end # Symbol additions

=begin rdoc
  Methods added to Time class for Goldsmith convenience.
=end
class Time

=begin rdoc
  Returns time elapsed since the receiver.
=end
  def since
    return Time.now - self
  end

end

=begin rdoc
  Methods added to Class class for Goldsmith convenience.
=end
class Class

=begin rdoc
Returns class name without ancestry.
=end
  def short_name
    return self.name.split('::')[-1]
  end

end # Class additions

=begin rdoc
  Methods added to Pathname class for Goldsmith convenience.
=end
class Pathname

  unless self.method_defined?(:sub_ext)
    def sub_ext(repl)
      ext = File.extname(@path)
      self.class.new(@path.chomp(ext) + repl)
    end
  end

=begin rdoc
  Saves object to a file in YAML format.
=end
  def write_obj(obj)
    begin
      self.open('w') { |f| YAML.dump(obj, f) }
    rescue
      "Could not save object to file: #{$!}.".abort(self)
    end
    return self
  end

=begin rdoc
Retrieves object from a YAML file. Returns nil if file does not exist.
=end
  def load_obj
    obj = nil
    if self.exist?
      begin
        obj = YAML.load_file(self)
      rescue
        "Error reading object from file: #{$!}".abort(self)
      end
    end
    return obj
  end

=begin rdoc
  Converts to PT's OS9-style internal path type. Must be called before
  sending a path to PT.
=end
  def tcl_path(platform)
    new_path = self.to_s.split('/')
    # In OS9, drive name is root, not "/Volumes/"
    new_path.delete_at(0) if new_path[0] == ''
    new_path.delete_at(0) if new_path[0] == 'Volumes'
    return new_path.join(TCL_PATH_DELIMITERS[platform])
  end

=begin rdoc
  Returns true if receiver Pathname exists on target.
=end
  def t_exist?(target)
    return target.perform_on_target(self, :exist?)
  end

=begin rdoc
  Returns entries of receiver on target.
=end
  def t_entries(target)
    return target.perform_on_target(self, :entries)
  end

=begin rdoc
  Returns children of receiver on target.
=end
  def t_children(target)
    return target.perform_on_target(self, :children)
  end

=begin rdoc
  Returns true if receiver is a directory on target.
=end
  def t_directory?(target)
    return target.perform_on_target(self, :directory?)
  end

end

=begin rdoc
  Methods added to Float class for DishTestTool.
=end
class Float
  #ruby 1.8.6 doesn't have round(n), just round()
  begin
    1.0.round(1)
  rescue ArgumentError
    alias_method(:orig_round, :round)
    def round(n=0)
      return self.orig_round if n == 0
      round_val = 0.5/(10**n)
      scale_val = 10**n
      scale_val_rp = 1.0/(10**n)
      ((self + round_val) * scale_val).to_i * scale_val_rp
    end
  end
end

=begin rdoc
  Methods added to TrueClass class for Goldsmith convenience.
=end
class TrueClass

=begin rdoc
  Passes through +self+ if asked to convert +to_boolean+.
=end
  def to_boolean
    return self
  end

end

=begin rdoc
  Methods added to FalseClass class for Goldsmith convenience.
=end
class FalseClass

=begin rdoc
  Passes through +self+ if asked to convert +to_boolean+.
=end
  def to_boolean
    return self
  end

end
