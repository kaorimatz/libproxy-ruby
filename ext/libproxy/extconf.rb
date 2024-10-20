require 'mkmf'

pkg_config('libproxy-1.0')
exit 1 unless have_library('proxy')

create_makefile('libproxy/libproxy_ext')
