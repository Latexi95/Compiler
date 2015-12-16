file(GLOB SRC "*.h" "*.cpp")

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-DBOOST_NO_AUTO_PTR)
find_package(Boost 1.59.0 REQUIRED COMPONENTS serialization system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
include_directories("../.")

add_executable(compiler ${SRC})
target_link_libraries(compiler ${Boost_FILESYSTEM_LIBRARY} ${Boost_SERIALIZATION_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(compiler common compiler_lib)
target_compile_features(compiler PRIVATE cxx_range_for)
