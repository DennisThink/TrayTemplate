cmake_minimum_required(VERSION 3.0)
project(package_indicator)
include_directories("/usr/include/gtk-3.0/") 
include_directories("/usr/include/glib-2.0/")
include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include/")
include_directories("/usr/include/pango-1.0/")
include_directories("/usr/include/harfbuzz/")
include_directories("/usr/include/cairo/")
include_directories("/usr/include/gdk-pixbuf-2.0/")
include_directories("/usr/include/atk-1.0/")
include_directories("/usr/include/libappindicator-0.1/")


link_directories("/usr/lib/x86_64-linux-gnu/")
link_libraries(pthread)
link_libraries(gtk-3)
link_libraries(gobject-2.0)
link_libraries(appindicator)

SET(UPDATE_INDICATOR ./example.c)
add_executable(package-update-indicator ${UPDATE_INDICATOR})

