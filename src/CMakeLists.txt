add_subdirectory(utils)
add_subdirectory(writer)
add_subdirectory(rtaudio)

include_directories(utils/headers)
include_directories(writer/headers)
include_directories(rtaudio)

# create morse library
add_library(morse ${SOURCE})

# create morse-writer binary
add_executable(morse-writer main.cpp)
target_link_libraries(morse-writer morse)

# create morse-player binary
add_executable(morse-player player.cpp)
# When linking to the static rtaudio link we get a lot of undefined references
target_link_libraries(morse-player morse rtaudio)
