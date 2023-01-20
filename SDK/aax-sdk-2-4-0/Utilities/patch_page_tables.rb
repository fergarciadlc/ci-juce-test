#        Copyright 2017 by Avid Technology, Inc.

MODE = ARGV[0]
XML_FILE = ARGV[1]

# ********************************
# Print usage
# ********************************
def print_usage
  puts "Usage: ruby patch_page_tables.rb MODE FILE"
  puts "Runs patch routines on a page table XML file"
  puts "Modes:"
  puts "  1: Generate Av18 from existing Av81"
  puts ""
end

# ********************************
# duplicate the original file in case of undesired results
# ********************************
def duplicate_original_file
  didWrite = false
  if (File.writable?("#{XML_FILE}"))
    file = File.open("#{XML_FILE}")
    linesArr = File.readlines(file)
    if (linesArr != nil)
      File.open("#{XML_FILE}.orig", 'w') do |f|
        linesArr.each { |element| f.puts(element) }
      end
      didWrite = true
    end
    file.close
  end
  
  if didWrite
    puts "original file copied to: #{XML_FILE}.orig"
  end
end

# ********************************
# transpose Av81 to Av18
# ********************************
def transpose_av81_to_av18
  puts "Transposing Av81 to Av18..."
  
  if (File.writable?("#{XML_FILE}"))
    file = File.open("#{XML_FILE}")
    linesArr = File.readlines(file)
    startIdx = linesArr.index{|line| (/<\s*PageTable[^>]*type\s*=\s*\'Av81\'/ =~ line)}
    if (startIdx != nil)
      numLines = linesArr[startIdx,linesArr.length].index{|line| (/<\s*\/\s*PageTable\s*>/ =~ line)} + 1
      endIdx = startIdx + numLines
      
      # duplicate the Av81 table
      linesAv81 = linesArr.slice(startIdx, numLines)
      linesAv81.reverse_each { |element| linesArr.insert(startIdx, String.new(element)) }
      
      # transpose the table
      for lineIdx in startIdx ... endIdx
        line = linesArr.at(lineIdx)
        if (line != nil)
          
          # convert to Av18
          if (/PageTable/ =~ line)
            line.gsub!(/\'Av81\'/, "'Av18'")
          end
          
          #transpose the table
          rowMatch = line.match( /<\s*Cell[^>]*(row\s*=\s*)'([0-9]+)'/ )
          colMatch = line.match( /<\s*Cell[^>]*(col\s*=\s*)'([0-9]+)'/ )
          if (rowMatch != nil && colMatch != nil)
            curRow = rowMatch[2].to_i
            newRow = 1
            newCol = 9 - curRow
            line.gsub!(/row\s*=\s*'[0-9]+'/, "#{rowMatch[1]}'#{newRow}'")
            line.gsub!(/col\s*=\s*'[0-9]+'/, "#{colMatch[1]}'#{newCol}'")
          end
        end
      end
      
      # write lines back to the file
      File.open("#{XML_FILE}", 'w') do |f|
        linesArr.each { |element| f.puts(element) }
      end
      
      puts "  done"
     
    else
      puts "  Failed to find Av18 page table"  
    end
    file.close
  end
end

# ********************************
# finish
# ********************************
def finish
  puts "script done!"
end

# ********************************
# main 
# ********************************
if ARGV.length != 2
  print_usage
elsif 1 != MODE.to_i
  puts "Encountered unexpected mode: #{MODE}"
  puts ""
  print_usage
else
  duplicate_original_file
  
  if 1 == MODE.to_i
    transpose_av81_to_av18
  end
  
  finish
end
