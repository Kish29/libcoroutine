cmake_minimum_required(VERSION 3.19)
project(libcoroutine)

set(CMAKE_CXX_STANDARD 11)

# 动态链接库不能只包含headers
# 否则链接器不会乐意工作
set(
        SOURCES
        coroutine.cpp
        coroutine_lambda.h
)

# 编译成动态链接库
add_library(coroutine_static STATIC ${SOURCES})
add_library(coroutine_shared SHARED ${SOURCES})

# 添加可执行的测试文件
macro(add_example_target EXAMPLE_TARGET)
    add_executable(
            "example_${EXAMPLE_TARGET}"
            "example_${EXAMPLE_TARGET}.cpp"
    )
    # 添加协程动态链接库
    target_link_libraries(
            "example_${EXAMPLE_TARGET}"
            coroutine_static
            pthread
    )
endmacro(add_example_target)

add_example_target(colambda)
add_example_target(thread)
