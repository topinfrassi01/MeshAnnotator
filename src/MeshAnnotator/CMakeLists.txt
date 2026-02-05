find_package(VTK COMPONENTS
  GUISupportQt
  RenderingQt
  IOPLY
  REQUIRED)

if(NOT DEFINED VTK_QT_VERSION)
  set(VTK_QT_VERSION 6)
endif()

set(qt_components Core Gui Widgets)
if(${VTK_QT_VERSION} VERSION_GREATER_EQUAL 6)
  list(APPEND qt_components OpenGLWidgets)
endif()

list(SORT qt_components)
# We have ui files, so this will also bring in the macro:
#   qt5_wrap_ui or qt_wrap_ui from Widgets.
find_package(Qt${VTK_QT_VERSION} REQUIRED COMPONENTS ${qt_components})

foreach(_qt_comp IN LISTS qt_components)
  list(APPEND qt_modules "Qt${VTK_QT_VERSION}::${_qt_comp}")
endforeach()

message (STATUS "VTK_VERSION: ${VTK_VERSION}, Qt Version: ${Qt${VTK_QT_VERSION}Widgets_VERSION}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT VTK_FOUND)
  message(FATAL_ERROR "MinimalQtVTKApp: Unable to find the VTK build folder.")
endif()

if(NOT(TARGET VTK::GUISupportQt))
  message(FATAL_ERROR "MinimalQtVTKApp: VTK not built with Qt support.")
endif()

# Main executable
add_executable(MeshAnnotator ControlPanel.cpp VtkViewport.cpp mainwindow.cpp main.cpp)
target_link_libraries(MeshAnnotator PRIVATE VTKLayer VTK::GUISupportQt VTK::RenderingQt VTK::IOPLY)

if (Qt${VTK_QT_VERSION}Widgets_VERSION VERSION_LESS "5.11.0")
  qt5_use_modules(MeshAnnotator ${qt_components})
else()
  target_link_libraries(MeshAnnotator PRIVATE ${qt_modules})
endif()

# Ensure VTK modules are initialized correctly for the executable
vtk_module_autoinit(
  TARGETS MeshAnnotator
  MODULES VTK::GUISupportQt VTK::RenderingQt
)
 