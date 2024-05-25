set(SRCS lox.cpp file.cpp error.cpp scanner.cpp)

add_executable(lox ${SRCS})

target_include_directories(lox PUBLIC ${PROJECT_SOURCE_DIR}/include)