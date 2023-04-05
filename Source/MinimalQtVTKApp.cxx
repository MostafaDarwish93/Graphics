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
#include <vtkTextWidget.h> 
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkDataSetMapper.h>
#include <iostream>
#include <vtkCellPicker.h>

#include <QApplication> 
#include <QDockWidget>
#include <QGridLayout> 
#include <QLabel> 
#include <QMainWindow> 
#include <QPointer> 
#include <QPushButton> 
#include <QVBoxLayout> 
#include <QFileDialog> 
#include <QInputDialog>
#include <qcombobox.h>
#include <vtkRegularPolygonSource.h>
using namespace std;

double picked[3]; // Declares an array of 3 doubles called "picked"
int inc = 0;

vtkSmartPointer<vtkActor> Lineactor;
//vtkSmartPointer<vtkActor> CircleActor;
std::vector<vtkSmartPointer<vtkActor>> lineActors;

vtkNew<vtkGenericOpenGLRenderWindow> window;

// Create a new instance of vtkLineSource using smart pointers
vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();

// Create global variables for the circle actor and mapper
vtkSmartPointer<vtkActor> Circle_actor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkPolyDataMapper> Circle_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

// Create global variables for the polygon actor and mapper
vtkSmartPointer<vtkActor> Polygon_actor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkPolyDataMapper> Polygon_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

// Create a new instance of vtkRenderWindowInteractor using vtkNew macro
vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

// Create a new instance of vtkRenderer using smart pointers
vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

// Create a new instance of vtkTextWidget using smart pointers
vtkSmartPointer<vtkTextWidget> textWidget = vtkSmartPointer<vtkTextWidget>::New();

// Define interaction style
    // 

void Draw_circle(double x, double y)
{
    // Get the radius of the circle from the user using a QInputDialog
    bool ok;
    double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the circle:", 1.0, 0.0, 100.0, 2, &ok);
    if (!ok) {
        return;
    }

    // Update the center and radius of the circle source
    vtkSmartPointer<vtkRegularPolygonSource> circleSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
    circleSource->SetNumberOfSides(50);
    circleSource->SetCenter(x, y, 0);
    circleSource->SetRadius(radius);
    circleSource->Update();

    // Update the mapper with the new data
    Circle_mapper->SetInputConnection(circleSource->GetOutputPort());
    Circle_mapper->Update();

    // Update the actor with the new mapper
    Circle_actor->SetMapper(Circle_mapper);
    Circle_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    // Add the actor to the renderer
    renderer->AddActor(Circle_actor);
    window->AddRenderer(renderer);
}




void Draw_Polygon() {
    // Prompt the user for the number of sides of the polygon
    bool ok;
    int numSides = QInputDialog::getInt(nullptr, "Enter Number of Sides", "Enter the number of sides of the polygon:", 3, 3, 100, 1, &ok);
    if (!ok) {
        return;
    }

    // Prompt the user for the radius of the polygon
    double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the polygon:", 1.0, 0.0, 100.0, 2, &ok);
    if (!ok) {
        return;
    }

    // Prompt the user for the length of the polygon
    double length = QInputDialog::getDouble(nullptr, "Enter Length", "Enter the length of the polygon:", 1.0, 0.0, 100.0, 2, &ok);
    if (!ok) {
        return;
    }

    // Calculate the apothem of the polygon
    double apothem = length / (2 * tan(M_PI / numSides));

    // Create a vtkRegularPolygonSource with the given number of sides and apothem
    vtkSmartPointer<vtkRegularPolygonSource> polySource = vtkSmartPointer<vtkRegularPolygonSource>::New();
    polySource->SetNumberOfSides(numSides);
    polySource->SetRadius(apothem);
    polySource->Update();

    // Update the mapper with the new data
    Polygon_mapper->SetInputConnection(polySource->GetOutputPort());
    Polygon_mapper->Update();

    // Update the actor with the new mapper
    Polygon_actor->SetMapper(Polygon_mapper);

    // Set the actor color to green by default
    Polygon_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);

    // Add the actor to the renderer
    renderer->AddActor(Polygon_actor);
    window->AddRenderer(renderer);
}


void Draw_Ellipse()
{
}


    // Define a function to change the color of the line
void ChangeColor(QComboBox* comboBox, vtkActor* actor)
{
    QString color_name = comboBox->currentText();

    if (color_name == "Red")
    {
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        Circle_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color_name == "Green")
    {
        actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Circle_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color_name == "Blue")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color_name == "Yellow")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color_name == "Magenta")
    {
        actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Circle_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    window->Render();
}

