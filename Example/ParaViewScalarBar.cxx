#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkFloatArray.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>

#include "vtkParaViewScalarBar.h"

static void CallbackFunction ( vtkObject* caller, long unsigned int eventId,
          void* clientData, void* callData );

int main (int, char *[])
{
  // Create a sphere for some geometry
  vtkSmartPointer<vtkSphereSource> sphere = 
    vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetCenter(0,0,0);
  sphere->SetRadius(1);
  sphere->Update();
 
  // Create scalar data to associate with the vertices of the sphere
  int numPts = sphere->GetOutput()->GetPoints()->GetNumberOfPoints();
  vtkSmartPointer<vtkFloatArray> scalars =
    vtkSmartPointer<vtkFloatArray>::New();
  scalars->SetNumberOfValues( numPts );
  for( int i = 0; i < numPts; ++i )
    {
    scalars->SetValue(i,static_cast<float>(i)/numPts);
    }
  vtkSmartPointer<vtkPolyData> poly =
    vtkSmartPointer<vtkPolyData>::New();
  poly->DeepCopy(sphere->GetOutput());
  poly->GetPointData()->SetScalars(scalars);
 
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  mapper->SetInput(poly);
#else
  mapper->SetInputData(poly);
#endif
  mapper->ScalarVisibilityOn();
  mapper->SetScalarModeToUsePointData();
  mapper->SetColorModeToMapScalars();
 
  vtkSmartPointer<vtkActor> actor = 
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  vtkNew<vtkParaViewScalarBar> scalarBar;
  scalarBar->SetTitle("Sphere");
 
  // Create a lookup table to share between the mapper and the scalarbar
  vtkSmartPointer<vtkDiscretizableColorTransferFunction> blueToRedRainbowLut =
    vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
  blueToRedRainbowLut->SetColorSpaceToHSV();
  blueToRedRainbowLut->SetHSVWrap(0);
  blueToRedRainbowLut->AddRGBPoint(0, 0, 0, 1);
  blueToRedRainbowLut->AddRGBPoint(1, 1, 0, 0);
  blueToRedRainbowLut->Build();

  scalarBar->SetLookupTable( blueToRedRainbowLut );
  mapper->SetLookupTable( blueToRedRainbowLut );
 
  // Create a renderer and render window
  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
 
  renderer->GradientBackgroundOn();
  renderer->SetBackground(1,1,1);
  renderer->SetBackground2(0,0,0);
 
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
 
  // Create an interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  // Add the actors to the scene
  renderer->AddActor(actor);

  scalarBar->SetRenderer(renderer);
  scalarBar->SetInteractor(renderWindowInteractor);

  vtkSmartPointer<vtkCallbackCommand> callback =
      vtkSmartPointer<vtkCallbackCommand>::New();
  callback->SetCallback(CallbackFunction);
  renderer->AddObserver(vtkCommand::EndEvent, callback);
 
  // Render the scene (lights and cameras are created automatically)
  renderWindow->Render();
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}

void CallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData) )
{
  vtkRenderer* renderer = static_cast<vtkRenderer*>(caller);

  double timeInSeconds = renderer->GetLastRenderTimeInSeconds();
  double fps = 1.0/timeInSeconds;
  std::cout << "FPS: " << fps << std::endl;
}
