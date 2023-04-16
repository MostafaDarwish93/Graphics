#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLineSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkNamedColors.h>

#include <QApplication>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>

#include <cmath>
#include <cstdlib>
#include <random>
#include <qcombobox.h>
#include <vtkLineSource.h>
#include <QFileDialog> 
#include <QInputDialog>
#include <qmessagebox.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataWriter.h>
#include <vtkCoordinate.h>
#include <sstream>
using namespace std;

vtkNew<vtkRenderer> renderer;

/// Actor, Source, Mapper
vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
vtkDataSetMapper* mapper = vtkDataSetMapper::New();
vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();

/// Global Initialization
double Radius_Circle;
double Radius_Sphere;
double Radius_Arc;
double Radius_Reg_Polygon;
int NO_POINTS;
double Radius_Cylinder;
double Height_Cylinder;
double MAJOR_AXIS;
double MINOR_AXIS;
double Radius_Hexahedron;
double Radius_Square;
bool Poly_Line = 0;


namespace {
void DrawLine(vtkRenderer* renderer, vtkPoints* points);

void DrawPoly_Line(vtkRenderer* renderer, vtkPoints* points);

void Change_Shapes(QComboBox* comboBox, vtkGenericOpenGLRenderWindow* window);

void ChangeColor(QComboBox* comboBox, vtkGenericOpenGLRenderWindow* window);

void UpdateLineThickness(int thickness, vtkGenericOpenGLRenderWindow* window);

void Save(QComboBox* comboBox);

void Load();
} // namespace

namespace {
    class MouseInteractorStyleDrawLine : public vtkInteractorStyleTrackballCamera
    {
    public:
        static MouseInteractorStyleDrawLine* New();
        vtkTypeMacro(MouseInteractorStyleDrawLine, vtkInteractorStyleTrackballCamera);

        MouseInteractorStyleDrawLine()
        {
            this->Points = vtkSmartPointer<vtkPoints>::New();
            this->Picker = vtkSmartPointer<vtkPointPicker>::New();
        }

        virtual void OnLeftButtonDown() override
        {
            this->Picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1], 0, this->Renderer);
            double point[3];
            this->Picker->GetPickPosition(point);
            std::cout << "Point: " << point[0] << ", " << point[1] << ", " << point[2] << std::endl;
            this->Points->InsertNextPoint(point);

            // Draw the line
            if (this->Points->GetNumberOfPoints() > 1 && Poly_Line == false)
            {
                DrawLine(renderer, this->Points);
            }
            else {
                DrawPoly_Line(renderer, this->Points);
            }


