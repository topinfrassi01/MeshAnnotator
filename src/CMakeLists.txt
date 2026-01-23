# Link to vcpkg-provided libraries
find_package(fmt CONFIG REQUIRED)

find_package(VTK COMPONENTS
  FiltersSources
  FiltersGeometry
  RenderingCore
  RenderingOpenGL2
  InteractionStyle
  InteractionWidgets
  GUISupportQt
  REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Widgets)  
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT VTK_FOUND)
  message(FATAL_ERROR "MinimalQtVTKApp: Unable to find the VTK build folder.")
endif()

if(NOT(TARGET VTK::GUISupportQt))
  message(FATAL_ERROR "MinimalQtVTKApp: VTK not built with Qt support.")
endif()

# Main executable
add_executable(MeshAnnotator main.cpp InteractorStylePointPicker.h InteractorStyleCellPicker.h)

target_link_libraries(MeshAnnotator PRIVATE Qt6::Widgets ${VTK_LIBRARIES})

# Ensure VTK modules are initialized correctly for the executable
vtk_module_autoinit(
  TARGETS MeshAnnotator
  MODULES ${VTK_LIBRARIES}
)
 