cmake_minimum_required(VERSION 3.8)
project(robo_op_sys_clt_lib C CXX)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  add_compile_options(
      -Wno-unused-result
      -Wno-return-type
      -Wno-deprecated-declarations
      -Wno-discarded-qualifiers
      -Wno-misleading-indentation
  )
endif()

find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosidl_runtime_c REQUIRED)

include_directories(include)

add_executable(node
  main.c

  src/ros.c
  src/node.c
  src/error.c

  src/pub.c
  src/sub.c

  src/server.c
  src/client.c

  src/spin.c
  src/iface.c
  src/cjson.c
)

ament_target_dependencies(node
  rcl
  std_msgs
  std_srvs
  rosidl_runtime_c
)

install(TARGETS node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
