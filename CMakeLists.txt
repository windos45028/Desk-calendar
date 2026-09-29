# 1. 指定 CMake 最低版本與 C++ 標準
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)

# 2. 設定專案名稱
project(calendar)

# 3. 尋找第三方庫 (相當於拉取依賴)
find_package(raylib REQUIRED)

# 1. 指定 macOS App 圖示檔名
set(MACOSX_BUNDLE_ICON_FILE app_icon.icns)
set(APP_ICON_RESOURCE assets/app_icon.icns)

# 2. 宣告這個 .icns 要被放進 .app 套件裡面的 Resources 資料夾
set_source_files_properties(${APP_ICON_RESOURCE} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)

# 3. 把原始碼與 .icns 檔一起傳給 add_executable
add_executable(calendar MACOSX_BUNDLE 
    src/main.cpp 
    ${APP_ICON_RESOURCE}
)

# 5. 將第三方庫跟 .exe 綁定在一起 (Link)
target_link_libraries(calendar PRIVATE raylib)

add_custom_command(TARGET calendar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:calendar>/assets
)
