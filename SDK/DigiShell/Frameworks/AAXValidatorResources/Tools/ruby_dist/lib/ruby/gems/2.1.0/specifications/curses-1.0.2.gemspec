# -*- encoding: utf-8 -*-
# stub: curses 1.0.2 ruby lib
# stub: ext/curses/extconf.rb

Gem::Specification.new do |s|
  s.name = "curses"
  s.version = "1.0.2"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Eric Hodel", "Shugo Maeda"]
  s.cert_chain = ["-----BEGIN CERTIFICATE-----\nMIIDNjCCAh6gAwIBAgIBAzANBgkqhkiG9w0BAQUFADBBMRAwDgYDVQQDDAdkcmJy\nYWluMRgwFgYKCZImiZPyLGQBGRYIc2VnbWVudDcxEzARBgoJkiaJk/IsZAEZFgNu\nZXQwHhcNMTUwODA1MjE1NzQ5WhcNMTYwODA0MjE1NzQ5WjBBMRAwDgYDVQQDDAdk\ncmJyYWluMRgwFgYKCZImiZPyLGQBGRYIc2VnbWVudDcxEzARBgoJkiaJk/IsZAEZ\nFgNuZXQwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCbbgLrGLGIDE76\nLV/cvxdEzCuYuS3oG9PrSZnuDweySUfdp/so0cDq+j8bqy6OzZSw07gdjwFMSd6J\nU5ddZCVywn5nnAQ+Ui7jMW54CYt5/H6f2US6U0hQOjJR6cpfiymgxGdfyTiVcvTm\nGj/okWrQl0NjYOYBpDi+9PPmaH2RmLJu0dB/NylsDnW5j6yN1BEI8MfJRR+HRKZY\nmUtgzBwF1V4KIZQ8EuL6I/nHVu07i6IkrpAgxpXUfdJQJi0oZAqXurAV3yTxkFwd\ng62YrrW26mDe+pZBzR6bpLE+PmXCzz7UxUq3AE0gPHbiMXie3EFE0oxnsU3lIduh\nsCANiQ8BAgMBAAGjOTA3MAkGA1UdEwQCMAAwCwYDVR0PBAQDAgSwMB0GA1UdDgQW\nBBS5k4Z75VSpdM0AclG2UvzFA/VW5DANBgkqhkiG9w0BAQUFAAOCAQEAXmQgUF6x\nM/aBwZ0uaXzquQHbgAzH3bXlGIa3DrxCiSRAZ1xwYX4gRMV9iQ2kVDwafz5sg9/p\nN/cMBNOjYRTX2XGv49PpaIiO/JRL/SLDdws/4tkIJOynFqpwcLNB66XeciXxh6nH\nMS+S1O749pcMIFIBpMXvRS5mG7MPY3NhnmzcIQNYvXAcSXv675Cg4h1c6pxiqeAS\nu3L+nuwJKNyOIWL3B5ek/yypK55RFUSwReqZTZQkVhNF4xlqPYGDsAxIE/xqadfP\n+paXWY9JmO3d13yH2/nhDPAo/y7KwmEETFvtUoBmX4o44n/ELqSgb9NrL+613z9j\nn8mbTOXmLGlTBA==\n-----END CERTIFICATE-----\n"]
  s.date = "2016-03-16"
  s.description = "A Ruby binding for curses, ncurses, and PDCurses.\ncurses is an extension library for text UI applications.\n\nFormerly part of the Ruby standard library, [curses was removed and placed in this gem][1]\nwith the release of Ruby 2.1.0. (see [ruby/ruby@9c5b2fd][2])"
  s.email = ["drbrain@segment7.net", ""]
  s.extensions = ["ext/curses/extconf.rb"]
  s.extra_rdoc_files = ["History.md", "Manifest.txt", "README.md", "ext/curses/curses.c"]
  s.files = ["History.md", "Manifest.txt", "README.md", "ext/curses/curses.c", "ext/curses/extconf.rb"]
  s.homepage = "https://github.com/ruby/curses"
  s.licenses = ["Ruby", "BSD-2-Clause"]
  s.rdoc_options = ["--main", "README.md"]
  s.rubygems_version = "2.2.5"
  s.summary = "A Ruby binding for curses, ncurses, and PDCurses"

  s.installed_by_version = "2.2.5" if s.respond_to? :installed_by_version

  if s.respond_to? :specification_version then
    s.specification_version = 4

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<minitest>, ["~> 5.8"])
      s.add_development_dependency(%q<rdoc>, ["~> 4.0"])
      s.add_development_dependency(%q<rake-compiler>, ["~> 0.8"])
      s.add_development_dependency(%q<hoe>, ["~> 3.14"])
    else
      s.add_dependency(%q<minitest>, ["~> 5.8"])
      s.add_dependency(%q<rdoc>, ["~> 4.0"])
      s.add_dependency(%q<rake-compiler>, ["~> 0.8"])
      s.add_dependency(%q<hoe>, ["~> 3.14"])
    end
  else
    s.add_dependency(%q<minitest>, ["~> 5.8"])
    s.add_dependency(%q<rdoc>, ["~> 4.0"])
    s.add_dependency(%q<rake-compiler>, ["~> 0.8"])
    s.add_dependency(%q<hoe>, ["~> 3.14"])
  end
end
