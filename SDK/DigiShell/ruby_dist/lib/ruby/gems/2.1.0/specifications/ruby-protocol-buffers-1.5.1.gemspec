# -*- encoding: utf-8 -*-
# stub: ruby-protocol-buffers 1.5.1 ruby lib

Gem::Specification.new do |s|
  s.name = "ruby-protocol-buffers"
  s.version = "1.5.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Brian Palmer", "Benedikt B\u{f6}hm", "Rob Marable", "Paulo Luis Franchini Casaretto"]
  s.date = "2013-10-28"
  s.email = ["brian@codekitchen.net", "bb@xnull.de"]
  s.executables = ["protoc-gen-ruby", "ruby-protoc"]
  s.extra_rdoc_files = ["Changelog.md"]
  s.files = ["Changelog.md", "bin/protoc-gen-ruby", "bin/ruby-protoc"]
  s.homepage = "https://github.com/codekitchen/ruby-protocol-buffers"
  s.licenses = ["BSD"]
  s.rubygems_version = "2.2.2"
  s.summary = "Ruby compiler and runtime for the google protocol buffers library."

  s.installed_by_version = "2.2.2" if s.respond_to? :installed_by_version

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<autotest-standalone>, [">= 0"])
      s.add_development_dependency(%q<autotest-growl>, [">= 0"])
      s.add_development_dependency(%q<rake>, [">= 0"])
      s.add_development_dependency(%q<rake-compiler>, [">= 0"])
      s.add_development_dependency(%q<simplecov>, [">= 0"])
      s.add_development_dependency(%q<rspec>, ["~> 2.5"])
      s.add_development_dependency(%q<yard>, [">= 0"])
    else
      s.add_dependency(%q<autotest-standalone>, [">= 0"])
      s.add_dependency(%q<autotest-growl>, [">= 0"])
      s.add_dependency(%q<rake>, [">= 0"])
      s.add_dependency(%q<rake-compiler>, [">= 0"])
      s.add_dependency(%q<simplecov>, [">= 0"])
      s.add_dependency(%q<rspec>, ["~> 2.5"])
      s.add_dependency(%q<yard>, [">= 0"])
    end
  else
    s.add_dependency(%q<autotest-standalone>, [">= 0"])
    s.add_dependency(%q<autotest-growl>, [">= 0"])
    s.add_dependency(%q<rake>, [">= 0"])
    s.add_dependency(%q<rake-compiler>, [">= 0"])
    s.add_dependency(%q<simplecov>, [">= 0"])
    s.add_dependency(%q<rspec>, ["~> 2.5"])
    s.add_dependency(%q<yard>, [">= 0"])
  end
end