void UpdateLineThickness(int thickness, vtkActor* actor) {
    actor->GetProperty()->SetLineWidth(thickness);
    Circle_actor->GetProperty()->SetLineWidth(thickness);
    Polygon_actor->GetProperty()->SetLineWidth(thickness);
    actor->GetMapper()->Update();
    window->Render();
}

// Save Function


void Save(vtkActor* actor) {
    // Get the start and end points of the line
    double* startPoint = lineSource->GetPoint1();
    double* endPoint = lineSource->GetPoint2();

    // Get the color and thickness of the line
    double* color = actor->GetProperty()->GetColor();
    double thickness = actor->GetProperty()->GetLineWidth();
    string color_name;

    // Get the name of the color based on its RGB value
    if (color[0] == 1.0 && color[1] == 0.0 && color[2] == 0.0) {
        color_name = "Red";
    }
    else if (color[0] == 0.0 && color[1] == 1.0 && color[2] == 0.0) {
        color_name = "Green";
    }
    else if (color[0] == 0.0 && color[1] == 0.0 && color[2] == 1.0) {
        color_name = "Blue";
    }
    else if (color[0] == 1.0 && color[1] == 1.0 && color[2] == 0.0) {
        color_name = "Yellow";
    }
    else if (color[0] == 1.0 && color[1] == 0.0 && color[2] == 1.0) {
        color_name = "Magenta";
    }
    else {
        color_name = "Unknown";
    }
    // Open a file dialog to let the user choose the output file
    QString filename = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");

    // If the user didn't cancel the file dialog, write to the output file
    if (!filename.isEmpty()) {
        // Open the output file for writing
        std::ofstream outputFile(filename.toStdString());

        // Write the start and end points, color, and thickness to the output file
        outputFile << "Start point: " << startPoint[0] << ", " << startPoint[1] << std::endl;
        outputFile << "End point: " << endPoint[0] << ", " << endPoint[1] << std::endl;
        outputFile << "Color: " << color_name << std::endl;
        outputFile << "Thickness: " << thickness << std::endl;

        // Close the output file
        outputFile.close();
    }
}

// Upload Function
void Upload(vtkActor* actor) {
    // Open a file dialog to select a text file
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Line", "", "Text Files (*.txt)");
    // If no file is selected, return from the function
    if (fileName.isEmpty()) {
        return;
    }
    // Open the selected file in read-only mode and text mode
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Failed to open file for reading." << std::endl;
        return;
    }

    // Create a QTextStream object to read the file
    QTextStream in(&file);
    QString line;

    // Read the file line by line
    while (!in.atEnd()) {
        // Get the current line from the file
        line = in.readLine();
        // Check if the line contains the start point coordinates
        if (line.startsWith("Start point:")) {
            // Extract the coordinates from the line and set the start point of the line source
            QStringList coords = line.section(":", 1).split(",");
            double x1 = coords[0].toDouble();
            double y1 = coords[1].toDouble();
            lineSource->SetPoint1(x1, y1, 0);
        }
        // Check if the line contains the end point coordinates
        else if (line.startsWith("End point:")) {
            // Extract the coordinates from the line and set the end point of the line source
            QStringList coords = line.section(":", 1).split(",");
            double x2 = coords[0].toDouble();
            double y2 = coords[1].toDouble();
            lineSource->SetPoint2(x2, y2, 0);
        }
        // Check if the line contains the color information
        else if (line.startsWith("Color:")) {
            // Extract the color values from the line and set the color of the actor based on the color name
            QStringList colorValues = line.section(":", 1).trimmed().split(" ");
            string color = colorValues.at(0).toStdString();
            if (color == "Red") {
                actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
            }
            else if (color == "Blue") {
                actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
            }
            else if (color == "Yellow") {
                actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
            }
            else if (color == "Green") {
                actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
            }
            else if (color == "Magenta") {
                actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
            }
        }
        // Check if the line contains the Thickness information
        else if (line.startsWith("Thickness:")) {
            // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
            int thickness = line.section(":", 1).trimmed().toInt();
            actor->GetProperty()->SetLineWidth(thickness);
        }
    }
    window->Render();
    file.close();
}
void Change_Shapes(QComboBox* comboBox, double* mouse)
{
    // Remove any existing actors from the renderer
    renderer->RemoveAllViewProps();

    QString shape_name = comboBox->currentText();

    if (shape_name == "Circle")
    {
        // Get the x and y coordinates of the mouse click
        double x = mouse[0];
        double y = mouse[1];

        // Pass the coordinates to the Draw_circle function
        Draw_circle(x, y);
    }
    else if (shape_name == "Ellipse")
    {
        Draw_Ellipse();
    }
    else if (shape_name == "Polygon")
    {
        Draw_Polygon();
    }
    else if (shape_name == "Yellow")
    {
        
    }
    else if (shape_name == "Magenta")
    {
        
    }
    else if (shape_name == "Polyline") {
        //Draw_Polyline();
    }
    // Reset the camera and render the window
    renderer->ResetCamera();
    window->Render();
}
namespace {

