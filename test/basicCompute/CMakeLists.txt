file(GLOB basic "*.cpp")

add_executable(basicCompute ${header} ${src} ${basic})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_include_directories(basicCompute PUBLIC ${CMAKE_SOURCE_DIR}/src)
  target_link_directories(basicCompute PUBLIC "/usr/local/USD/lib")
  target_link_directories(basicCompute PUBLIC "/usr/local/USD/lib64")
  target_link_directories(basicCompute PUBLIC vulkan SDL2 work boost_atomic
    boost_system boost_python38 boost_regex __boost_thread boost_date_time
    boost_filesystem boost_program_options python3.8 vt kind arch ar garch
    gf glf hd hdSt hf hgiGL jpeg png tiff sdf usdGeom usd usdUI usdUtils
    usdRi usdVol usdRender usdShade usdImageGL usdImaging)
endif()

