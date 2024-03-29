cmake_minimum_required(VERSION 3.10)

project(get_start)

file(GLOB PROJECT_HDR ./*.h)
file(GLOB PROJECT_SRC ./*.cpp)

set(BASE_HDR ../base/gl_utility.h
             ../base/application.h
             ../base/frame_rate_indicator.h
             ../base/input.h
             ../base/glsl_program.h)

set(BASE_SRC ../base/application.cpp ../base/glsl_program.cpp)
set(IMGGUI_HDR img/*.h)
set(IMGGUI_SRC img/*.s)


add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_HDR} ${BASE_SRC} ${BASE_HDR} ${IMGGUI_HDR} ${IMGGUI_SRC})

source_group("Header Files" FILES ${BASE_HDR} ${PROJECT_HDR} ${IMGGUI_HDR})
source_group("Source Files" FILES ${BASE_SRC} ${PROJECT_SRC} ${IMGGUI_SRC})

configure_project(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)