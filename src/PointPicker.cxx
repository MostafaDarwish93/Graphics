#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPointPicker.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>
#include <vtkTextRepresentation.h>
#include <vtkSmartPointer.h>
#include <vtkTextWidget.h>
#include <vtkTextActor.h>
double picked[3];


// Create a new instance of vtkLineSource using smart pointers
vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();


namespace {

    // Define interaction style
    class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
    {
    public:
        static MouseInteractorStylePP* New();
        vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);

        int check = 0;
        virtual void OnLeftButtonDown() override
        {
            std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
                << " " << this->Interactor->GetEventPosition()[1] << std::endl;
            this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                this->Interactor->GetEventPosition()[1],
                0, // always zero.
                this->Interactor->GetRenderWindow()
                ->GetRenderers()
                ->GetFirstRenderer());
            this->Interactor->GetPicker()->GetPickPosition(picked);
            std::cout << "Picked value: " << picked[0] << " " << picked[1] << " "
                << picked[2] << std::endl;
            check++;
            if (check == 1) {
                this->SetFirstPoint();
                this->SetSecondPoint();
                this->DrawLine();
            }
            else if (check == 2) {
                this->SetSecondPoint();
                check = 0;
                this->DrawLine();
            }
            // Forward events
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }
        virtual void SetFirstPoint() {
            lineSource->SetPoint1(picked[0], picked[1], picked[2]);
            return;
        }
        virtual void SetSecondPoint() {
            lineSource->SetPoint2(picked[0], picked[1], picked[2]);
            return;
        }
        virtual void DrawLine() {
            // Create mapper and actor for the line
            vtkNew<vtkPolyDataMapper> mapper;
            mapper->SetInputConnection(lineSource->GetOutputPort());

            vtkNew<vtkActor> actor;
            actor->SetMapper(mapper);
            actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

            // Add the actor to the renderer
            this->Interactor->GetRenderWindow()
                ->GetRenderers()
                ->GetFirstRenderer()
                ->AddActor(actor);

            // Render the scene
            this->Interactor->GetRenderWindow()->Render();
            return;
            /////////////////////////////////////////////////////////////////////
            return;
        }

    };

    vtkStandardNewMacro(MouseInteractorStylePP);

} // namespace

int main(int, char* [])
{
    vtkNew<vtkNamedColors> colors;

    ///vtkNew<vtkSphereSource> sphereSource;
    //sphereSource->Update();

    vtkNew<vtkPointPicker> pointPicker;

    // Create a mapper and actor
    //vtkNew<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputConnection(lineSource->GetOutputPort());
    //mapper->SetInputConnection(sphereSource->GetOutputPort());
    //vtkNew<vtkActor> actor;
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

    actor->SetMapper(mapper); //////////////////////////////////////////////////////////
    actor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());

    // Create a renderer, render window, and interactor
    //vtkNew<vtkRenderer> renderer;  //////////////////////////////////////////////////////////
    //vtkNew<vtkRenderWindow> renderWindow;   ///////////////////////////////////////////////////////
    renderWindow->AddRenderer(renderer);
    //renderWindow->SetWindowName("PointPicker"); ///////////////////////////////////////////////
    //vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;    ////////////////////////
    renderWindowInteractor->SetPicker(pointPicker);
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkNew<MouseInteractorStylePP> style;
    renderWindowInteractor->SetInteractorStyle(style);

    // Add the actor to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

    // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;

}