cmake_minimum_required(VERSION 3.8)
project(rcl_utils C)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)

add_executable(node_impl 
  src/main.c
  src/ros.c   
  src/node.c
  src/error.c
)

ament_target_dependencies(node_impl
  rcl
)

install(TARGETS node_impl
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()