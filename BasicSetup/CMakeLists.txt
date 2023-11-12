set(APP_NAME "ImageViewerX")

file(GLOB_RECURSE TARGET_SRC "src/*.cpp")
file(GLOB_RECURSE TARGET_H "include/*.h")

#this is used to add and access QML that are part of Resources (.qrc) in cpp
file(GLOB_RECURSE TARGET_QML "resource/qml.qrc")
qt_add_resources(QML_RESOURCES ${TARGET_QML})

#this is used to add and access ICONS that are part of Resources (.qrc) in cpp
file(GLOB_RECURSE TARGET_ICON "resource/icon.qrc")
qt_add_resources(ICON_RESOURCES ${TARGET_ICON})

add_executable(${APP_NAME} ${TARGET_SRC} ${TARGET_H} ${QML_RESOURCES} ${ICON_RESOURCES})

target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${APP_NAME} PRIVATE Qt6::Quick Qt6::Widgets ${OpenCV_LIBS})
