

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shader)
configure_file(${CMAKE_SOURCE_DIR}/src/TKUtility.cpp.in
  ${CMAKE_SOURCE_DIR}/src/TKUtility.cpp @ONLY)

file(GLOB test   "${CMAKE_SOURCE_DIR}/test/*.cpp")

add_executable(basic ${header} ${src} ${test})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_directories(basic PUBLIC "/usr/local/USD/lib")
  target_link_directories(basic PUBLIC "/usr/local/USD/lib64")
  target_link_libraries(basic PUBLIC vulkan SDL2 work boost_atomic
    boost_system boost_python38 boost_regex boost_thread boost_date_time
    boost_filesystem boost_program_options python3.8 vt kind arch ar garch gf
    glf hd hdSt hf work hgiGL jpeg png tiff sdf sdr usdGeom usd usdUI usdUtils
    usdRi usdVol usdRender usdShade usdImagingGL usdImaging)
endif()
