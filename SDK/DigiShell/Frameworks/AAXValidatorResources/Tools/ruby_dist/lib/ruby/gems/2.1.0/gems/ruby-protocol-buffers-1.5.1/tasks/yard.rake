require 'yard'

YARD::Rake::YardocTask.new(:doc) do |t|
  version = ProtocolBuffers::VERSION
  t.options = ["--title", "ruby protocol buffers #{version}", "--files", "LICENSE,Changelog.md"]
end
