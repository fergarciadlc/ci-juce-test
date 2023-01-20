# -*- encoding: utf-8 -*-
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "protocol_buffers/version"

Gem::Specification.new do |gem|
  gem.name          = "ruby-protocol-buffers"
  gem.version       = ProtocolBuffers::VERSION
  gem.authors       = ["Brian Palmer", "Benedikt Böhm", "Rob Marable", "Paulo Luis Franchini Casaretto"]
  gem.email         = ["brian@codekitchen.net", "bb@xnull.de"]
  gem.summary       = %{Ruby compiler and runtime for the google protocol buffers library.}
  gem.homepage      = "https://github.com/codekitchen/ruby-protocol-buffers"

  gem.files         = `git ls-files`.split($/)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.require_paths = ["lib"]

  gem.license       = 'BSD'

  gem.extra_rdoc_files << "Changelog.md"

  gem.add_development_dependency "autotest-standalone"
  gem.add_development_dependency "autotest-growl"
  gem.add_development_dependency "rake"
  gem.add_development_dependency "rake-compiler"
  gem.add_development_dependency "simplecov"
  gem.add_development_dependency "rspec", "~> 2.5"
  gem.add_development_dependency "yard"
end
