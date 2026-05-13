cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets Core)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header -Wno-comma>
  )
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON
  )
endif()

target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
    src/plugin-main.c
    src/zondel-filter.c
    src/zondel-state.c
    src/zondel-launch.c
    src/ring-buffer.c
    src/resampler.c
    src/downmix.c
    src/pipe-client.c
)

if(WIN32)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE advapi32 shell32)
endif()

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})

# --- Tests (windows-x64 CI only; not part of the plugin DLL) ---
option(BUILD_PLUGIN_TESTS "Build all unit-test executables" OFF)

if(BUILD_PLUGIN_TESTS)
  enable_testing()

  add_executable(fake-pipe-server test/fake-pipe-server.c)
  if(WIN32)
    target_link_libraries(fake-pipe-server PRIVATE advapi32)
  endif()

  add_executable(test-pipe-client test/test-pipe-client.c src/pipe-client.c)
  if(WIN32)
    target_link_libraries(test-pipe-client PRIVATE advapi32)
  endif()
  add_test(NAME pipe-client COMMAND test-pipe-client)

  add_executable(test-zondel-state test/test-zondel-state.c src/zondel-state.c)
  add_test(NAME zondel-state COMMAND test-zondel-state)

  add_executable(test-ring-buffer test/test-ring-buffer.c src/ring-buffer.c)
  add_test(NAME ring-buffer COMMAND test-ring-buffer)

  add_executable(test-resampler test/test-resampler.c src/resampler.c)
  if(WIN32)
    target_link_libraries(test-resampler PRIVATE)
  else()
    target_link_libraries(test-resampler PRIVATE m)   # libm for sinf/cosf on Unix
  endif()
  add_test(NAME resampler COMMAND test-resampler)
endif()
