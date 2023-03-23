#include <vtkActor.h> // Includes the header file for vtkActor class
#include <vtkInteractorStyleTrackballCamera.h> // Includes the header file for vtkInteractorStyleTrackballCamera class
#include <vtkNamedColors.h> // Includes the header file for vtkNamedColors class
#include <vtkNew.h> // Includes the header file for vtkNew macro
#include <vtkObjectFactory.h> // Includes the header file for vtkObjectFactory class
#include <vtkPointPicker.h> // Includes the header file for vtkPointPicker class
#include <vtkPolyDataMapper.h> // Includes the header file for vtkPolyDataMapper class
#include <vtkProperty.h> // Includes the header file for vtkProperty class
#include <vtkRenderWindow.h> // Includes the header file for vtkRenderWindow class
#include <vtkRenderWindowInteractor.h> // Includes the header file for vtkRenderWindowInteractor class
#include <vtkRenderer.h> // Includes the header file for vtkRenderer class
#include <vtkRendererCollection.h> // Includes the header file for vtkRendererCollection class
#include <vtkSphereSource.h> // Includes the header file for vtkSphereSource class
#include <vtkLineSource.h> // Includes the header file for vtkLineSource class
#include <vtkSmartPointer.h> // Includes the header file for vtkSmartPointer class
#include <vtkTextRepresentation.h> // Includes the header file for vtkTextRepresentation class
#include <vtkSmartPointer.h> // Includes the header file for vtkSmartPointer class (repeated)
#include <vtkTextWidget.h> // Includes the header file for vtkTextWidget class
#include <vtkTextActor.h> // Includes the header file for vtkTextActor class
#include <vtkTextProperty.h> // Includes the header file for vtkTextProperty class
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkDataSetMapper.h>



#include <QApplication> // Includes the header file for QApplication class
#include <QDockWidget> // Includes the header file for QDockWidget class
#include <QGridLayout> // Includes the header file for QGridLayout class
#include <QLabel> // Includes the header file for QLabel class
#include <QMainWindow> // Includes the header file for QMainWindow class
#include <QPointer> // Includes the header file for QPointer class
#include <QPushButton> // Includes the header file for QPushButton class
#include <QVBoxLayout> // Includes the header file for QVBoxLayout class
#include <QFileDialog> 
#include <qcombobox.h>




double picked[3]; // Declares an array of 3 doubles called "picked"


vtkSmartPointer<vtkActor> Lineactor;

vtkNew<vtkGenericOpenGLRenderWindow> window;


// Create a new instance of vtkLineSource using smart pointers
vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();

// Create a new instance of vtkRenderWindowInteractor using vtkNew macro
vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

// Create a new instance of vtkRenderer using smart pointers
vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

// Create a new instance of vtkTextWidget using smart pointers
vtkSmartPointer<vtkTextWidget> textWidget = vtkSmartPointer<vtkTextWidget>::New();

// Create a new instance of vtkRenderWindow using smart pointers
//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

// Define interaction style
    // 
    // Define a function to change the color of the line
void ChangeColor(QComboBox* comboBox, vtkActor* actor, vtkLineSource* line)
{
    QString color_name = comboBox->currentText();

    if (color_name == "Red")
    {
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    }
    else if (color_name == "Green")
    {
        actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color_name == "Blue")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color_name == "Yellow")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color_name == "Magenta")
    {
        actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    window->Render();
}

void UpdateLineThickness(int thickness, vtkActor* actor) {
    actor->GetProperty()->SetLineWidth(thickness);
    actor->GetMapper()->Update();
    window->Render();
}


// Save Function
void Save(const char* filename) {
    // Get the start and end points of the line
    double* startPoint = lineSource->GetPoint1();
    double* endPoint = lineSource->GetPoint2();

    // Open the output file for writing
    std::ofstream outputFile(filename);

    // Write the start and end points to the output file
    outputFile << "Start point: " << startPoint[0] << ", " << startPoint[1] << std::endl;
    outputFile << "End point: " << endPoint[0] << ", " << endPoint[1] << std::endl;

    // Close the output file
    outputFile.close();
}

// Upload Function
void Upload() {
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Line", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Failed to open file for reading." << std::endl;
        return;
    }

    QTextStream in(&file);
    double x1, y1, z1, x2, y2, z2;
    in >> x1 >> y1 >> z1;
    in >> x2 >> y2 >> z2;

    //vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
    // Set the first and second endpoints of the line using the coordinates read from the file
    lineSource->SetPoint1(x1, y1, 0);
    lineSource->SetPoint2(x2, y2, 0);
    //window->Render();

    file.close();
}
namespace {