            // Forward events
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }

        void SetRenderer(vtkRenderer* renderer)
        {
            this->Renderer = renderer;
        }

    private:
        vtkSmartPointer<vtkPoints> Points;
        vtkRenderer* Renderer;
        vtkSmartPointer<vtkPointPicker> Picker;
    };
    vtkStandardNewMacro(MouseInteractorStyleDrawLine);
} // namespace

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // main window
  QMainWindow mainWindow;
  mainWindow.setWindowTitle("Computer Graphics & Visualization");
  mainWindow.resize(1200, 900);

  // control area
  QDockWidget controlDock;
  mainWindow.addDockWidget(Qt::LeftDockWidgetArea, &controlDock);

  QLabel controlDockTitle("Control Dock");
  controlDockTitle.setMargin(20);
  controlDock.setTitleBarWidget(&controlDockTitle);

  QPointer<QVBoxLayout> dockLayout = new QVBoxLayout();
  QWidget layoutContainer;
  layoutContainer.setLayout(dockLayout);
  controlDock.setWidget(&layoutContainer);

  QPushButton randomizeButton;
  randomizeButton.setText("Randomize");
  dockLayout->addWidget(&randomizeButton);

  // Save Button 
  QPushButton saveButton;
  saveButton.setText("Save");
  dockLayout->addWidget(&saveButton);

  // Upload Button 
  QPushButton uploadButton;
  uploadButton.setText("Load");
  dockLayout->addWidget(&uploadButton);

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

  // Shapes droplist
  QComboBox* shapesdroplist = new QComboBox();
  shapesdroplist->addItem("Line");
  shapesdroplist->addItem("Polyline");
  shapesdroplist->addItem("Polygon");
  shapesdroplist->addItem("Regular Polygon");
  shapesdroplist->addItem("Circle");
  shapesdroplist->addItem("Arc");
  shapesdroplist->addItem("Ellipse");
  shapesdroplist->addItem("Cone");
  shapesdroplist->addItem("Cylinder");
  shapesdroplist->addItem("Hexahedron");
  shapesdroplist->addItem("Triangle Strip");
  shapesdroplist->addItem("Sphere");
  shapesdroplist->addItem("Square");
  shapesdroplist->addItem("Text");
  shapesdroplist->addItem("Rosette");
  shapesdroplist->setCurrentIndex(0); // Set default value
  dockLayout->addWidget(shapesdroplist);

  // render area
  QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget =
      new QVTKOpenGLNativeWidget();
  mainWindow.setCentralWidget(vtkRenderWidget);

  // VTK part
  vtkNew<vtkGenericOpenGLRenderWindow> window;
  vtkRenderWidget->setRenderWindow(window.Get());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetEdgeVisibility(true);
  actor->GetProperty()->SetRepresentationToSurface();

  renderer->AddActor(actor);

  window->AddRenderer(renderer);

  // Use a lambda function with capture by reference
  QObject::connect(changeshapes, &QPushButton::clicked,
      [=, &shapesdroplist, &window]() { Change_Shapes(shapesdroplist, window); });

  // Connect Change Color 
  QObject::connect(changeColorButton, &QPushButton::clicked,
      [=, &colorDroplist, &window]() { ChangeColor(colorDroplist, window); });

  // Connect Change Thickness
  QObject::connect(thicknessSlider, &QSlider::valueChanged, [&]() {
      int thickness = thicknessSlider->value();
      UpdateLineThickness(thickness, window);
      });

  // Connect save button
  QObject::connect(&saveButton, &QPushButton::released,
      [&]() {  ::Save(shapesdroplist); });

  // Connect upload button
  QObject::connect(&uploadButton, &QPushButton::released,
      [&]() {  ::Load(); });

  mainWindow.show();

  return app.exec();
}

namespace {
void Draw_circle(double radius, string color, int thickness)
{
    double R = radius; // Radius of the circle
    int numPoints = 100; // Number of points to approximate the circle

    lineSource->SetResolution(numPoints);
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Generate points on the circle using parametric equations
    for (int i = 0; i <= numPoints; i++)
    {
        double t = static_cast<double>(i) / numPoints;
        double x = R * cos(2 * vtkMath::Pi() * t);
        double y = R * sin(2 * vtkMath::Pi() * t);
        points->InsertNextPoint(x, y, 0.0); // Insert points on the circle
    }
    lineSource->SetPoints(points);

    // Update the mapper with the new data
    mapper->SetInputConnection(lineSource->GetOutputPort());
    mapper->Update();

    // Update the actor with the new mapper and properties based on user's choice
    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);
}

void Draw_Ellipse(double x_axis, double y_axis, string color, int thickness) {
    // Create an ellipse using parametric equations
    double A = x_axis; // Major axis length
    double B = y_axis; // Minor axis length
    int numPoints = 100; // Number of points to approximate the ellipse

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i <= numPoints; ++i) // Include the last point to complete the ellipse
    {
        double t = 2 * vtkMath::Pi() * static_cast<double>(i) / numPoints;
        double x = A * cos(t);
        double y = B * sin(t);
        points->InsertNextPoint(x, y, 0.0);
    }

    lineSource->SetPoints(points);

    mapper->SetInputConnection(lineSource->GetOutputPort());

    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);

}

void Draw_Regular_Polygon(double radius, int no_points, string color, int thickness) {
    // Define parameters for the regular polygon
    double R = radius; // Radius of the regular polygon
    int numPoints = no_points; // Number of points to approximate the regular polygon
    double angleIncrement = 2 * vtkMath::Pi() / numPoints; // Angle increment between consecutive points

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Generate points on the regular polygon using parametric equation
    for (int i = 0; i <= numPoints; i++)
    {
        double angle = i * angleIncrement;
        double x = R * cos(angle);
        double y = R * sin(angle);
        points->InsertNextPoint(x, y, 0.0); // Insert points on the regular polygon
    }

    // Set the points as the input points for the line source
    lineSource->SetPoints(points);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties
    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);
}

