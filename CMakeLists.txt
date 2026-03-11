cmake_minimum_required(VERSION 3.8)
project(rcl_utils C CXX)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wunused-result)
endif()

find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosidl_runtime_c REQUIRED)

include_directories(include)

add_executable(node_impl
  main.c
  src/ros.c
  src/node.c
  src/error.c
  src/pub.c
  src/sub.c
  src/spin.c
  src/server.c
  src/client.c
  src/iface.c
)

ament_target_dependencies(node_impl
  rcl
  std_msgs
  std_srvs
  rosidl_runtime_c
)

install(TARGETS node_impl
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
