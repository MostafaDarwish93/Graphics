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
#include <vtkConeSource.h>

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
#include <qmessagebox.h>
using namespace std;

double picked[3]; // Declares an array of 3 doubles called "picked"
int inc = 0;
bool circle_state = 0;
bool req_poly_state = 0;
bool ellip_state = 0;
bool cone_state = 0;

void Draw_Line();
vtkSmartPointer<vtkActor> Lineactor;
//vtkSmartPointer<vtkActor> CircleActor;
std::vector<vtkSmartPointer<vtkActor>> lineActors;

vtkNew<vtkGenericOpenGLRenderWindow> window;

// Create a new instance of vtkLineSource using smart pointers
vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();

// Create global variables for the circle actor and mapper
vtkSmartPointer<vtkActor> Circle_actor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkRegularPolygonSource> circleSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
vtkSmartPointer<vtkPolyDataMapper> Circle_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

// Create global variables for the polygon actor and mapper
vtkSmartPointer<vtkActor> Polygon_actor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkRegularPolygonSource> polySource = vtkSmartPointer<vtkRegularPolygonSource>::New();
vtkSmartPointer<vtkPolyDataMapper> Polygon_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

// Create global variables for the Cone actor and mapper
vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

// Create a new instance of vtkRenderWindowInteractor using vtkNew macro
vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

// Create a new instance of vtkRenderer using smart pointers
vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

// Create a new instance of vtkTextWidget using smart pointers
vtkSmartPointer<vtkTextWidget> textWidget = vtkSmartPointer<vtkTextWidget>::New();

// Define interaction style
    // 

void Draw_circle(double x, double y, double radius, string mode, string color, int thickness)
{
    // Update the center and radius of the circle source
    circleSource->SetNumberOfSides(50);
    circleSource->SetCenter(x, y, 0);
    circleSource->SetRadius(radius);
    circleSource->Update();

    // Update the mapper with the new data
    Circle_mapper->SetInputConnection(circleSource->GetOutputPort());
    Circle_mapper->Update();

    // Update the actor with the new mapper and properties based on user's choice
    Circle_actor->SetMapper(Circle_mapper);
    if (mode == "Filled") {
        circle_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        Circle_actor->SetProperty(fillProperty);

        // Show only the edges
        Circle_actor->GetProperty()->EdgeVisibilityOn();
        Circle_actor->GetProperty()->BackfaceCullingOn();
        circle_state = 0;
    }
    if (color == "Red") {
        Circle_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        Circle_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        Circle_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    Circle_actor->GetProperty()->SetLineWidth(thickness);
    // Add the actor to the renderer
    renderer->AddActor(Circle_actor);
}


void Draw_Regular_Polygon(double x, double y, int numSides, double apothem, string mode, string color, int thickness) {
    // Create a vtkRegularPolygonSource with the given number of sides and apothem
    polySource->SetCenter(x, y, 0);
    polySource->SetNumberOfSides(numSides);
    polySource->SetRadius(apothem / cos(M_PI / numSides));
    polySource->Update();

    // Update the mapper with the new data
    Polygon_mapper->SetInputConnection(polySource->GetOutputPort());
    Polygon_mapper->Update();

    // Update the actor with the new mapper
    Polygon_actor->SetMapper(Polygon_mapper);
    if (mode == "Filled") {
        req_poly_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        Polygon_actor->SetProperty(fillProperty);

        // Show only the edges
        Polygon_actor->GetProperty()->EdgeVisibilityOn();
        Polygon_actor->GetProperty()->BackfaceCullingOn();
        req_poly_state = 0;
    }
    if (color == "Red") {
        Polygon_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        Polygon_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        Polygon_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    Polygon_actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(Polygon_actor);
}


void Draw_Ellipse()
{
}

void Draw_Line() {
    
}

void Draw_Cone(double x, double y, double height, int res, double rad, string color, int thickness, string mode) {
    coneSource->SetHeight(height);
    coneSource->SetCenter(x, y, 0);
    coneSource->SetRadius(rad);
    coneSource->SetResolution(res);

    coneMapper->SetInputConnection(coneSource->GetOutputPort());
    coneMapper->Update();

    coneActor->SetMapper(coneMapper);
    if (mode == "Filled") {
        cone_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        coneActor->SetProperty(fillProperty);

        // Show only the edges
        coneActor->GetProperty()->EdgeVisibilityOn();
        coneActor->GetProperty()->BackfaceCullingOn();
        cone_state = 0;
    }
    if (color == "Red") {
        coneActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        coneActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        coneActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        coneActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        coneActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        coneActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        coneActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    coneActor->GetProperty()->SetLineWidth(thickness);
    
    

    // Add the cone actor to the renderer
    renderer->AddActor(coneActor);
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
        coneActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color_name == "Green")
    {
        actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Circle_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        coneActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color_name == "Blue")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        coneActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color_name == "Yellow")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        coneActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color_name == "Magenta")
    {
        actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Circle_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        coneActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color_name == "Black")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        coneActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color_name == "White")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        coneActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    window->Render();
}