void Draw_Arc(double radius, string color, int thickness) {
    // Define parameters for the arc
    double R = radius; // Radius of the arc
    double startAngle = 1.0; // Start angle of the arc in radians
    double endAngle = vtkMath::Pi(); // End angle of the arc in radians
    int numPoints = 100; // Number of points to approximate the arc

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Generate points on the arc using parametric equation
    double angleIncrement = (endAngle - startAngle) / numPoints;
    for (int i = 0; i <= numPoints; i++)
    {
        double angle = startAngle + i * angleIncrement;
        double x = R * cos(angle);
        double y = R * sin(angle);
        points->InsertNextPoint(x, y, 0.0); // Insert points on the arc
    }

    // Set the points as the input points for the line source
    lineSource->SetPoints(points);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties
    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);

}

void Draw_Cylinder(double radius, double height, string color, int thickness) {
    // Define parameters for the cylinder
    double R = radius; // Radius of the cylinder
    double H = height; // Height of the cylinder
    int numPoints = 1000; // Number of points to approximate the cylinder

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Generate points on the cylinder using parametric equation
    double angleIncrement = 2 * vtkMath::Pi() / numPoints;
    for (int i = 0; i <= numPoints; i++)
    {
        double angle = i * angleIncrement;
        double x = R * cos(angle);
        double y = R * sin(angle);
        double z = (i % 2 == 0) ? H / 2.0 : -H / 2.0; // Alternate between positive and negative z values to create the top and bottom faces of the cylinder
        points->InsertNextPoint(x, y, z); // Insert points on the cylinder
    }

    // Set the points as the input points for the line source
    lineSource->SetPoints(points);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties
    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);

}

void Draw_Cone()
{
    // Set up the parameters for the cone
    double height = 2.0;
    double radius = 1.0;
    int numSegments = 100; // Number of segments to create the cone

    // Generate points for the cone using parametric equation
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i <= numSegments; ++i)
    {
        double angle = 2.0 * vtkMath::Pi() * i / numSegments;
        double x = radius * (1 - i / static_cast<double>(numSegments)) * std::cos(angle);
        double y = radius * (1 - i / static_cast<double>(numSegments)) * std::sin(angle);
        double z = height * i / static_cast<double>(numSegments);
        points->InsertNextPoint(x, y, z);
    }

    // Set the points as the input points for the line source
    lineSource->SetPoints(points);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color of the cone

    // Add the actor to the renderer
    renderer->AddActor(actor);
}

void Draw_Football(double radius, string color, int thickness) {
    // Define parameters for the sphere
    double R = radius; // Radius of the sphere
    int numPointsTheta = 100; // Number of points in theta direction
    int numPointsPhi = 50; // Number of points in phi direction

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Generate points on the sphere using parametric equation
    double thetaIncrement = vtkMath::Pi() / numPointsTheta;
    double phiIncrement = 2 * vtkMath::Pi() / numPointsPhi;
    for (int i = 0; i <= numPointsTheta; i++)
    {
        double theta = i * thetaIncrement;
        for (int j = 0; j <= numPointsPhi; j++)
        {
            double phi = j * phiIncrement;
            double x = R * sin(theta) * cos(phi);
            double y = R * sin(theta) * sin(phi);
            double z = R * cos(theta);
            points->InsertNextPoint(x, y, z); // Insert points on the sphere
        }
    }

    // Set the points as the input points for the line source
    lineSource->SetPoints(points);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties
    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);

}

void Draw_Square(double radius_square, string color, int thickness)
{
    // Define the center point of the hexahedron
    double center[3] = { 0.0, 0.0, 0.0 };

    // Define the radius from the center to each vertex
    double radius = 0.2;

    // Define the coordinates of the four vertices of the square
    double vertices[5][3] = { {center[0] - radius, center[1] - radius, center[2] + radius},
                              {center[0] - radius, center[1] + radius, center[2] + radius},
                              {center[0] + radius, center[1] + radius, center[2] + radius},
                              {center[0] + radius, center[1] - radius, center[2] + radius},
                              {center[0] - radius, center[1] - radius, center[2] + radius} };
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Insert the vertices as points in the points array
    for (int i = 0; i < 5; i++) // Update the loop to include the last point (to connect back to the first point)
    {
        points->InsertNextPoint(vertices[i]);
    }

    // Set the points as the input points for the line source
    lineSource->SetPoints(points);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties
    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(actor);
}

