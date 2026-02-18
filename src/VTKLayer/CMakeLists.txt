find_package(VTK COMPONENTS
  FiltersSources
  FiltersGeometry
  RenderingCore
  RenderingOpenGL2
  InteractionStyle
  InteractionWidgets
  REQUIRED)

add_library(VTKLayer 
  vtkInteractorStyleTrackballCameraWithPicker.h
  InteractorStylePointPicker.h
  InteractorStylePointPicker.cpp
  InteractorStyleCellPicker.h
  InteractorStyleCellPicker.cpp)

target_include_directories(VTKLayer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(VTKLayer PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(VTKLayer PUBLIC Core ${VTK_LIBRARIES})
# Ensure VTK modules are initialized correctly for the executable
vtk_module_autoinit(
  TARGETS VTKLayer
  MODULES ${VTK_LIBRARIES}
)
 