
set(source  Source/additive.cpp)
set(binary  additive)
set(app     Additive)
set(version 0.0.0)

#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11.4)
include(Platform/build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

add_executable(${binary} ${source})
target_link_libraries(${binary} ${platform_libs})