void Draw_Hexahedron(double radius_hex, string color, int thickness)
{
    // Define the center point of the hexahedron
    double center[3] = { 0.0, 0.0, 0.0 };

    // Define the radius from the center to each vertex
    double radius = radius_hex;

    // Calculate the coordinates of the vertices of the upper part of the hexahedron
    double upperVertices[5][3] = { {center[0] - radius, center[1] - radius, center[2] + radius},
                                   {center[0] - radius, center[1] + radius, center[2] + radius},
                                   {center[0] + radius, center[1] + radius, center[2] + radius},
                                   {center[0] + radius, center[1] - radius, center[2] + radius},
                                   {center[0] - radius, center[1] - radius, center[2] + radius} };

    // Define the coordinates of the vertices of the lower part of the hexahedron
    double lowerVertices[5][3] = { {center[0] - radius, center[1] - radius, center[2] - radius},
                                   {center[0] - radius, center[1] + radius, center[2] - radius},
                                   {center[0] + radius, center[1] + radius, center[2] - radius},
                                   {center[0] + radius, center[1] - radius, center[2] - radius},
                                   {center[0] - radius, center[1] - radius, center[2] - radius} };

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Insert the vertices of the upper part of the hexahedron as points in the points array
    for (int i = 0; i < 5; i++)
    {
        points->InsertNextPoint(upperVertices[i]);
    }

    // Insert the vertices of the lower part of the hexahedron as points in the points array
    for (int i = 0; i < 5; i++)
    {
        points->InsertNextPoint(lowerVertices[i]);
    }

    points->InsertNextPoint(upperVertices[1]);
    points->InsertNextPoint(lowerVertices[1]);
    points->InsertNextPoint(lowerVertices[2]);
    points->InsertNextPoint(upperVertices[2]);
    points->InsertNextPoint(upperVertices[3]);
    points->InsertNextPoint(lowerVertices[3]);
    points->InsertNextPoint(lowerVertices[4]);
    points->InsertNextPoint(upperVertices[4]);

    // Set the points and cells as the input data for the line source
    lineSource->SetPoints(points);
    //lineSource->SetCells(VTK_HEXAHEDRON, hexahedronCells);

    // Update the mapper with the line source output
    mapper->SetInputConnection(lineSource->GetOutputPort());

    // Update the actor with the mapper and properties

    actor->SetMapper(mapper);
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
    actor->GetProperty()->SetLineWidth(thickness);
    // Add the actor to the renderer
    renderer->AddActor(actor);
}

void Draw_Rosette() {
    //// Create the line source for the rosette
    //lineSource->SetPoint1(0.0, 0.0, 0.0); // Start point
    //lineSource->SetPoint2(1.0, 0.0, 0.0); // End point
    //lineSource->SetResolution(100); // Number of segments

    //// Create the mapper and actor
    //mapper->SetInputConnection(lineSource->GetOutputPort());
    //actor->SetMapper(mapper);
    //actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color to red
    //actor->GetProperty()->SetLineWidth(2.0); // Set line width

    //// Add the actor to the renderer
    //renderer->AddActor(actor);
}
void DrawLine(vtkRenderer* renderer, vtkPoints* points) {

    lineSource->SetPoint1(points->GetPoint(points->GetNumberOfPoints() - 2));
    lineSource->SetPoint2(points->GetPoint(points->GetNumberOfPoints() - 1));

    // Create a mapper and actor for the line

    mapper->SetInputConnection(lineSource->GetOutputPort());

    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    actor->GetProperty()->SetLineWidth(3.0);

    // Add the actor to the scene
    renderer->AddActor(actor);
}

void DrawPoly_Line(vtkRenderer* renderer, vtkPoints* points) {

}

