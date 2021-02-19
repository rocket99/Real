cmake_minimum_required(VERSION 3.10)
project(vk_proj)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

#execute_process(COMMAND cp -rf ./shader ./build/ .)

find_path(PYTHON_DIR REQUIRED)
file(GLOB cpp_src "*.cpp")
file(GLOB header "*.h")
add_compile_options("-std=c++14")
add_definitions("-Wall -g")

include_directories("/usr/include/")
include_directories("/usr/include/SDL2/")
include_directories("/usr/include/python3.8/")
include_directories("${VULKAN_SDK}/include/")
include_directories("/usr/local/USD/include/")
add_executable(main ${cpp_src} ${header})

#target_include_directories(main PUBLIC /usr/include/)
#target_include_directories(main PUBLIC ${VULKAN_SDK}/include)
#target_include_directories(main PUBLIC /usr/local/USD/include)
#target_include_directories(main PUBLIC /usr/include/python3.8/)

target_link_directories(main PUBLIC /usr/local/USD/lib)
target_link_directories(main PUBLIC /usr/local/USD/lib64)


target_link_libraries(main PUBLIC
  vulkan SDL2 work boost_atomic boost_system boost_python38
  boost_regex boost_thread boost_date_time boost_filesystem
  boost_program_options python3.8 vt kind arch ar garch gf glf 
  hd hdSt hf vt work hgiGL jpeg png tiff sdf sdr usdGeom usd
  usdUI usdUtils usdRi usdVol usdRender  usdShade usdImagingGL
  usdImaging) 


