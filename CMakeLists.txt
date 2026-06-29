cmake_minimum_required(VERSION 3.16)

project(ispd-exa-gui VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg PrintSupport)

set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

add_executable(ispd-exa-gui src/ispd-exa-gui.cpp

  # Windows / UI
  include/window/packcircles.h
  src/window/qcustomplot.cpp include/window/qcustomplot.h
  src/window/scatterplot.cpp include/window/scatterplot.h forms/scatterplot.ui
  src/window/settings.cpp include/window/settings.h forms/settingswindow.ui
  src/window/adduser.cpp include/window/adduser.h forms/adduser.ui
  src/window/drawingtable/view.cpp include/window/drawingtable/view.h
  src/window/drawingtable/scene.cpp include/window/drawingtable/scene.h
  src/window/drawingtable/drawingtable.cpp include/window/drawingtable/drawingtable.h
  src/window/main.cpp include/window/main.h forms/mainwindow.ui
  src/window/schemawindow.cpp include/window/schemawindow.h
  src/window/simulation.cpp include/window/simulation.h forms/simulation.ui
  src/window/users.cpp include/window/users.h forms/userwindow.ui
  src/window/addworkloadwindow.cpp include/window/addworkloadwindow.h forms/addworkloadwindow.ui

  # Component configuration windows
  src/window/machineconfigurationwindow.cpp include/window/machineconfigurationwindow.h forms/machineconfigurationwindow.ui
  src/window/linkconfigurationwindow.cpp include/window/linkconfigurationwindow.h forms/linkconfigurationwindow.ui
  src/window/machinesetconfigurationwindow.cpp include/window/machinesetconfigurationwindow.h forms/machinesetconfigurationwindow.ui
  src/window/switchconfigurationwindow.cpp include/window/switchconfigurationwindow.h forms/switchconfigurationwindow.ui
  src/window/schemaconfigurationwindow.cpp include/window/schemaconfigurationwindow.h forms/schemaconfigurationwindow.ui
  src/window/propertiespanel.cpp include/window/propertiespanel.h

  # Components
  src/components/machine.cpp include/components/machine.h
  src/components/switch.cpp include/components/switch.h
  src/components/machineset.cpp include/components/machineset.h
  src/components/link.cpp include/components/link.h
  src/components/schema.cpp include/components/schema.h


  # Icons
  include/icon/icon.h
  src/icon/pixmapicon.cpp include/icon/pixmapicon.h
  src/icon/linkicon.cpp include/icon/linkicon.h
  include/utils/iconSize.h
  include/utils/iconPath.h

  # Context
  include/context/context.h
  include/context/user.h
  include/context/workload.h
  src/workload.cpp
)

qt_add_resources(ispd-exa-gui imageresources
    PREFIX "/"
    FILES
        icons/add_icon.png
        icons/cluster.png icons/clusterSelected.png
        icons/pc.png icons/pcSelected.png
        icons/switch.svg icons/switchSelected.png
        icons/connectableset.png
        icons/connection.png
        icons/cursor.png
        icons/engine.png
        icons/ethernet.png
        icons/folder-new.svg
        icons/head.png
        icons/machine.png
        icons/perfil.png
        icons/save.png
        icons/torre.png
        icons/x_button.png
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(ispd-exa-gui PRIVATE -g)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(ispd-exa-gui PRIVATE -O3)
endif()

target_link_libraries(ispd-exa-gui PRIVATE Qt6::Widgets Qt6::PrintSupport Qt6::Svg nlohmann_json::nlohmann_json)
target_include_directories(ispd-exa-gui PUBLIC include)

set_target_properties(ispd-exa-gui PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
