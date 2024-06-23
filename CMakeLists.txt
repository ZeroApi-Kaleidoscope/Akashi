cmake_minimum_required(VERSION 3.7.0)

project("akashi" VERSION 1.8.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(KAL_AKASHI_BUILD_TESTS "Build tests" ON)
option(KAL_AKASHI_DEBUG_NETWORK "Enable network debug" OFF)

if(NOT KAL_BUILD_OUTPUT)
set(KAL_BUILD_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network WebSockets Sql REQUIRED)

if (Qt5Core_FOUND)
  if (Qt5Core_VERSION VERSION_LESS 5.15.0)
    message(FATAL_ERROR "Minimum supported Qt5 version is 5.15.0!")
  endif()
endif()

add_library(akashi STATIC
  src/acl_roles_handler.cpp
  src/acl_roles_handler.h
  src/advertiser.cpp
  src/advertiser.h
  src/akashidefs.h
  src/akashiutils.h
  src/aoclient.cpp
  src/aoclient.h
  src/area_data.cpp
  src/area_data.h
  src/command_extension.cpp
  src/command_extension.h
  src/commands/area.cpp
  src/commands/authentication.cpp
  src/commands/casing.cpp
  src/commands/command_helper.cpp
  src/commands/messaging.cpp
  src/commands/moderation.cpp
  src/commands/music.cpp
  src/commands/roleplay.cpp
  src/config_manager.cpp
  src/config_manager.h
  src/data_types.h
  src/db_manager.cpp
  src/db_manager.h
  src/discord.cpp
  src/discord.h
  src/logger/u_logger.cpp
  src/logger/u_logger.h
  src/logger/writer_full.cpp
  src/logger/writer_full.h
  src/logger/writer_modcall.cpp
  src/logger/writer_modcall.h
  src/music_manager.cpp
  src/music_manager.h
  src/network/aopacket.cpp
  src/network/aopacket.h
  src/network/network_socket.cpp
  src/network/network_socket.h
  src/packet/packet_askchaa.cpp
  src/packet/packet_askchaa.h
  src/packet/packet_casea.cpp
  src/packet/packet_casea.h
  src/packet/packet_cc.cpp
  src/packet/packet_cc.h
  src/packet/packet_ch.cpp
  src/packet/packet_ch.h
  src/packet/packet_ct.cpp
  src/packet/packet_ct.h
  src/packet/packet_de.cpp
  src/packet/packet_de.h
  src/packet/packet_ee.cpp
  src/packet/packet_ee.h
  src/packet/packet_factory.cpp
  src/packet/packet_factory.h
  src/packet/packet_generic.cpp
  src/packet/packet_generic.h
  src/packet/packet_hi.cpp
  src/packet/packet_hi.h
  src/packet/packet_hp.cpp
  src/packet/packet_hp.h
  src/packet/packet_id.cpp
  src/packet/packet_id.h
  src/packet/packet_info.h
  src/packet/packet_mc.cpp
  src/packet/packet_mc.h
  src/packet/packet_ms.cpp
  src/packet/packet_ms.h
  src/packet/packet_pe.cpp
  src/packet/packet_pe.h
  src/packet/packet_pl.cpp
  src/packet/packet_pl.h
  src/packet/packet_pw.cpp
  src/packet/packet_pw.h
  src/packet/packet_rc.cpp
  src/packet/packet_rc.h
  src/packet/packet_rd.cpp
  src/packet/packet_rd.h
  src/packet/packet_rm.cpp
  src/packet/packet_rm.h
  src/packet/packet_rt.cpp
  src/packet/packet_rt.h
  src/packet/packet_setcase.cpp
  src/packet/packet_setcase.h
  src/packet/packet_zz.cpp
  src/packet/packet_zz.h
  src/packets.cpp
  src/playerstateobserver.cpp
  src/playerstateobserver.h
  src/server.cpp
  src/server.h
  src/testimony_recorder.cpp
  src/typedefs.h
)

target_compile_definitions(akashi PUBLIC QT_NO_KEYWORDS QT_MESSAGELOGCONTEXT)
target_include_directories(akashi PUBLIC src)
target_link_libraries(akashi PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::WebSockets
  Qt${QT_VERSION_MAJOR}::Sql
)

if(KAL_AKASHI_DEBUG_NETWORK)
target_compile_definitions(akashi PUBLIC KAL_AKASHI_DEBUG_NETWORK)
endif()

add_executable(akashi-runtime
  src/main.cpp
)

target_include_directories(akashi-runtime PUBLIC src)
target_link_libraries(akashi-runtime PRIVATE akashi)

set_target_properties(akashi-runtime PROPERTIES RUNTIME_OUTPUT_NAME "akashi")
set_target_properties(akashi-runtime PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${KAL_BUILD_OUTPUT}")

if(WIN32)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET akashi-runtime PROPERTY WIN32_EXECUTABLE true)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resource/akashi.rc")
    target_sources(akashi-runtime PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
  endif()
endif()

if(KAL_AKASHI_BUILD_TESTS)
  add_subdirectory(tests)
endif()
