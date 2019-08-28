cmake_minimum_required(VERSION 2.6)
 
PROJECT(ParaViewScalarBar)
 
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

INCLUDE_DIRECTORIES(Example/ ParaViewScalarBar/)

ADD_EXECUTABLE(ParaViewScalarBar Example/ParaViewScalarBar.cxx
                                 Example/vtkDiscretizableColorTransferFunction.cxx # bug fixed version
                                 ParaViewScalarBar/vtkParaViewScalarBar.cxx
                                 ParaViewScalarBar/vtkContext2DScalarBarActor.cxx
                                 ParaViewScalarBar/vtkPVScalarBarRepresentation.cxx
                                 ParaViewScalarBar/vtkBoundingRectContextDevice2D.cxx)
TARGET_LINK_LIBRARIES(ParaViewScalarBar ${VTK_LIBRARIES})
