cmake_minimum_required(VERSION 3.25)

include(ExternalProject)
include(FetchContent)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

set(CMAKE_CXX_STANDARD 17)
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

if(APPLE)
  set(jq_CPPFLAGS "-D_REENTRANT")
endif()

ExternalProject_Add(jq
  URL https://github.com/jqlang/jq/releases/download/jq-1.6/jq-1.6.zip
  URL_HASH SHA1=2a49855ef4997fcebd10487b06d16fe34c2abae0
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE

  CONFIGURE_COMMAND autoreconf -fi
            COMMAND ${CMAKE_COMMAND} -E env CPPFLAGS=${jq_CPPFLAGS} ./configure
                      --with-oniguruma=builtin
                      --disable-maintainer-mode
                      --prefix=<INSTALL_DIR>
  BUILD_COMMAND make -j8
  INSTALL_COMMAND make install
  BUILD_IN_SOURCE TRUE
)
add_dependencies(${PROJECT_NAME} jq)
ExternalProject_Get_Property(jq INSTALL_DIR)
set(jq_PREFIX ${INSTALL_DIR})
file(MAKE_DIRECTORY ${jq_PREFIX}/include)

add_library(jq::libjq STATIC IMPORTED)
set_target_properties(jq::libjq PROPERTIES
  IMPORTED_LOCATION ${jq_PREFIX}/lib/libjq${CMAKE_STATIC_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${jq_PREFIX}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE jq::libjq)

add_library(jq::libonig STATIC IMPORTED)
set_target_properties(jq::libonig PROPERTIES
  IMPORTED_LOCATION ${jq_PREFIX}/lib/libonig${CMAKE_STATIC_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${jq_PREFIX}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE jq::libonig)

FetchContent_Declare(Boost
  URL https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
  URL_HASH SHA256=66a469b6e608a51f8347236f4912e27dc5c60c60d7d53ae9bfe4683316c6f04c
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_GetProperties(Boost)
if(NOT Boost_POPULATED)
  FetchContent_Populate(Boost)
  set(BOOST_ROOT ${boost_SOURCE_DIR})
  find_package(Boost 1.82 REQUIRED)
endif()
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()
