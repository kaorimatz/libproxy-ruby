require 'mkmf'

exit 1 unless have_library('proxy')

create_makefile('libproxy/libproxy_ext')
