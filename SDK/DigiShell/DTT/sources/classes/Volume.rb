#
#
# Copyright 2014 by Avid Technology, Inc.
#
#

module DishTestTool

=begin rdoc
Stores info about target volume.
=end
  class Volume

    attr_reader :name, :free_space, :drive_type, :capacity, :file_system, :id

    def initialize(name, id, free_space, capacity, drive_type, file_system, platform)
      @name = name
      @id = id
      @free_space = free_space
      @capacity = capacity
      @drive_type = drive_type
      @file_system = file_system
      @platform = platform
    end

=begin rdoc
    Returns drive info in a multi-line human-readable format.
=end
    def info
      return ["#@name:", "ID: #@id", "Free Space: #@free_space", "Capacity: #@capacity",
      "Drive Type: #@drive_type", "File System: #@file_system"].join("\n  ")
    end

=begin rdoc
    Returns absolute path to drive.
=end
    def path
      return Pathname.new((@platform == :mac) ? "/Volumes/#@name" : @id)
    end

=begin rdoc
    Returns true if an audio volume.
=end
    def audio?
      return !IS_AUDIO_VOLUME.match(@name).nil?
    end

=begin rdoc
    Returns true if a video volume.
=end
    def video?
      return !IS_VIDEO_VOLUME.match(@name).nil?
    end

=begin rdoc
    Overloaded comarison operator. Currently alpha by volume name.
=end
    def <=>(compare_vol)
      return self.name <=> compare_vol.name
    end

=begin rdoc
    Returns drive info in a compact one-line string.
=end
    def to_s
      return "#@name (#@id, #@free_space/#@capacity)"
    end
  end

end
