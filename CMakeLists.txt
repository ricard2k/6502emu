cmake_minimum_required(VERSION 3.5.0)
project(6502emu VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_compile_options(-W -Wall -Werror) #All Warnings, all warnings are errors

add_executable(6502emu main.cpp 6502emu.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()

add_executable(6502emuTest 6502emuTest.cpp 6502emu.cpp)

target_link_libraries(6502emuTest gtest_main)

include(GoogleTest)
gtest_discover_tests(6502emuTest)