void UpdateLineThickness(int thickness, vtkActor* actor) {
    actor->GetProperty()->SetLineWidth(thickness);
    Circle_actor->GetProperty()->SetLineWidth(thickness);
    Polygon_actor->GetProperty()->SetLineWidth(thickness);
    coneActor->GetProperty()->SetLineWidth(thickness);
    actor->GetMapper()->Update();
    window->Render();
}

// Save Function


void Save(vtkActor* actor, QComboBox* comboBox) {
    // Get the start and end points of the line
    QString shape_name = comboBox->currentText();
    if (shape_name == "Line") {
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
        else if (color[0] == 0.0 && color[1] == 0.0 && color[2] == 0.0) {
            color_name = "Black";
        }
        else if (color[0] == 1.0 && color[1] == 1.0 && color[2] == 1.0) {
            color_name = "White";
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
            outputFile << "Line" << std::endl;
            outputFile << "Start point: " << startPoint[0] << ", " << startPoint[1] << std::endl;
            outputFile << "End point: " << endPoint[0] << ", " << endPoint[1] << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;

            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Circle") {
        // Get the start and end points of the line
        double centerX = circleSource->GetCenter()[0];
        double centerY = circleSource->GetCenter()[1];
        double radius = circleSource->GetRadius();

        // Get the color and thickness of the line
        double* color = Circle_actor->GetProperty()->GetColor();
        double thickness = Circle_actor->GetProperty()->GetLineWidth();
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
        else if (color[0] == 0.0 && color[1] == 0.0 && color[2] == 0.0) {
            color_name = "Black";
        }
        else if (color[0] == 1.0 && color[1] == 1.0 && color[2] == 1.0) {
            color_name = "White";
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
            outputFile << "Circle" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Radius: " << radius << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            if (circle_state == 1) {
                outputFile << "Circle State: " << "Filled" << std::endl;
            }
            else {
                outputFile << "Circle State: " << "Non-Filled" << std::endl;

            }
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Regular Polygon") {
        // Get the start and end points of the line
        double centerX = polySource->GetCenter()[0];
        double centerY = polySource->GetCenter()[1];
        double radius = polySource->GetRadius();
        int no_sides = polySource->GetNumberOfSides();

        // Get the color and thickness of the line
        double* color = Polygon_actor->GetProperty()->GetColor();
        double thickness = Polygon_actor->GetProperty()->GetLineWidth();
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
        else if (color[0] == 0.0 && color[1] == 0.0 && color[2] == 0.0) {
            color_name = "Black";
        }
        else if (color[0] == 1.0 && color[1] == 1.0 && color[2] == 1.0) {
            color_name = "White";
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
            outputFile << "Regular Polygon" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Radius: " << radius << std::endl;
            outputFile << "Number of Sides: " << no_sides << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            if (req_poly_state == 1) {
                outputFile << "Regular Polygon State: " << "Filled" << std::endl;
            }
            else {
                outputFile << "Regular Polygon State: " << "Non-Filled" << std::endl;

            }
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Cone") {
        // Get the start and end points of the line
        double centerX = coneSource->GetCenter()[0];
        double centerY = coneSource->GetCenter()[1];
        double radius = coneSource->GetRadius();
        int reso = coneSource->GetResolution();
        int height = coneSource->GetHeight();

        // Get the color and thickness of the line
        double* color = coneActor->GetProperty()->GetColor();
        double thickness = coneActor->GetProperty()->GetLineWidth();
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
        else if (color[0] == 0.0 && color[1] == 0.0 && color[2] == 0.0) {
            color_name = "Black";
        }
        else if (color[0] == 1.0 && color[1] == 1.0 && color[2] == 1.0) {
            color_name = "White";
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
            outputFile << "Cone" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Radius: " << radius << std::endl;
            outputFile << "Height: " << height << std::endl;
            outputFile << "Resolution: " << reso << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            if (cone_state == 1) {
                outputFile << "Cone State: " << "Filled" << std::endl;
            }
            else {
                outputFile << "Cone State: " << "Non-Filled" << std::endl;

            }
            // Close the output file
            outputFile.close();
        }
    }
}

// Upload Function
void Upload(vtkActor* actor) {
    // Open a file dialog to select a text file
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Shape", "", "Text Files (*.txt)");
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

    // Get the current line from the file
    line = in.readLine();
    // Read the file line by line
    if (line.startsWith("Line")) {
        !in.atEnd();
        while (!in.atEnd()) {
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
                else if (color == "Black") {
                    actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
                }
                else if (color == "White") {
                    actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
                }
            }
            // Check if the line contains the Thickness information
            else if (line.startsWith("Thickness:")) {
                // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
                int thickness = line.section(":", 1).trimmed().toInt();
                actor->GetProperty()->SetLineWidth(thickness);
            }
        }
    }
    else if (line.startsWith("Circle")) {
        !in.atEnd();
        double x, y, radius;
        string color;
        int thickness;
        string state;
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the line contains the center point coordinates
            if (line.startsWith("Center point:")) {
                // Extract the coordinates from the line and set the start point of the line source
                QStringList coords = line.section(":", 1).split(",");
                x = coords[0].toDouble();
                y = coords[1].toDouble();
            }
            // Check if the line contains the radius information
            else if (line.startsWith("Radius:")) {
                // Extract the radius value from the line and set the radius of the circle source
                radius = line.section(":", 1).trimmed().toDouble();
            }
            // Check if the line contains the color information
            else if (line.startsWith("Color:")) {
                // Extract the color values from the line and set the color of the actor based on the color name
                QStringList colorValues = line.section(":", 1).trimmed().split(" ");
                color = colorValues.at(0).toStdString();
            }
            // Check if the line contains the Thickness information
            else if (line.startsWith("Thickness:")) {
                // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
                thickness = line.section(":", 1).trimmed().toInt();

            }
            // Check if the circle is filled
            else if (line.startsWith("Circle State:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_circle(x, y, radius, state, color, thickness);
    }
    else if (line.startsWith("Regular Polygon")) {
        !in.atEnd();
        double x, y, radius;
        int no_sides;
        string color;
        int thickness;
        string state;
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the line contains the center point coordinates
            if (line.startsWith("Center point:")) {
                // Extract the coordinates from the line and set the start point of the line source
                QStringList coords = line.section(":", 1).split(",");
                x = coords[0].toDouble();
                y = coords[1].toDouble();
            }
            // Check if the line contains the radius information
            else if (line.startsWith("Radius:")) {
                // Extract the radius value from the line and set the radius of the circle source
                radius = line.section(":", 1).trimmed().toDouble();
            }
            else if (line.startsWith("Number of Sides:")) {
                no_sides = line.section(":", 1).trimmed().toInt();
            }
            // Check if the line contains the color information
            else if (line.startsWith("Color:")) {
                // Extract the color values from the line and set the color of the actor based on the color name
                QStringList colorValues = line.section(":", 1).trimmed().split(" ");
                color = colorValues.at(0).toStdString();
            }
            // Check if the line contains the Thickness information
            else if (line.startsWith("Thickness:")) {
                // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
                thickness = line.section(":", 1).trimmed().toInt();

            }
            // Check if the circle is filled
            else if (line.startsWith("Regular Polygon State:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_Regular_Polygon(x, y, no_sides, radius, state, color, thickness);
    }
    else if (line.startsWith("Cone")) {
        !in.atEnd();
        double x, y, radius, height;
        int reso;
        string color;
        int thickness;
        string state;
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the line contains the center point coordinates
            if (line.startsWith("Center point:")) {
                // Extract the coordinates from the line and set the start point of the line source
                QStringList coords = line.section(":", 1).split(",");
                x = coords[0].toDouble();
                y = coords[1].toDouble();
            }
            // Check if the line contains the radius information
            else if (line.startsWith("Radius:")) {
                // Extract the radius value from the line and set the radius of the circle source
                radius = line.section(":", 1).trimmed().toDouble();
            }
            else if (line.startsWith("Height:")) {
                height = line.section(":", 1).trimmed().toDouble();
            }
            else if (line.startsWith("Resolution:")) {
                reso = line.section(":", 1).trimmed().toInt();
            }
            // Check if the line contains the color information
            else if (line.startsWith("Color:")) {
                // Extract the color values from the line and set the color of the actor based on the color name
                QStringList colorValues = line.section(":", 1).trimmed().split(" ");
                color = colorValues.at(0).toStdString();
            }
            // Check if the line contains the Thickness information
            else if (line.startsWith("Thickness:")) {
                // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
                thickness = line.section(":", 1).trimmed().toInt();

            }
            // Check if the circle is filled
            else if (line.startsWith("Regular Polygon State:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_Cone(x, y, height, reso, radius, color, thickness, state);
    }
    window->Render();
    file.close();
}
void Change_Shapes(QComboBox* comboBox)
{
    // Remove any existing actors from the renderer
    renderer->RemoveAllViewProps();

    QString shape_name = comboBox->currentText();

    if (shape_name == "Circle")
    {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the circle:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the circle:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        // Get the radius of the circle from the user using a QInputDialog
        double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the circle:", 1.0, 0.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        // Ask user for filled or non-filled region
        QMessageBox messageBox;
        messageBox.setText("Choose Region Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("Filled"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("Non-Filled"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        Draw_circle(x, y, radius, mode, "Red", 1.0);
    }
    else if (shape_name == "Ellipse")
    {
        Draw_Ellipse();
    }
    else if (shape_name == "Polygon")
    {
        //Draw_Polygon();
    }
    else if (shape_name == "Line")
    {
        Draw_Line();
    }
    else if (shape_name == "Cone"){
        // Prompt the user for the number of sides of the polygon
        bool ok;
        // Get the center of the Regular Polygon from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the Cone:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the Cone:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double Height = QInputDialog::getDouble(nullptr, "Enter Height", "Enter the Height of the Cone:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        int resoultion = QInputDialog::getInt(nullptr, "Enter Resolution", "Enter the Resolution of the Cone:", 3, 3, 100, 1, &ok);
        if (!ok) {
            return;
        }
        // Prompt the user for the radius of the Cone
        double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the polygon:", 1.0, 0.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        // Ask user for filled or non-filled region
        QMessageBox messageBox;
        messageBox.setText("Choose Region Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("Filled"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("Non-Filled"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        Draw_Cone(x, y, Height, resoultion, radius, "Red", 1.0, mode);
    }
    else if (shape_name == "Regular Polygon")
    {
        // Prompt the user for the number of sides of the polygon
        bool ok;
        // Get the center of the Regular Polygon from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the circle:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the circle:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        int numSides = QInputDialog::getInt(nullptr, "Enter Number of Sides", "Enter the number of sides of the polygon:", 3, 3, 100, 1, &ok);
        if (!ok) {
            return;
        }

        // Prompt the user for the radius of the polygon
        double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the polygon:", 1.0, 0.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double apothem = radius / (2.0 * tan(M_PI / numSides));
        // Ask user for filled or non-filled region
        QMessageBox messageBox;
        messageBox.setText("Choose Region Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("Filled"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("Non-Filled"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        Draw_Regular_Polygon(x, y, numSides, apothem, mode, "Red", 1.0);
    }
    else if (shape_name == "Polyline") {
        //Draw_Polyline();
    }
    // Reset the camera and render the window
    window->Render();
    //renderer->ResetCamera();

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
        int check = 0;
        vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);
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
    colorDroplist->addItem("Black");
    colorDroplist->addItem("White");
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
    shapesdroplist->addItem("Cone");
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
    /*vtkNew<vtkRenderer> renderer;*/
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
    QObject::connect(saveButton, &QPushButton::released, [&]() {
        ::Save(Lineactor, shapesdroplist);
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
    //double* mouse = style->GetMousePos();
    // Connect Change Shape 
    QObject::connect(changeshapes, &QPushButton::clicked, [=]() {
        Change_Shapes(shapesdroplist);
        });

    mainWindow.show();

    return app.exec(); // Run the Qt application event loop and return the exit code
}
