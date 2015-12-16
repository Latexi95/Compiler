
file(GLOB SRC "*.h" "*.cpp")

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-DBOOST_NO_AUTO_PTR)
find_package(Boost 1.59.0 REQUIRED COMPONENTS serialization system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_library(test_support ${SRC})
target_link_libraries(test_support ${Boost_FILESYSTEM_LIBRARY} ${Boost_SERIALIZATION_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(test_support common compiler_lib)
target_compile_features(test_support PRIVATE cxx_range_for)
