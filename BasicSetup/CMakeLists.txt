set(APP_NAME "ImageViewerX")

file(GLOB_RECURSE TARGET_SRC "src/*.cpp")
file(GLOB_RECURSE TARGET_H "include/*.h")

#this is used to add and access QML that are part of Resources (.qrc) in cpp
file(GLOB_RECURSE TARGET_QML "resource/qml.qrc")
qt_add_resources(QML_RESOURCES ${TARGET_QML})

#this is used to add and access images/icons that are part of Resources (.qrc) in cpp
file(GLOB_RECURSE TARGET_ICON "resource/icon.qrc")
qt_add_resources(ICON_RESOURCES ${TARGET_ICON})

#SETTING APP ICON
#file name
set(MACOSX_BUNDLE_ICON_FILE photosurface.icns)
#file location
set(APP_ICON_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/resource/app_icon/photosurface.icns")
#defines the icon file's install location
set_source_files_properties(${APP_ICON_MACOS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

#CREATE EXECUTABLE BUNDLE WITH GIVEN APP ICON AND RESOURCES
add_executable(${APP_NAME} MACOSX_BUNDLE ${TARGET_SRC} ${TARGET_H} ${QML_RESOURCES} ${ICON_RESOURCES} ${APP_ICON_MACOS})

target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${APP_NAME} PRIVATE Qt6::Quick Qt6::Widgets ${OpenCV_LIBS})
