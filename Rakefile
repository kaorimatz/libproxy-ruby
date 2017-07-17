require 'bundler/gem_tasks'
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec)

require 'rake/extensiontask'

task build: :compile

Rake::ExtensionTask.new('libproxy_ext') do |ext|
  ext.ext_dir = 'ext/libproxy'
  ext.lib_dir = 'lib/libproxy'
end

task default: %i[clobber compile spec]