    class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
    {
    private:
        vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkActor> Lineactor = vtkSmartPointer<vtkActor>::New();
    public:
        static MouseInteractorStylePP* New();
        vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);
        int check = 0;
        double picked[3];
        void setlinesource(vtkSmartPointer<vtkLineSource> lSource) {
            lineSource = lSource;
        }
        void setlineactor(vtkSmartPointer<vtkActor> lactor){
            Lineactor = lactor;
        }
        vtkSmartPointer<vtkLineSource> getlineSource()
        {
            return lineSource;
        }
        vtkSmartPointer<vtkActor> getLineactor() {
            return Lineactor;
        }
        virtual void OnLeftButtonDown() override
        {
            // Print the pixel coordinates of the mouse click event to the console
            std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
                << " " << this->Interactor->GetEventPosition()[1] << std::endl;

            // Use the vtkPicker to pick an object in the scene at the location of the mouse click event
            this->Interactor->GetPicker()->Pick(
                this->Interactor->GetEventPosition()[0], // x-coordinate of the mouse click event
                this->Interactor->GetEventPosition()[1], // y-coordinate of the mouse click event
                0, // always zero (z-coordinate of the mouse click event)
                this->Interactor->GetRenderWindow()
                ->GetRenderers()
                ->GetFirstRenderer() // pick from the first renderer in the render window
            );

            // Get the 3D position of the pick location
            this->Interactor->GetPicker()->GetPickPosition(picked);

            // Print the 3D coordinates of the picked location to the console
            std::cout << "Picked value: " << picked[0] << " " << picked[1] << " "
                << picked[2] << std::endl;

            // Increment the check variable to keep track of the number of clicks
            check++;

            // If this is the first click, set the first point and draw a temporary line
            if (check == 1) {
                this->SetFirstPoint();
                this->SetSecondPoint();
                this->DrawLine();
            }
            // If this is the second click, set the second point, draw the final line, and reset the check variable
            else if (check == 2) {
                this->SetSecondPoint();
                check = 0;
                this->DrawLine();
            }

            // Get the renderer and the render window
            vtkSmartPointer<vtkRenderer> renderer = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
            vtkSmartPointer<vtkRenderWindow> renderWindow = this->Interactor->GetRenderWindow();

            // Create a vtkTextActor and a vtkTextWidget to display the coordinates
            vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
            vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
            textActor->SetInput("(0, 0)");
            textActor->GetTextProperty()->SetColor(colors->GetColor3d("White").GetData());
            textActor->GetTextProperty()->SetFontSize(30);
            textWidget->SetInteractor(renderWindowInteractor);
            textWidget->SetRepresentation(vtkSmartPointer<vtkTextRepresentation>::New());
            textWidget->GetTextActor()->SetPosition2(0.2, 0.15);
            textWidget->GetTextActor()->SetDisplayPosition(60, 10);

            // Get the size of the renderer
            int* rendererSize = renderer->GetSize();

            // Calculate the middle position for the text
            int xPosition = rendererSize[0] / 2;
            int yPosition = 10 + textActor->GetTextProperty()->GetFontSize() + (rendererSize[1] - textActor->GetTextProperty()->GetFontSize() - 20) / 2;

            textWidget->GetTextActor()->SetPosition2(0, 0); // Reset the position of the text actor
            textWidget->GetTextActor()->SetDisplayPosition(xPosition, yPosition); // Set the new position for the text actor

            textWidget->SelectableOff();

            // Add the vtkTextActor to the renderer
            renderer->AddActor2D(textActor);

            // Enable the vtkTextWidget
            textWidget->SetEnabled(true);

            // Set the coordinates as the text for the vtkTextActor
            std::string text = "(" + std::to_string(picked[0]) + ", " + std::to_string(picked[1]) + ")";
            textActor->SetInput(text.c_str());


            // Forward the left button down event to the parent class vtkInteractorStyleTrackballCamera
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }
        virtual void SetFirstPoint() {
            lineSource->SetPoint1(picked[0], picked[1], picked[2]); // Set the first endpoint of the line
            return;
        }
        virtual void SetSecondPoint() {
            lineSource->SetPoint2(picked[0], picked[1], picked[2]); // Set the second endpoint of the line
            return;
        }
        virtual void DrawLine() {
            // Remove any previously drawn objects from the renderer
            this->Interactor->GetRenderWindow()
                ->GetRenderers()
                ->GetFirstRenderer()
                ->RemoveAllViewProps();

            // Create a mapper and actor for the line
            vtkNew<vtkPolyDataMapper> mapper;
            mapper->SetInputConnection(lineSource->GetOutputPort());

            //vtkNew<vtkActor> lineActor;
            //lineActor->SetMapper(mapper);
            Lineactor->GetProperty()->SetColor(1.0, 0.0, 0.0);

            // Add the actor to the renderer
            this->Interactor->GetRenderWindow()
                ->GetRenderers()
                ->GetFirstRenderer()
                ->AddActor(Lineactor);

            // Render the scene
            this->Interactor->GetRenderWindow()->Render();
        }

    };
    vtkStandardNewMacro(MouseInteractorStylePP);
} // namespace

