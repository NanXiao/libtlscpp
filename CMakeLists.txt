cmake_minimum_required(VERSION 3.5)
project (libtlscpp VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

set(HEADER_FILES 
    include/config.hxx
    include/context.hxx
    include/client.hxx 
    include/server.hxx
    include/exception.hxx
)

add_library(tlscpp SHARED 
    src/config.cxx
    src/context.cxx
    src/client.cxx
    src/server.cxx
    src/exception.cxx
)

target_include_directories(tlscpp PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(tlscpp PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 1
	PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS tlscpp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/libtlscpp
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libtlscpp)

add_subdirectory(examples)