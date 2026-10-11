cmake_minimum_required(VERSION 3.16)

project(japanese-era-converter
    VERSION 45.0.0
    DESCRIPTION "KRunner plugin to convert Gregorian years to Japanese era names"
    LANGUAGES CXX
)

set(QT_MIN_VERSION 6.5.0)
set(KF_MIN_VERSION 6.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Extra CMake Modules ----
find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

# ---- Qt6 ----
find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

# ---- KDE Frameworks 6 ----
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Runner
    I18n
    CoreAddons
)

# ---- KRunner 插件 ----
# 使用 kcoreaddons_add_plugin 确保安装到 kf6/krunner/ 目录，
# KRunner 只从该目录加载插件。
kcoreaddons_add_plugin(japaneseera
    SOURCES
        src/japaneseera.cpp
        src/japaneserunner.cpp
    INSTALL_NAMESPACE "kf6/krunner"
)

target_include_directories(japaneseera PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(japaneseera
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        KF6::Runner
        KF6::I18n
        KF6::CoreAddons
)

# ---- 翻译文件 ----
ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
