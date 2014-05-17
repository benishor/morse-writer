add_subdirectory(utils)
add_subdirectory(writer)

include_directories(utils/headers)
include_directories(writer/headers)

add_library(morse ${SOURCE})
add_executable(morse-writer main.cpp)
target_link_libraries(morse-writer morse)