    class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
    {
    private:
        vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkActor> Lineactor = vtkSmartPointer<vtkActor>::New();
        double mousePos[2] = { 0.0, 0.0 };
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
        void SetMousePos(double x, double y)
        {
            mousePos[0] = x;
            mousePos[1] = y;
        }
        double* GetMousePos()
        {
            return mousePos;
        }
        virtual void OnLeftButtonDown() override
        {
            // Get the pixel coordinates of the mouse click event
            double x = this->Interactor->GetEventPosition()[0];
            double y = this->Interactor->GetEventPosition()[1];

            // Set the mouse coordinates
            this->SetMousePos(x, y);

            // Print the pixel coordinates of the mouse click event to the console
           /* std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
                << " " << this->Interactor->GetEventPosition()[1] << std::endl;*/

                // Use the vtkPicker to pick an object in the scene at the location of the mouse click event
            this->Interactor->GetPicker()->Pick(
                x, // x-coordinate of the mouse click event
                y, // y-coordinate of the mouse click event
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
    mainWindow.setWindowTitle("Computer Graphics & Visualization");
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
    thicknessSlider->setMaximum(20);
    thicknessSlider->setValue(1); // Set default value
    dockLayout->addWidget(thicknessSlider);

    // Chooosing Shapes to draw button
    QPushButton* changeshapes = new QPushButton("Change Shape");
    dockLayout->addWidget(changeshapes);

    // Color droplist
    QComboBox* shapesdroplist = new QComboBox();
    shapesdroplist->addItem("Line");
    shapesdroplist->addItem("Polyline");
    shapesdroplist->addItem("Polygon");
    shapesdroplist->addItem("Regular Polygon");
    shapesdroplist->addItem("Circle");
    shapesdroplist->addItem("Arc");
    shapesdroplist->addItem("Ellipse");
    shapesdroplist->setCurrentIndex(0); // Set default value
    dockLayout->addWidget(shapesdroplist);

    //Lineactor = vtkSmartPointer<vtkActor>::New(); // Fix the typo here
    vtkSmartPointer<vtkActor> Lineactor = vtkSmartPointer<vtkActor>::New();




    // render area
    QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget = new QVTKOpenGLNativeWidget();
    mainWindow.setCentralWidget(vtkRenderWidget);

    // VTK part
    vtkRenderWidget->setRenderWindow(window.Get());

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
    renderer->SetBackground(0.5, 0.502, 0.5647); // Set the background color of the renderer
    window->GetInteractor()->SetPicker(pointPicker);    // connect between qt and vtk
    window->SetInteractor(vtkRenderWidget->interactor());
    window->GetInteractor()->SetInteractorStyle(style);
    style->setlinesource(lineSource);
    style->setlineactor(Lineactor);
    //vtkRenderWidget->update();    //update render
    window->Render();    // Render the window
    // Connect Save button
    QObject::connect(saveButton, &QPushButton::released, [&]() {
        ::Save(Lineactor);
        });

    // Connect Upload button
    QObject::connect(uploadButton, &QPushButton::clicked, [&]() {
        ::Upload(Lineactor);
        });

    // Connect Change Color 
    QObject::connect(changeColorButton, &QPushButton::clicked, [=]() {
        ChangeColor(colorDroplist,Lineactor);
        });

    // Connect Change Thickness
    QObject::connect(thicknessSlider, &QSlider::valueChanged, [&]() {
        int thickness = thicknessSlider->value();
        UpdateLineThickness(thickness, Lineactor);
        });
    double* mouse = style->GetMousePos();
    // Connect Change Color 
    QObject::connect(changeshapes, &QPushButton::clicked, [=]() {
        Change_Shapes(shapesdroplist, mouse);
        });

    mainWindow.show();

    return app.exec(); // Run the Qt application event loop and return the exit code
}
