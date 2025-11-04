cmake_minimum_required(VERSION 3.15)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.
    Please create a subfolder and use `cmake ..` inside it.
    NOTE: cmake creates CMakeCache.txt and CMakeFiles/*.
          Remove them, or cmake will refuse to work.")
endif()

project(Utils C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wno-unused-parameter)

# utils_ds_objs
add_library(utils_ds_objs OBJECT
    src/blocking_queue.c
    src/deque.c
    src/hash_table.c
    src/rb_tree.c
    # src/state_machine.c
)
target_include_directories(utils_ds_objs 
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(utils_ds_objs
    PRIVATE
        $<$<CONFIG:Debug>: -O0 -g>
)
find_library(PTHREAD_LIB pthread REQUIRED)
if (PTHREAD_LIB)
    target_link_libraries(utils_ds_objs PUBLIC ${PTHREAD_LIB})
endif()

# utils_ds
add_library(utils_ds
    # $<TARGET_OBJECTS:utils_ds_objs>
)
target_link_libraries(utils_ds PUBLIC utils_ds_objs)
# find_library(PTHREAD_LIB pthread REQUIRED)
# if (PTHREAD_LIB)
#     target_link_libraries(utils_ds PUBLIC ${PTHREAD_LIB})
# endif()

install(DIRECTORY include/utils_ds DESTINATION include)
install(TARGETS utils_ds
        ARCHIVE DESTINATION lib)

# utils_objs
add_library(utils_objs OBJECT 
    src/my_err.c
    src/socket_helper.c
    src/state_machine.c
)
# target_link_libraries(utils_objs PUBLIC utils_ds_objs)
target_include_directories(utils_objs 
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(utils_objs
    PRIVATE
    $<$<CONFIG:Debug>: -O0 -g>
)

#debug
# add_library(objs STATIC $<TARGET_OBJECTS:utils_objs>)
# add_custom_target(print_objs ALL
#     COMMAND ${CMAKE_COMMAND} -E echo "Objects:\n$<JOIN:$<TARGET_OBJECTS:utils_objs>,\n>"
# )

# get_target_property(utils_objs_LIBRARY_SOURCES utils_objs SOURCES)
# message("Source files for utils_objs: ${utils_objs_LIBRARY_SOURCES}")
# message("Utils: PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

# install(DIRECTORY include/utils DESTINATION include)
