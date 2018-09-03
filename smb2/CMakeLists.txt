cmake_minimum_required(VERSION 2.6)
project(proxyserver)

find_package(Threads REQUIRED)

add_executable(proxyserver smb2Name.c smb2tools.c short_name_list.c proxyserver1.c smb_session_list.c unicode2utf8.c fname_decode.c)

if(THREADS_HAVE_PTHREAD_ARG)
  set_property(TARGET proxyserver PROPERTY COMPILE_OPTIONS "-pthread")
  set_property(TARGET proxyserver PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(proxyserver "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install(TARGETS proxyserver RUNTIME DESTINATION bin)
