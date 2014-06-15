add_subdirectory(libmorse)
add_subdirectory(rtaudio)

include_directories(libmorse/headers)
include_directories(rtaudio)

# create morse library
add_library(morse ${SOURCE})

# create morse-writer binary
add_executable(morse-writer morse-writer.cpp)
target_link_libraries(morse-writer morse)

# create morse-player binary
add_executable(morse-player morse-player.cpp)
# When linking to the static rtaudio link we get a lot of undefined references
target_link_libraries(morse-player morse rtaudio)
