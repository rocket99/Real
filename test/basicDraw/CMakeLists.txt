
file(GLOB test  "${CMAKE_SOURCE_DIR}/test/basicDraw/*.cpp")

add_executable(basic ${header} ${src} ${test})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_include_directories(basic PUBLIC "${CMAKE_SOURCE_DIR}/src/") 
  target_link_directories(basic PUBLIC "/usr/local/USD/lib")
  target_link_directories(basic PUBLIC "/usr/local/USD/lib64")
  target_link_libraries(basic PUBLIC vulkan SDL2 work boost_atomic
    boost_system boost_python38 boost_regex boost_thread boost_date_time
    boost_filesystem boost_program_options python3.8 vt kind arch ar garch
    gf glf hd hdSt hf hgiGL jpeg png tiff sdf sdr usdGeom usd usdUI usdUtils
    usdRi usdVol usdRender usdShade usdImagingGL usdImaging)
endif()