void Save(QComboBox* comboBox){
    QString shape_name = comboBox->currentText();
    if (shape_name == "Circle") {
        double radius = Radius_Circle;
        //Get the color and thickness of the line
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\tRadius\tColor\tThickness" << std::endl;
                outputFile_txt << "Circle\t" << radius << "\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Color,Thickness" << std::endl;
                outputFile_csv << "Circle," << radius << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }

        }
    }
    else if (shape_name == "Sphere") {
        double radius = Radius_Sphere;
        //Get the color and thickness of the line
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\tRadius\tColor\tThickness" << std::endl;
                outputFile_txt << "Sphere\t" << radius << "\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Color,Thickness" << std::endl;
                outputFile_csv << "Sphere," << radius << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
    else if (shape_name == "Arc") {
        double radius = Radius_Arc;
        //Get the color and thickness of the line
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\tRadius\tColor\tThickness" << std::endl;
                outputFile_txt << "Arc\t" << radius << "\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Color,Thickness" << std::endl;
                outputFile_csv << "Arc," << radius << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
    else if (shape_name == "Hexahedron") {
        double radius = Radius_Hexahedron;
        //Get the color and thickness 
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\t\tRadius\tColor\tThickness" << std::endl;
                outputFile_txt << "Hexahedron\t" << radius << "\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");

            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Color,Thickness" << std::endl;
                outputFile_csv << "Hexahedron," << radius << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
    else if (shape_name == "Square") {
        double radius = Radius_Square;
        //Get the color and thickness 
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\tRadius\tColor\tThickness" << std::endl;
                outputFile_txt << "Square\t" << radius << "\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Color,Thickness" << std::endl;
                outputFile_csv << "Square," << radius << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
    else if (shape_name == "Regular Polygon") {
        double radius = Radius_Reg_Polygon;
        int number_points = NO_POINTS;
        //Get the color and thickness 
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\t\t\tRadius\tNumber of sides\tColor\tThickness" << std::endl;
                outputFile_txt << "Regular_Polygon\t" << "\t" << radius << "\t" << number_points << "\t\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Number of sides,Color,Thickness" << std::endl;
                outputFile_csv << "Regular Polygon," << radius << "," << number_points << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
    else if (shape_name == "Cylinder") {
        double radius = Radius_Cylinder;
        double height = Height_Cylinder;
        //Get the color and thickness of the line
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\t\tRadius\tHeight\tColor\tThickness" << std::endl;
                outputFile_txt << "Cylinder\t" << radius << "\t" << height << "\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Radius,Height,Color,Thickness" << std::endl;
                outputFile_csv << "Cylinder," << radius << "," << height << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
    else if (shape_name == "Ellipse") {
        double x_axis = MAJOR_AXIS;
        double y_axis = MINOR_AXIS;
        //Get the color and thickness of the line
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
        // Ask user for TXT or CSV
        QMessageBox messageBox;
        messageBox.setText("Choose Save Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("TXT"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("CSV"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        if (mode == "TXT") {
            QString filename_txt = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text files (*.txt)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_txt.isEmpty()) {
                // Open the TXT output file for writing
                std::ofstream outputFile_txt(filename_txt.toStdString());
                outputFile_txt << "Shape\tMajor Axis\tMinor Axis\tColor\tThickness" << std::endl;
                outputFile_txt << "Ellipse\t" << x_axis << "\t\t" << y_axis << "\t\t" << color_name << "\t" << thickness << std::endl;
                outputFile_txt.close();
            }
        }
        else {
            QString filename_csv = QFileDialog::getSaveFileName(nullptr, "Save File", "", "CSV files (*.csv)");
            // If the user didn't cancel the file dialogs, write to the output files
            if (!filename_csv.isEmpty()) {
                // Open the CSV output file for writing
                std::ofstream outputFile_csv(filename_csv.toStdString());
                outputFile_csv << "Shape,Major Axis,Minor Axis,Color,Thickness" << std::endl;
                outputFile_csv << "Ellipse," << x_axis << "," << y_axis << "," << color_name << "," << thickness << std::endl;
                outputFile_csv.close();
            }
        }
    }
}

void ChangeColor(QComboBox* comboBox, vtkGenericOpenGLRenderWindow* window) {
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
    else if (color_name == "Black")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color_name == "White")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    window->Render();
}

void Load()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Text files (*.txt)");

    if (!filename.isEmpty()) {
        // Open the input file for reading
        std::ifstream inputFile(filename.toStdString());
        if (!inputFile.is_open()) {
            std::cout << "Error opening file " << filename.toStdString() << std::endl;
            return;
        }

        // Read the first line (header)
        std::string line;
        std::getline(inputFile, line);

        // Read the second line (data)
        std::getline(inputFile, line);
        std::istringstream iss(line);
        std::string shape;
        double radius;
        double cylinder_height;
        double major_axis;
        double minor_axis;
        int no_of_sides;
        std::string color_name;
        double thickness;
        iss >> shape;
        if (shape == "Circle") {
            iss >> radius >> color_name >> thickness;
            Draw_circle(radius, color_name, thickness);
        }
        else if (shape == "Sphere") {
            iss >> radius >> color_name >> thickness;
            Draw_Football(radius, color_name, thickness);
        }
        else if (shape == "Arc") {
            iss >> radius >> color_name >> thickness;
            Draw_Arc(radius, color_name, thickness);
        }
        else if (shape == "Hexahedron") {
            iss >> radius >> color_name >> thickness;
            Draw_Hexahedron(radius, color_name, thickness);
        }
        else if (shape == "Regular_Polygon") {
            iss >> radius >> no_of_sides >> color_name >> thickness;
            Draw_Regular_Polygon(radius, no_of_sides, color_name, thickness);
        }
        else if (shape == "Square") {
            iss >> radius >> color_name >> thickness;
            Draw_Square(radius, color_name, thickness);
        }
        else if (shape == "Cylinder") {
            iss >> radius >> cylinder_height >> color_name >> thickness;
            Draw_Cylinder(radius, cylinder_height, color_name, thickness);
        }
        else if (shape == "Ellipse") {
            iss >> major_axis >> minor_axis >> color_name >> thickness;
            Draw_Ellipse(major_axis, minor_axis, color_name, thickness);
        }
        // Close the input file
        inputFile.close();
    }
}

void UpdateLineThickness(int thickness, vtkGenericOpenGLRenderWindow* window) {
    actor->GetProperty()->SetLineWidth(thickness);
    actor->GetMapper()->Update();
    window->Render();
}

void Change_Shapes(QComboBox* comboBox,
    vtkGenericOpenGLRenderWindow* window)
{
    QString shape_name = comboBox->currentText();

    if (shape_name == "Circle")
    {
        bool ok;
        Radius_Circle = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the circle:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_circle(Radius_Circle, "Red", 1.0);
    }
    else if (shape_name == "Sphere") {
        bool ok;
        Radius_Sphere = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Sphere:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Football(Radius_Sphere, "Red", 1.0);
    }
    else if (shape_name == "Arc")
    {
        bool ok;
        Radius_Arc = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Arc(Radius_Arc, "Red", 1.0);
    }
    else if (shape_name == "Hexahedron")
    {
        bool ok;
        Radius_Hexahedron = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Hexahedron:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Hexahedron(Radius_Hexahedron, "Red", 1.0);
    }
    else if (shape_name == "Line")
    {
        Poly_Line = false;
        vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
        renderWindowInteractor->SetRenderWindow(window);
        // Set the custom interactor style
        vtkNew<MouseInteractorStyleDrawLine> style;
        style->SetRenderer(renderer);

        renderWindowInteractor->SetInteractorStyle(style.Get());
        /*window->Render();
        renderWindowInteractor->Initialize();
        renderWindowInteractor->Start();*/
    }
    else if (shape_name == "Polyline") {
    }
    else if (shape_name == "Cone") {
        Draw_Cone();
    }
    else if (shape_name == "Regular Polygon")
    {
        bool ok;
        Radius_Reg_Polygon = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Regular Polygon:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        NO_POINTS = QInputDialog::getDouble(nullptr, "Enter Number of sides", "Enter number of sides of the Regular Polygon:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Regular_Polygon(Radius_Reg_Polygon, NO_POINTS, "Red", 1.0);
    }
    else if (shape_name == "Cylinder") {
        bool ok;
        Radius_Cylinder = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Cylinder:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Height_Cylinder = QInputDialog::getDouble(nullptr, "Enter Height", "Enter the height of the Cylinder:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Cylinder(Radius_Cylinder, Height_Cylinder, "Red", 1.0);
    }
    else if (shape_name == "Ellipse") {
        bool ok;
        MAJOR_AXIS = QInputDialog::getDouble(nullptr, "Enter Major Axis", "Enter the major axis (x) of the Ellipse:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        MINOR_AXIS = QInputDialog::getDouble(nullptr, "Enter Minor Axis", "Enter the minor axis (y) of the Ellipse:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Ellipse(MAJOR_AXIS, MINOR_AXIS, "Red", 1.0);
    }
    else if (shape_name == "Triangle Strip") {
    }
    else if (shape_name == "Text") {
    }
    else if (shape_name == "Square") {
        bool ok;
        Radius_Square = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Square:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_Square(Radius_Square, "Red", 1.0);
    }
    else if (shape_name == "Rosette") {
        Draw_Rosette();
    }
    window->Render();
}
} // namespace
