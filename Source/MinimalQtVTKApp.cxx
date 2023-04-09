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
#include <vtkCylinderSource.h>
#include <vtkHexahedron.h>
#include <vtkParametricEllipsoid.h>
#include <vtkTriangleStrip.h>

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
#include <vtkUnstructuredGrid.h>
#include <vtkArcSource.h>
#include <vtkParametricEllipsoid.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSuperEllipsoid.h>
#include <vtkTextureMapToSphere.h>
#include <vtkJPEGReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkArrowSource.h>
#include <vtkParametricSpline.h>
using namespace std;

double picked[3]; // Declares an array of 3 doubles called "picked"
int inc = 0;
bool circle_state = 0;
bool req_poly_state = 0;
bool ellip_state = 0;
bool cone_state = 0;
bool cylinder_state = 0;
bool Hexahedron_state = 0;
bool ellipse_state = 0;
bool square_state = 0;

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

// Create global variables for the Ellipse actor and mapper
vtkSmartPointer<vtkParametricEllipsoid> parametricEllipsoid = vtkSmartPointer<vtkParametricEllipsoid>::New();
vtkSmartPointer<vtkParametricFunctionSource> ellipsoidSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
vtkSmartPointer<vtkPolyDataMapper> ellipseMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
vtkSmartPointer<vtkActor> ellipseActor = vtkSmartPointer<vtkActor>::New();

// Create global variables for the Hexahedron actor 
vtkSmartPointer<vtkActor> Hexahedron_actor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkUnstructuredGrid> hexGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

// Create global variables for the A actor and mapper
vtkSmartPointer<vtkArcSource> arcSource = vtkSmartPointer<vtkArcSource>::New();
vtkSmartPointer<vtkPolyDataMapper> arcMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
vtkSmartPointer<vtkActor> arcActor = vtkSmartPointer<vtkActor>::New();

// Create global variables for the Cylinder actor and mapper
vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkProperty> cylinderProperty = vtkSmartPointer<vtkProperty>::New();

// Create global variables for the Cylinder actor and mapper
vtkSmartPointer<vtkActor> Trinangle_strip_actor = vtkSmartPointer<vtkActor>::New();
vtkNew<vtkPoints> tri_points;
vtkNew<vtkTriangleStrip> triangleStrip;

// Create global variables for the Ball actor and mapper
vtkSmartPointer<vtkActor> ball_actor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkSphereSource> ball_src = vtkSmartPointer<vtkSphereSource>::New();

// Text requirments
vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
vtkSmartPointer<vtkTextProperty> textProp = textActor->GetTextProperty();

//Square Requirments
vtkSmartPointer<vtkRegularPolygonSource> squareSource = vtkSmartPointer<vtkRegularPolygonSource>::New();
vtkSmartPointer<vtkActor> squareActor = vtkSmartPointer<vtkActor>::New();
vtkSmartPointer<vtkPolyDataMapper> squareMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

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
    //circleSource->GeneratePolygonOff();
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
        fillProperty->SetOpacity(1);
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