int main(int argc, char* argv[])
{

    // Create a new instance of a Qt application
    QApplication app(argc, argv);

    // main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Line Drawer");
    mainWindow.resize(1200, 900);

    // control area
    QDockWidget controlDock;  // Declare a new instance of a QDockWidget named controlDock.
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, &controlDock);   // Add the controlDock widget to the main window on the left side of the window.

    QLabel controlDockTitle("Control Dock"); // Declare a new instance of a QLabel named controlDockTitle with the text "Control Dock".
    controlDockTitle.setMargin(20);   //Set the margin of the controlDockTitle label to 20 pixels.
    controlDock.setTitleBarWidget(&controlDockTitle);   //Set the title bar widget of controlDock to controlDockTitle.

    QVBoxLayout* dockLayout = new QVBoxLayout();   //Declare a new instance of a QVBoxLayout named dockLayout.
    QWidget layoutContainer;   //Declare a new instance of a QWidget named layoutContainer.
    layoutContainer.setLayout(dockLayout);     //Set the layout of layoutContainer to dockLayout. 
    controlDock.setWidget(&layoutContainer);      //Set the widget of controlDock to layoutContainer/ Create a new MouseInteractorStylePP object

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    // Save Button 
    QPushButton* saveButton = new QPushButton("Save");
    dockLayout->addWidget(saveButton);

    // Upload Button 
    QPushButton* uploadButton = new QPushButton("Upload");
    dockLayout->addWidget(uploadButton);

    // Change color Button 
    QPushButton* changeColorButton = new QPushButton("Change color");
    dockLayout->addWidget(changeColorButton);

    // Color droplist
    QComboBox* colorDroplist = new QComboBox();
    colorDroplist->addItem("Red");
    colorDroplist->addItem("Green");
    colorDroplist->addItem("Blue");
    colorDroplist->addItem("Yellow");
    colorDroplist->addItem("Magenta");
    colorDroplist->setCurrentIndex(0); // Set default value
    dockLayout->addWidget(colorDroplist);

    // Thickness Slider
    QSlider* thicknessSlider = new QSlider(Qt::Horizontal);
    thicknessSlider->setMinimum(1);
    thicknessSlider->setMaximum(10);
    thicknessSlider->setValue(1); // Set default value
    dockLayout->addWidget(thicknessSlider);


    //Lineactor = vtkSmartPointer<vtkActor>::New(); // Fix the typo here
    vtkSmartPointer<vtkActor> Lineactor = vtkSmartPointer<vtkActor>::New();

    // render area
    QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget = new QVTKOpenGLNativeWidget();
    mainWindow.setCentralWidget(vtkRenderWidget);

    // VTK part
    //vtkNew<vtkGenericOpenGLRenderWindow> window;
    vtkRenderWidget->setRenderWindow(window.Get());

    //// Connect Save button
    //QString filename = "data.txt";  // Example filename
    //QObject::connect(saveButton, &QPushButton::released, [&]() {
    //    ::Save(filename.toStdString().c_str()); 
    //    });

    //// Connect Upload button
    //QObject::connect(uploadButton, &QPushButton::clicked, [&]() {
    //    ::Upload(); 
    //    });

    //// Connect Change Color 
    //QObject::connect(changeColorButton, &QPushButton::clicked, [=]() {
    //    ChangeColor(colorDroplist, Lineactor);
    //    });

    //// Connect Change Thickness
    //QObject::connect(thicknessSlider, &QSlider::valueChanged, [&]() {
    //    int thickness = thicknessSlider->value();
    //    UpdateLineThickness(thickness, Lineactor);
    //    });

    vtkNew<vtkPointPicker> pointPicker; // Create a new instance of the VTK point picker
    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputConnection(lineSource->GetOutputPort()); // Set the input of the mapper to the output of the line source
    mapper->Update();
    Lineactor->SetMapper(mapper); // Set the mapper for the actor
    Lineactor->GetProperty()->SetColor(1, 0.8941, 0.8824); // Set the color of the actor's property
    vtkNew <MouseInteractorStylePP> style; // Create a new instance of a custom VTK interactor style
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(Lineactor);
    window->AddRenderer(renderer);    //add renderer to window   
    renderer->SetBackground(0.4392, 0.502, 0.5647); // Set the background color of the renderer
    window->GetInteractor()->SetPicker(pointPicker);    // connect between qt and vtk
    window->SetInteractor(vtkRenderWidget->interactor());
    window->GetInteractor()->SetInteractorStyle(style);
    style->setlinesource(lineSource);
    style->setlineactor(Lineactor);
    //vtkRenderWidget->update();    //update render
    window->Render();    // Render the window
    // Connect Save button
    QString filename = "data.txt";  // Example filename
    QObject::connect(saveButton, &QPushButton::released, [&]() {
        ::Save(filename.toStdString().c_str());
        });

    // Connect Upload button
    QObject::connect(uploadButton, &QPushButton::clicked, [&]() {
        ::Upload();
        });

    // Connect Change Color 
    QObject::connect(changeColorButton, &QPushButton::clicked, [=]() {
        ChangeColor(colorDroplist,Lineactor, lineSource);
        });

    // Connect Change Thickness
    QObject::connect(thicknessSlider, &QSlider::valueChanged, [&]() {
        int thickness = thicknessSlider->value();
        UpdateLineThickness(thickness, Lineactor);
        });

    mainWindow.show();
    //window->GetInteractor()->Start();    // Start the render window interactor event loop

    return app.exec(); // Run the Qt application event loop and return the exit code

}