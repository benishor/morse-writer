add_subdirectory(utils)
add_subdirectory(writer)
add_subdirectory(rtaudio)

include_directories(utils/headers)
include_directories(writer/headers)
include_directories(rtaudio)

add_library(morse ${SOURCE})
add_executable(morse-writer main.cpp)
target_link_libraries(morse-writer morse)

add_executable(player player.cpp)
# When linking to the static rtaudio link we get a lot of undefined references
target_link_libraries(player morse rtaudio)
