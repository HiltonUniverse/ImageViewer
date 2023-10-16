set(APP_NAME "BasicSetupApp")

file(GLOB_RECURSE TARGET_SRC "src/*.cpp")
file(GLOB_RECURSE TARGET_H "include/*.h")

add_executable(${APP_NAME} ${TARGET_SRC} ${TARGET_H})
target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

qt_add_qml_module(${APP_NAME}
    URI Basic
    VERSION 1.0
    QML_FILES
        src/main.qml
)

target_link_libraries(${APP_NAME} PRIVATE Qt6::Quick Qt6::Widgets)

