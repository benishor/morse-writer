include_directories(
	${morse-writer_SOURCE_DIR}/src/writer/headers
	UnitTest++/src
)

add_subdirectory(UnitTest++)
file(GLOB TEST_UNITS src/*.cpp)

add_executable(main main.cpp ${TEST_UNITS})
target_link_libraries(main morse UnitTestPP)