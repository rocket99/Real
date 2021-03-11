cmake_minimum_required(VERSION 3.10)

project(Real)

file(GLOB header "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB src    "${CMAKE_SOURCE_DIR}/src/*.cpp")



if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  #INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/header/")
  INCLUDE_DIRECTORIES("/usr/include/")
  INCLUDE_DIRECTORIES("/usr/include/SDL2/")
  INCLUDE_DIRECTORIES("/usr/local/USD/include/")
  INCLUDE_DIRECTORIES("/usr/include/python3.8/")

endif()

add_subdirectory(test)
###
#add_executable(${PROJECT_NAME} ${header} ${src} ${test})
#if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#  target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/USD/lib")
#  target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/USD/lib64")
#endif()

#target_link_libraries(${PROJECT_NAME} PUBLIC vulkan SDL2 work boost_atomic
#  boost_system boost_python38 boost_regex boost_thread boost_date_time
#  boost_filesystem boost_program_options python3.8
#  vt kind arch ar garch gf glf hd hdSt hf work hgiGL jpeg png tiff sdf sdr
#  usdGeom usd usdUI usdUtils usdRi usdVol usdRender usdShade usdImagingGL
#  usdImaging)
###
