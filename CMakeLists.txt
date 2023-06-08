cmake_minimum_required(VERSION 3.15)

include(ExternalProject)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
add_definitions(-DNAPI_VERSION=4)

project (node-libjq)

file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES
  PREFIX ""
  SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

execute_process(COMMAND node -p "require('node-addon-api').include.replace(/['\"]/g, '')"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NAPI_INC
  OUTPUT_STRIP_TRAILING_WHITESPACE)
target_include_directories(${PROJECT_NAME} PRIVATE ${NAPI_INC})

ExternalProject_Add(jq
  URL               https://github.com/jqlang/jq/releases/download/jq-1.6/jq-1.6.zip
  URL_HASH          SHA1=2a49855ef4997fcebd10487b06d16fe34c2abae0
  DOWNLOAD_EXTRACT_TIMESTAMP 1

  CONFIGURE_COMMAND autoreconf -fi
            COMMAND ./configure --with-oniguruma=builtin --disable-maintainer-mode --prefix=<INSTALL_DIR>
  BUILD_COMMAND     make -j8
  INSTALL_COMMAND   make install
  BUILD_IN_SOURCE   1
)
add_dependencies(${PROJECT_NAME} jq)
ExternalProject_Get_Property(jq INSTALL_DIR)
set(jq_PREFIX ${INSTALL_DIR})
file(MAKE_DIRECTORY ${jq_PREFIX}/include)

add_library(libjq SHARED IMPORTED)
set_target_properties(libjq PROPERTIES
  IMPORTED_LOCATION ${jq_PREFIX}/lib/libjq.so
  INTERFACE_INCLUDE_DIRECTORIES ${jq_PREFIX}/include)
target_link_libraries(${PROJECT_NAME} libjq)

add_library(libonig SHARED IMPORTED)
set_target_properties(libonig PROPERTIES
  IMPORTED_LOCATION ${jq_PREFIX}/lib/libonig.so
  INTERFACE_INCLUDE_DIRECTORIES ${jq_PREFIX}/include)
target_link_libraries(${PROJECT_NAME} libonig)

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()