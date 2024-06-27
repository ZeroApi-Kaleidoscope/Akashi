project(tests LANGUAGES CXX C)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_testing(true)

set(SKIP_AUTOMOC ON)

function(declare_server_test partial_test_id)
  message(STATUS "Declaring server test ${partial_test_id}")

  set(test_id "test_server_${partial_test_id}")

  add_executable(${test_id} ${ARGN})

  target_compile_definitions(${test_id} PRIVATE KAL_AKASHI_TEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resource")
  target_link_libraries(${test_id} PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test akashi)

  if(WIN32)
    set_target_properties(${test_id} PROPERTIES WIN32_EXECUTABLE TRUE)
  endif()

  add_test(NAME ${test_id} COMMAND ${test_id})
endfunction()

declare_server_test(acl_roles_handler test_acl_roles_handler.cpp)
declare_server_test(akashi_utils test_akashi_utils.cpp)
declare_server_test(aopacket test_aopacket.cpp)
declare_server_test(area test_area.cpp)
declare_server_test(config_manager test_config_manager.cpp)
declare_server_test(crypto test_crypto.cpp)
declare_server_test(music_manager test_music_manager.cpp)
