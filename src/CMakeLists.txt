# Link to vcpkg-provided libraries
find_package(fmt CONFIG REQUIRED)

find_package(VTK COMPONENTS
  FiltersSources
  FiltersGeometry
  RenderingCore
  RenderingOpenGL2
  InteractionStyle
  InteractionWidgets
  REQUIRED)

# Main executable
add_executable(MeshAnnotator main.cpp InteractorStyleCellTrackerWrapper.h InteractorStylePointTrackerWrapper.h LandmarksPickingCallback.h CellsPickingCallback.h)

target_link_libraries(MeshAnnotator PRIVATE ${VTK_LIBRARIES})

# Ensure VTK modules are initialized correctly for the executable
vtk_module_autoinit(
  TARGETS MeshAnnotator
  MODULES ${VTK_LIBRARIES}
)
 