void Draw_Ellipse(double x, double y, double radiusX, double radiusY, string mode, string color, int thickness)
{
    // Set the position of the actor
    ellipseActor->SetPosition(x, y, 0);
    // Create a parametric ellipsoid
    parametricEllipsoid->SetXRadius(radiusX);
    parametricEllipsoid->SetYRadius(radiusY);
    parametricEllipsoid->SetZRadius(0);

    // Update the source with information
    ellipsoidSource->SetParametricFunction(parametricEllipsoid);
    ellipsoidSource->SetUResolution(50);
    ellipsoidSource->SetVResolution(50);
    ellipsoidSource->Update();

    // Update the mapper with the new data
    ellipseMapper->SetInputConnection(ellipsoidSource->GetOutputPort());
    ellipseMapper->Update();

    // Update the actor with the new mapper
    ellipseActor->SetMapper(ellipseMapper);

    // Update the actor with the new mapper and properties based on user's choice
    if (mode == "Filled") {
        ellipse_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        ellipse_state = 0;
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        ellipseActor->SetProperty(fillProperty);

        // Show only the edges
        ellipseActor->GetProperty()->EdgeVisibilityOn();
        ellipseActor->GetProperty()->BackfaceCullingOn();
    }
    if (color == "Red") {
        ellipseActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        ellipseActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        ellipseActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        ellipseActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        ellipseActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        ellipseActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        ellipseActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    ellipseActor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(ellipseActor);
}

void Draw_Line() {
    
}

void Draw_Arc(double x, double y, double px1, double py1, double px2, double py2, int reso, string color, int thickness) {
    // Create a new instance of vtkArcSource using smart pointers
    arcSource->SetCenter(x, y, 0);  // Set the center of the arc to the origin
    arcSource->SetPoint1(px1, py1, 0);  // Set the starting point of the arc
    arcSource->SetPoint2(px2, py2, 0);  // Set the ending point of the arc
    arcSource->SetResolution(reso);   // Set the number of points used to draw the arc
    arcSource->SetNegative(false);  // Set the arc to be drawn in the counterclockwise direction
    arcSource->Update();            // Update the arc source

    // Create a new instance of vtkPolyDataMapper using smart pointers
    vtkSmartPointer<vtkPolyDataMapper> arcMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    arcMapper->SetInputConnection(arcSource->GetOutputPort());  // Set the input connection to the arc source

    // Create a new instance of vtkActor using smart pointers
    arcActor->SetMapper(arcMapper);  // Set the mapper for the arc actor
    if (color == "Red") {
        arcActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        arcActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        arcActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        arcActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        arcActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        arcActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        arcActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    arcActor->GetProperty()->SetLineWidth(thickness);

    // Add the arc actor to the renderer
    renderer->AddActor(arcActor);

    // Render the scene
    window->Render();
}

void Draw_Hexahedron(string color, double thickness, string mode, double length)
{
    // Define the geometry of the hexahedron
    const double halfLength = length / 2.0;
    const double vertices[8][3] = { {-halfLength, -halfLength, -halfLength},
                                    { halfLength, -halfLength, -halfLength},
                                    { halfLength,  halfLength, -halfLength},
                                    {-halfLength,  halfLength, -halfLength},
                                    {-halfLength, -halfLength,  halfLength},
                                    { halfLength, -halfLength,  halfLength},
                                    { halfLength,  halfLength,  halfLength},
                                    {-halfLength,  halfLength,  halfLength} };
    const vtkIdType faces[6][4] = { {0, 1, 2, 3}, {0, 4, 5, 1}, {1, 5, 6, 2},
                                   {2, 6, 7, 3}, {3, 7, 4, 0}, {4, 7, 6, 5} };

    // Create a source object that generates the hexahedron
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < 8; ++i) {
        points->InsertNextPoint(vertices[i]);
    }
    for (int i = 0; i < 6; ++i) {
        vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();
        for (int j = 0; j < 4; ++j) {
            hex->GetPointIds()->SetId(j, faces[i][j]);
        }
        cells->InsertNextCell(hex);
    }
    hexGrid->SetPoints(points);
    hexGrid->SetReferenceCount(length);
    hexGrid->SetCells(VTK_HEXAHEDRON, cells);
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(hexGrid);

    // Create an actor to display the hexahedron in the renderer
    Hexahedron_actor->SetMapper(mapper);
    if (mode == "Filled") {
        Hexahedron_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        Hexahedron_actor->SetProperty(fillProperty);

        // Show only the edges
        Hexahedron_actor->GetProperty()->EdgeVisibilityOn();
        Hexahedron_actor->GetProperty()->BackfaceCullingOn();
        Hexahedron_state = 0;
    }
    if (color == "Red") {
        Hexahedron_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        Hexahedron_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        Hexahedron_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        Hexahedron_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        Hexahedron_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        Hexahedron_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        Hexahedron_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    Hexahedron_actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(Hexahedron_actor);
}

void Draw_Text(QString text, int fontSize, string color)
{
    // Set the text properties
    textProp->SetFontFamilyToArial();
    if (color == "Red") {
        textProp->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        textProp->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        textProp->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        textProp->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        textProp->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        textProp->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        textProp->SetColor(1.0, 1.0, 1.0);
    }
    textProp->SetFontSize(fontSize);

    // Set the text
    textActor->SetInput(text.toStdString().c_str());

    // Set the position of the text in the middle of the renderer window
    double windowCenter[2];
    renderer->GetRenderWindow()->GetSize();
    //renderer->SetViewport(0, 0, 1, 1);
    windowCenter[0] = renderer->GetCenter()[0];
    windowCenter[1] = renderer->GetCenter()[1];
    textActor->SetPosition(windowCenter[0] - 50, windowCenter[1]);

    // Add the text actor to the renderer
    renderer->AddActor(textActor);
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

void Draw_Football(int reso, double radius, double x, double y, string color, int thickness) {
    // Define football parameters
    double a = 1.0; // radius in x direction
    double b = 1.0; // radius in y direction
    double c = 0.75; // radius in z direction
    int resolution = reso; // number of points in each direction

    // Create a new vtkPoints object to hold the football vertices
    vtkNew<vtkPoints> points;

    // Generate the football vertices using a combination of spherical harmonics and trigonometric functions
    for (int i = 0; i <= resolution; i++) {
        double theta = i * M_PI / resolution;
        double sin_theta = sin(theta);
        double cos_theta = cos(theta);

        for (int j = 0; j <= resolution; j++) {
            double phi = j * 2 * M_PI / resolution;
            double sin_phi = sin(phi);
            double cos_phi = cos(phi);

            double x = a * sin_theta * cos_phi;
            double y = b * sin_theta * sin_phi;
            double z = c * cos_theta;

            double r = sqrt(x * x + y * y + z * z);

            // Scale the points to fit within the unit sphere
            x /= r;
            y /= r;
            z /= r;

            points->InsertNextPoint(x, y, z);
        }
    }

    // Create a new vtkPolyData object to hold the football geometry
    vtkNew<vtkPolyData> football;

    // Set the points for the vtkPolyData object
    football->SetPoints(points);

    // Create a new vtkSphereSource object to generate normals for the football surface
    ball_src->SetCenter(x, y, 0);
    ball_src->SetRadius(radius);
    ball_src->SetPhiResolution(resolution);
    ball_src->SetThetaResolution(resolution);

    // Compute the normals for the football surface
    vtkNew<vtkPolyDataNormals> normals;
    normals->SetInputData(football);
    normals->SetInputConnection(ball_src->GetOutputPort());

    // Create a new vtkPolyDataMapper object to map the football geometry to an actor
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(normals->GetOutputPort());

    // Create a new vtkActor object to represent the football
    ball_actor->SetMapper(mapper);
    if (color == "Red") {
        ball_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        ball_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        ball_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        ball_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        ball_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        ball_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        ball_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    ball_actor->GetProperty()->SetLineWidth(thickness);

    // Add the actor to the renderer
    renderer->AddActor(ball_actor);
}

void Draw_Cylinder(double x, double y, double height, double radius, int reso, string color, int thickness, string mode)
{
    // Create a new instance of vtkCylinderSource using smart pointers
    cylinderSource->SetCenter(x, y, 0);
    cylinderSource->SetHeight(height);
    cylinderSource->SetRadius(radius);
    cylinderSource->SetResolution(reso);

    // Create a new instance of vtkPolyDataMapper using smart pointers
    cylinderMapper->SetInputConnection(cylinderSource->GetOutputPort());
    cylinderMapper->Update();

    // Create a new instance of vtkActor using smart pointers
    cylinderActor->SetMapper(cylinderMapper);
    if (mode == "Filled") {
        cylinder_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        cylinderActor->SetProperty(fillProperty);

        // Show only the edges
        cylinderActor->GetProperty()->EdgeVisibilityOn();
        cylinderActor->GetProperty()->BackfaceCullingOn();
        cylinder_state = 0;
    }
    if (color == "Red") {
        cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        cylinderActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        cylinderActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        cylinderActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        cylinderActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        cylinderActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        cylinderActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    cylinderActor->GetProperty()->SetLineWidth(thickness);
    // Set the actor's properties
    cylinderProperty->SetDiffuse(0.7);
    cylinderProperty->SetSpecular(0.4);
    cylinderProperty->SetSpecularPower(20);
    cylinderActor->SetProperty(cylinderProperty);

    // Add the actor to the renderer
    renderer->AddActor(cylinderActor);
}

void Draw_TriangleStrip(string color, int thickness, double sideLength)
{
    tri_points->InsertNextPoint(0, 0, 0);
    tri_points->InsertNextPoint(0, sideLength, 0);
    tri_points->InsertNextPoint(sideLength, 0, 0);
    double hypotenuse = sqrt(2 * pow(sideLength, 2)); // Calculate length of hypotenuse
    tri_points->InsertNextPoint(sideLength + hypotenuse / 2, sideLength, 0); // Vertex opposite the hypotenuse

    triangleStrip->GetPointIds()->SetNumberOfIds(4);
    triangleStrip->GetPointIds()->SetId(0, 0);
    triangleStrip->GetPointIds()->SetId(1, 1);
    triangleStrip->GetPointIds()->SetId(2, 2);
    triangleStrip->GetPointIds()->SetId(3, 3);
    triangleStrip->SetReferenceCount(sideLength);

    vtkNew<vtkCellArray> cells;
    cells->InsertNextCell(triangleStrip);

    vtkNew<vtkPolyData> polydata;
    polydata->SetPoints(tri_points);
    polydata->SetStrips(cells);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(polydata);

    if (color == "Red") {
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        Trinangle_strip_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        Trinangle_strip_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        Trinangle_strip_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    Trinangle_strip_actor->GetProperty()->SetLineWidth(thickness);

    Trinangle_strip_actor->SetMapper(mapper);
    renderer->AddActor(Trinangle_strip_actor);
}

void Draw_Square(double x, double y, double radius, string mode, string color, int thickness) {
    // Create a regular polygon source with 4 sides (a square)
    squareSource->SetNumberOfSides(4);
    squareSource->SetRadius(radius); // adjust the size of the square as needed
    squareSource->SetCenter(x, y, 0); // set the center of the square to the origin

    // Create a mapper and actor for the square
    squareMapper->SetInputConnection(squareSource->GetOutputPort());

    squareActor->SetMapper(squareMapper);
    if (mode == "Filled") {
        square_state = 1;
    }
    else if (mode == "Non-Filled") {
        // Set fill color to background color and opacity to 0
        vtkSmartPointer<vtkProperty> fillProperty = vtkSmartPointer<vtkProperty>::New();
        fillProperty->SetColor(renderer->GetBackground());
        fillProperty->SetOpacity(1.0);
        squareActor->SetProperty(fillProperty);

        // Show only the edges
        squareActor->GetProperty()->EdgeVisibilityOn();
        squareActor->GetProperty()->BackfaceCullingOn();
        square_state = 0;
    }
    if (color == "Red") {
        squareActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color == "Blue") {
        squareActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color == "Yellow") {
        squareActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color == "Green") {
        squareActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color == "Magenta") {
        squareActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color == "Black") {
        squareActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color == "White") {
        squareActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    squareActor->GetProperty()->SetLineWidth(thickness);

    // Add the square actor to the renderer
    renderer->AddActor(squareActor);
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
        cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        Hexahedron_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        arcActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        ellipseActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        ball_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        textProp->SetColor(1.0, 0.0, 0.0);
        squareActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    }
    else if (color_name == "Green")
    {
        actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Circle_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        coneActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        cylinderActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Hexahedron_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        arcActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        ellipseActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        Trinangle_strip_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        ball_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        textProp->SetColor(0.0, 1.0, 0.0);
        squareActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    }
    else if (color_name == "Blue")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        coneActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        cylinderActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Hexahedron_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        arcActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        ellipseActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        Trinangle_strip_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        ball_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
        textProp->SetColor(0.0, 0.0, 1.0);
        squareActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    }
    else if (color_name == "Yellow")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        coneActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        cylinderActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Hexahedron_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        arcActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        ellipseActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        ball_actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
        textProp->SetColor(1.0, 1.0, 0.0);
        squareActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
    }
    else if (color_name == "Magenta")
    {
        actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Circle_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        coneActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        cylinderActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Hexahedron_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        arcActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        ellipseActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        ball_actor->GetProperty()->SetColor(1.0, 0.0, 1.0);
        textProp->SetColor(1.0, 0.0, 1.0);
        squareActor->GetProperty()->SetColor(1.0, 0.0, 1.0);
    }
    else if (color_name == "Black")
    {
        actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        Circle_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        Polygon_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        coneActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        cylinderActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        Hexahedron_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        arcActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        ellipseActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        Trinangle_strip_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        ball_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
        textProp->SetColor(0.0, 0.0, 0.0);
        squareActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    }
    else if (color_name == "White")
    {
        actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        Circle_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        Polygon_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        coneActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        cylinderActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        Hexahedron_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        arcActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        ellipseActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        Trinangle_strip_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        ball_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        textProp->SetColor(1.0, 1.0, 1.0);
        squareActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    }
    window->Render();
}

void UpdateLineThickness(int thickness, vtkActor* actor) {
    actor->GetProperty()->SetLineWidth(thickness);
    Circle_actor->GetProperty()->SetLineWidth(thickness);
    Polygon_actor->GetProperty()->SetLineWidth(thickness);
    coneActor->GetProperty()->SetLineWidth(thickness);
    cylinderActor->GetProperty()->SetLineWidth(thickness);
    Hexahedron_actor->GetProperty()->SetLineWidth(thickness);
    arcActor->GetProperty()->SetLineWidth(thickness);
    ellipseActor->GetProperty()->SetLineWidth(thickness);
    Trinangle_strip_actor->GetProperty()->SetLineWidth(thickness);
    ball_actor->GetProperty()->SetLineWidth(thickness);
    textProp->SetFontSize(thickness);
    squareActor->GetProperty()->SetLineWidth(thickness);
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
    else if (shape_name == "Cylinder") {
        // Get the start and end points of the line
        double centerX = cylinderSource->GetCenter()[0];
        double centerY = cylinderSource->GetCenter()[1];
        double radius = cylinderSource->GetRadius();
        int reso = cylinderSource->GetResolution();
        int height = cylinderSource->GetHeight();

        // Get the color and thickness of the line
        double* color = cylinderActor->GetProperty()->GetColor();
        double thickness = cylinderActor->GetProperty()->GetLineWidth();
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
            outputFile << "Cylinder" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Radius: " << radius << std::endl;
            outputFile << "Height: " << height << std::endl;
            outputFile << "Resolution: " << reso << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            if (cylinder_state == 1) {
                outputFile << "Cylinder State: " << "Filled" << std::endl;
            }
            else {
                outputFile << "Cylinder State: " << "Non-Filled" << std::endl;

            }
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Hexahedron") {
        // Define the geometry of the hexahedron
        double length = hexGrid->GetReferenceCount();
        double vertices[8][3] = { {-length / 2.0, -length / 2.0, -length / 2.0},
                              { length / 2.0, -length / 2.0, -length / 2.0},
                              { length / 2.0,  length / 2.0, -length / 2.0},
                              {-length / 2.0,  length / 2.0, -length / 2.0},
                              {-length / 2.0, -length / 2.0,  length / 2.0},
                              { length / 2.0, -length / 2.0,  length / 2.0},
                              { length / 2.0,  length / 2.0,  length / 2.0},
                              {-length / 2.0,  length / 2.0,  length / 2.0} };
        const vtkIdType faces[6][4] = { {0, 1, 2, 3}, {0, 4, 5, 1}, {1, 5, 6, 2},
                                   {2, 6, 7, 3}, {3, 7, 4, 0}, {4, 7, 6, 5} };

        // Get the color and thickness of the line
        double* color = Hexahedron_actor->GetProperty()->GetColor();
        double thickness = Hexahedron_actor->GetProperty()->GetLineWidth();
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

            // Write the vertices and faces to the output file
            outputFile << "Hexahedron" << std::endl;
            outputFile << "The length of the sides of Hexahedron is: " << length << std::endl;;
            for (int i = 0; i < 8; ++i) {
                outputFile << "Vertex " << i << ": " << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << std::endl;
            }
            for (int i = 0; i < 6; ++i) {
                outputFile << "Face " << i << ": " << faces[i][0] << " " << faces[i][1] << " " << faces[i][2] << " " << faces[i][3] << std::endl;
            }
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            outputFile.close();
        }
    }
    else if (shape_name == "Arc") {
        // Get the start and end points of the line
        double centerX = arcSource->GetCenter()[0];
        double centerY = arcSource->GetCenter()[1];
        double point1X = arcSource->GetPoint1()[0];
        double point1Y = arcSource->GetPoint1()[1];
        double point2X = arcSource->GetPoint2()[0];
        double point2Y = arcSource->GetPoint2()[1];
        int reso = arcSource->GetResolution();

        // Get the color and thickness of the line
        double* color = arcActor->GetProperty()->GetColor();
        double thickness = arcActor->GetProperty()->GetLineWidth();
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
            outputFile << "Arc" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Point 1: " << point1X << ", " << point1Y << std::endl;
            outputFile << "Point 2: " << point2X << ", " << point2Y << std::endl;
            outputFile << "Resolution: " << reso << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Ellipse") {
        // Get the input parameters of the ellipse
        float centerX = ellipseActor->GetCenter()[0];
        double centerY = ellipseActor->GetCenter()[1];
        double radiusX = parametricEllipsoid->GetXRadius();
        double radiusY = parametricEllipsoid->GetYRadius();

        // Get the color and thickness of the line
        double* color = ellipseActor->GetProperty()->GetColor();
        double thickness = ellipseActor->GetProperty()->GetLineWidth();
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
            outputFile << "Ellipse" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "RadiusX: " << radiusX << std::endl;
            outputFile << "RadiusY: " << radiusY << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            if (ellipse_state == 1) {
                outputFile << "Ellipse State: " << "Filled" << std::endl;
            }
            else {
                outputFile << "Ellipse State: " << "Non-Filled" << std::endl;
            }
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Ball"){
        // Get the start and end points of the line
        double centerX = ball_src->GetCenter()[0];
        double centerY = ball_src->GetCenter()[1];
        double radius = ball_src->GetRadius();
        int reso = ball_src->GetPhiResolution();

        // Get the color and thickness of the line
        double* color = ball_actor->GetProperty()->GetColor();
        double thickness = ball_actor->GetProperty()->GetLineWidth();
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
            outputFile << "Ball" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Radius: " << radius << std::endl;
            outputFile << "Resolution: " << reso << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Text") {
        // Get the start and end points of the line
        QString text = textActor->GetInput();

        // Get the color and Font Size of the Text
        double* color = textProp->GetColor();
        int font_size = textProp->GetFontSize();
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
            outputFile << "Text" << std::endl;
            outputFile << "Text Input: " << text.toStdString() << std::endl;
            outputFile << "Font Size: " << font_size << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Triangle Strip") {
        double length = triangleStrip->GetReferenceCount();

        // Get the color and thickness of the line
        double* color = Trinangle_strip_actor->GetProperty()->GetColor();
        double thickness = Trinangle_strip_actor->GetProperty()->GetLineWidth();
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
            outputFile << "Triangle Strip" << std::endl;
            outputFile << "Side length: " << length << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            // Close the output file
            outputFile.close();
        }
    }
    else if (shape_name == "Square") {
        // Get the start and end points of the Square
        double centerX = squareSource->GetCenter()[0];
        double centerY = squareSource->GetCenter()[1];
        double radius = squareSource->GetRadius();

        // Get the color and thickness of the Square
        double* color = squareActor->GetProperty()->GetColor();
        double thickness = squareActor->GetProperty()->GetLineWidth();
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
            outputFile << "Square" << std::endl;
            outputFile << "Center point: " << centerX << ", " << centerY << std::endl;
            outputFile << "Radius: " << radius << std::endl;
            outputFile << "Color: " << color_name << std::endl;
            outputFile << "Thickness: " << thickness << std::endl;
            if (square_state == 1) {
                outputFile << "Square State: " << "Filled" << std::endl;
            }
            else {
                outputFile << "Square State: " << "Non-Filled" << std::endl;
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
    else if (line.startsWith("Cylinder")) {
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
            else if (line.startsWith("Cylinder State:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_Cylinder(x, y, height, radius, reso, color, thickness, state);
    }
    else if (line.startsWith("Hexahedron")) {
        !in.atEnd();
        string color;
        int thickness;
        string state;
        double length;
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the line contains the color information
            if (line.startsWith("Color:")) {
                // Extract the color values from the line and set the color of the actor based on the color name
                QStringList colorValues = line.section(":", 1).trimmed().split(" ");
                color = colorValues.at(0).toStdString();
            }
            else if (line.startsWith("The length of the sides of Hexahedron is:")) {
                length = line.section(":", 1).trimmed().toDouble();

            }
            // Check if the line contains the Thickness information
            else if (line.startsWith("Thickness:")) {
                // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
                thickness = line.section(":", 1).trimmed().toInt();

            }
            // Check if the circle is filled
            else if (line.startsWith("Cylinder State:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_Hexahedron(color, thickness, state, length);
    }
    else if (line.startsWith("Arc")) {
        !in.atEnd();
        double x, y, px1, py1, px2, py2;
        string color;
        int thickness, reso;
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the line contains the center point coordinates
            if (line.startsWith("Center point:")) {
                // Extract the coordinates from the line and set the start point of the line source
                QStringList coords = line.section(":", 1).split(",");
                x = coords[0].toDouble();
                y = coords[1].toDouble();
            }
            else if (line.startsWith("Point 1:")) {
                // Extract the coordinates from the line and set the start point of the line source
                QStringList coords = line.section(":", 1).split(",");
                px1 = coords[0].toDouble();
                py1 = coords[1].toDouble();
            }
            else if (line.startsWith("Point 2:")) {
                // Extract the coordinates from the line and set the start point of the line source
                QStringList coords = line.section(":", 1).split(",");
                px2 = coords[0].toDouble();
                py2 = coords[1].toDouble();
            }
            else if (line.startsWith("Resolution:")) {
                // Extract the Thickness integer for the line and set the Thickness of the actor based on the Thickness
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
        }
        Draw_Arc(x, y, px1, py1, px2, py2, reso, color, thickness);
    }
    else if (line.startsWith("Ellipse")) {
        !in.atEnd();
        double x, y, radiusX, radiusY;
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
            else if (line.startsWith("RadiusX:")) {
                // Extract the radius value from the line and set the radius of the ellipse source
                radiusX = line.section(":", 1).trimmed().toDouble();

            }
            else if (line.startsWith("RadiusY:")) {
                radiusY = line.section(":", 1).trimmed().toDouble();
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
            else if (line.startsWith("Ellipse State:")) {
                // Extract the ellipse state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_Ellipse(x, y, radiusX, radiusY, state, color, thickness);
        }
    else if (line.startsWith("Ball")) {
        !in.atEnd();
        double x, y, radius;
        string color;
        int thickness, reso;
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
            else if (line.startsWith("Resolution:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                reso = line.section(":", 1).trimmed().toInt();
            }
        }
        Draw_Football(reso, radius, x, y, color, thickness);
    }
    else if (line.startsWith("Text")) {
        !in.atEnd();
        QString text_input;
        int font_size = 0;
        string color = "";
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the Text has a text
            if (line.startsWith("Text Input:")) {
                QStringList text = line.section(":", 1).trimmed().split(" ");
                text_input = QString::fromStdString(text.at(0).toStdString());
            }
            // Check if the line contains the color
            else if (line.startsWith("Color:")) {
                QStringList colorValues = line.section(":", 1).trimmed().split(" ");
                color = colorValues.at(0).toStdString();
            }
            // Check if the line contains the font size
            else if (line.startsWith("Font Size:")) {
                font_size = line.section(":", 1).trimmed().toInt();
            }
        }
        Draw_Text(text_input, font_size, color);
    }
    else if (line.startsWith("Triangle Strip")) {
        !in.atEnd();
        double length;
        string color;
        int thickness;
        while (!in.atEnd()) {
            line = in.readLine();
            // Check if the line contains the center point coordinates
            if (line.startsWith("Side length:")) {
                length = line.section(":", 1).trimmed().toDouble();
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
        }
        Draw_TriangleStrip(color , thickness, length);
    }
    else if (line.startsWith("Square")) {
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
            else if (line.startsWith("Square State:")) {
                // Extract the circle state value from the line and set the actor state based on the circle state
                state = line.section(":", 1).trimmed().toStdString();
            }
        }
        Draw_Square(x, y, radius, state, color, thickness);
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
    else if (shape_name == "Ball") {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the Ball:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the Ball:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double radius = QInputDialog::getDouble(nullptr, "Enter the radius", "Enter the radius of the Ball:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        int reso = QInputDialog::getDouble(nullptr, "Enter Resolution", "Enter the Resolution of the Ball : ", 3, 3, 100, 1, &ok);
        if (!ok) {
            return;
        }
        Draw_Football(reso, radius, x, y, "White", 1.0);
    }
    else if (shape_name == "Arc")
    {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double px1 = QInputDialog::getDouble(nullptr, "Enter Point 1", "Enter the x coordinate of the point 1 of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double py1 = QInputDialog::getDouble(nullptr, "Enter Point 1", "Enter the y coordinate of the point 1 of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double px2 = QInputDialog::getDouble(nullptr, "Enter Point 2", "Enter the x coordinate of the point 2 of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double py2 = QInputDialog::getDouble(nullptr, "Enter Point 2", "Enter the y coordinate of the point 2 of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        int reso = QInputDialog::getDouble(nullptr, "Enter Resolution", "Enter the Resolution of the Arc : ", 3, 3, 100, 1, &ok);
        if (!ok) {
            return;
        }
        Draw_Arc(x, y, px1, py1, px2, py2, reso, "Red", 1.0);
    }
    else if (shape_name == "Hexahedron")
    {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double len = QInputDialog::getDouble(nullptr, "Enter the length", "Enter the length of the each side of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        QMessageBox messageBox;
        messageBox.setText("Choose Region Type");
        QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("Filled"), QMessageBox::YesRole);
        QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("Non-Filled"), QMessageBox::YesRole);
        messageBox.exec();
        QString buttonText = messageBox.clickedButton()->text();
        std::string mode = buttonText.toStdString();
        Draw_Hexahedron("Red", 1.0, mode, len);
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
    else if (shape_name == "Cylinder") {
        bool ok;
        // Get the center of the Regular Polygon from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the Cylinder:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the Cylinder:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double Height = QInputDialog::getDouble(nullptr, "Enter Height", "Enter the Height of the Cylinder:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        int resoultion = QInputDialog::getInt(nullptr, "Enter Resolution", "Enter the Resolution of the Cylinder:", 3, 3, 100, 1, &ok);
        if (!ok) {
            return;
        }
        // Prompt the user for the radius of the Cone
        double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Cylinder:", 1.0, 0.0, 100.0, 2, &ok);
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
        Draw_Cylinder(x, y, Height, radius, resoultion, "Red", 1.0, mode);
    }
    else if (shape_name == "Ellipse") {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the Ellipse:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the Ellipse:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        // Get the radius of the circle from the user using a QInputDialog
        double radiusX = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radiusX of the Ellipse:", 1.0, 0.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double radiusY = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radiusY of the Ellipse:", 1.0, 0.0, 100.0, 2, &ok);
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
        Draw_Ellipse(x, y, radiusX, radiusY, mode, "Red", 1.0);
    }
    else if (shape_name == "Triangle Strip") {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double len = QInputDialog::getDouble(nullptr, "Enter the length", "Enter the length of the each side of the Triangle Strip:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        Draw_TriangleStrip("Red", 1.0, len);
    }
    else if (shape_name == "Text") {
        // Prompt user for text and font size
        QString text = QInputDialog::getText(nullptr, "Text Input", "Enter text:");
        bool ok;
        int fontSize = QInputDialog::getInt(nullptr, "Font Size Input", "Enter font size:", 24, 1, 100, 1, &ok);
        if (!ok) return; // User cancelled input dialog
        Draw_Text(text, fontSize, "Red");
    }
    else if (shape_name == "Square") {
        bool ok;
        // Get the center of the circle from the user using a QInputDialog
        double x = QInputDialog::getDouble(nullptr, "Enter Center X", "Enter the x coordinate of the center of the Square:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        double y = QInputDialog::getDouble(nullptr, "Enter Center Y", "Enter the y coordinate of the center of the Square:", 0.0, -100.0, 100.0, 2, &ok);
        if (!ok) {
            return;
        }
        // Get the radius of the circle from the user using a QInputDialog
        double radius = QInputDialog::getDouble(nullptr, "Enter Radius", "Enter the radius of the Square:", 1.0, 0.0, 100.0, 2, &ok);
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
        Draw_Square(x, y, radius, mode, "White", 1.0);
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
    shapesdroplist->addItem("Ball");
    shapesdroplist->addItem("Square");
    shapesdroplist->addItem("Text");
    shapesdroplist->setCurrentIndex(0); // Set default value
    dockLayout->addWidget(shapesdroplist);

    //// Font text box
    //QLineEdit* fontLineEdit = new QLineEdit();
    //dockLayout->addWidget(fontLineEdit);

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
    //window->GetInteractor()->SetInteractorStyle(style);
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
