#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
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
using namespace std;

vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

/// Actor, Source, Mapper
vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
vtkDataSetMapper* mapper = vtkDataSetMapper::New();
vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();



namespace {
    /**
     * Deform the sphere source using a random amplitude and modes and render it in
     * the window
     *
     * @param sphere the original sphere source
     * @param mapper the mapper for the scene
     * @param window the window to render to
     * @param randEng the random number generator engine
     */
    void Randomize(vtkSphereSource* sphere, vtkMapper* mapper,
        vtkGenericOpenGLRenderWindow* window, std::mt19937& randEng);
   // void DrawLine(vtkRenderer* renderer, int startX, int startY, int endX, int endY);
    void DrawLine(vtkRenderer* renderer, vtkPoints* points, vtkNamedColors* colors);
    void DrawPolyLine(vtkSmartPointer<vtkActor> Poly_Line_Actor, vtkDataSetMapper* Poly_Line_mapper, vtkSmartPointer<vtkLineSource> Poly_Line_Source, vtkSmartPointer<vtkRenderer> Poly_Line_renderer, vtkSmartPointer<vtkPoints> points);
    void Change_Shapes(QComboBox* comboBox,
        vtkGenericOpenGLRenderWindow* window);
} // namespace

//_____________________________________________________________ Draw Line Funtion ___________________________________________________________//
namespace {
    class MouseInteractorStyleDrawLine : public vtkInteractorStyleTrackballCamera
    {
    public:
        static MouseInteractorStyleDrawLine* New();
        vtkTypeMacro(MouseInteractorStyleDrawLine, vtkInteractorStyleTrackballCamera);

        MouseInteractorStyleDrawLine()
        {
            this->Points = vtkSmartPointer<vtkPoints>::New();
            this->Color = vtkSmartPointer<vtkNamedColors>::New();
            this->Picker = vtkSmartPointer<vtkPointPicker>::New();
            this->flag = true;
            this->numofpoints = NULL;
        }

        virtual void OnLeftButtonDown() override
        {
            if (this->flag == true || this->Polyflag == true) {
                this->Picker->Pick(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1], 0, this->Renderer);
                double point[3];
                this->Picker->GetPickPosition(point);
                std::cout << "Point: " << point[0] << ", " << point[1] << ", " << point[2] << std::endl;
                this->Points->InsertNextPoint(point);

                // Draw the line
                if (this->Points->GetNumberOfPoints() > 2 && this->flag == true)
                {
                    DrawLine(renderer, this->Points, this->Color);
                }
                else if (this->Points->GetNumberOfPoints() > 2 && this->Polyflag == true) {
                    vtkSmartPointer<vtkActor> PolyLineActor = actor;
                    vtkDataSetMapper* PolyLineMapper = mapper;
                    vtkSmartPointer<vtkLineSource> PolyLineSource = lineSource;
                    vtkSmartPointer<vtkRenderer>PolyLineRenderer = renderer;
                    DrawPolyLine(PolyLineActor, PolyLineMapper, PolyLineSource, PolyLineRenderer, Points);
                    //DrawLine(renderer, this->Points, this->Color);
                   // Points->InsertNextPoint(point); // insert the first point again
                   // DrawLine(renderer, this->Points, this->Color);
                }
            }

            // Forward events
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }

        void SetColor(vtkSmartPointer<vtkNamedColors> color)
        {
            this->Color = color;
        }

        void SetRenderer(vtkRenderer* renderer)
        {
            this->Renderer = renderer;
        }
        void setFlag(bool f) {
            this->flag = f;
        }
        bool getFLag() {
            return flag;
        }
        void setPolyFlag(bool f) {
            Polyflag = f;
        }

    private:
        vtkSmartPointer<vtkPoints> Points;
        vtkSmartPointer<vtkNamedColors> Color;
        vtkRenderer* Renderer;
        vtkSmartPointer<vtkPointPicker> Picker;
        bool flag;
        bool Polyflag;
        int numofpoints;
    };
    vtkStandardNewMacro(MouseInteractorStyleDrawLine);
}

namespace {
    void Randomize(vtkSphereSource* sphere, vtkMapper* mapper,
        vtkGenericOpenGLRenderWindow* window, std::mt19937& randEng)
    {
        // generate randomness
        double randAmp = 0.2 + ((randEng() % 1000) / 1000.0) * 0.2;
        double randThetaFreq = 1.0 + (randEng() % 9);
        double randPhiFreq = 1.0 + (randEng() % 9);

        // extract and prepare data
        sphere->Update();
        vtkSmartPointer<vtkPolyData> newSphere;
        newSphere.TakeReference(sphere->GetOutput()->NewInstance());
        newSphere->DeepCopy(sphere->GetOutput());
        vtkNew<vtkDoubleArray> height;
        height->SetName("Height");
        height->SetNumberOfComponents(1);
        height->SetNumberOfTuples(newSphere->GetNumberOfPoints());
        newSphere->GetPointData()->AddArray(height);

        // deform the sphere
        for (int iP = 0; iP < newSphere->GetNumberOfPoints(); iP++)
        {
            double pt[3] = { 0.0 };
            newSphere->GetPoint(iP, pt);
            double theta = std::atan2(pt[1], pt[0]);
            double phi =
                std::atan2(pt[2], std::sqrt(std::pow(pt[0], 2) + std::pow(pt[1], 2)));
            double thisAmp =
                randAmp * std::cos(randThetaFreq * theta) * std::sin(randPhiFreq * phi);
            height->SetValue(iP, thisAmp);
            pt[0] += thisAmp * std::cos(theta) * std::cos(phi);
            pt[1] += thisAmp * std::sin(theta) * std::cos(phi);
            pt[2] += thisAmp * std::sin(phi);
            newSphere->GetPoints()->SetPoint(iP, pt);
        }
        newSphere->GetPointData()->SetScalars(height);

        // reconfigure the pipeline to take the new deformed sphere
        mapper->SetInputDataObject(newSphere);
        mapper->SetScalarModeToUsePointData();
        mapper->ColorByArrayComponent("Height", 0);
        window->Render();
    }

    
    void DrawPolyLine(vtkSmartPointer<vtkActor> Poly_Line_Actor, vtkDataSetMapper* Poly_Line_mapper, vtkSmartPointer<vtkLineSource> Poly_Line_Source, vtkSmartPointer<vtkRenderer> Poly_Line_renderer, vtkSmartPointer<vtkPoints> points) {
        
        Poly_Line_Source->SetPoints(points);
        Poly_Line_mapper->SetInputConnection(Poly_Line_Source->GetOutputPort());
        Poly_Line_mapper->Update();
        Poly_Line_Actor->SetMapper(Poly_Line_mapper);
        Poly_Line_Actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        Poly_Line_Actor->GetProperty()->SetLineWidth(3.0);

        // Add the actor to the scene
        Poly_Line_renderer->AddActor(Poly_Line_Actor);
        
    }
    void Draw_circle(vtkSmartPointer<vtkActor> actor, vtkDataSetMapper* mapper, vtkSmartPointer<vtkLineSource> lineSource, vtkSmartPointer<vtkRenderer> renderer)
    {
       

        double R = 5.0; // Radius of the circle
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
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color of the circle
        actor->GetProperty()->SetLineWidth(2.0); // Set line width of the circle

        // Add the actor to the renderer
        renderer->AddActor(actor);
    }

    void Draw_Ellipse() {
        // Create an ellipse using parametric equations
        double A = 5.0; // Major axis length
        double B = 3.0; // Minor axis length
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
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color of the ellipse
        actor->GetProperty()->SetLineWidth(2.0); // Set line width of the ellipse

        // Add the actor to the renderer
        renderer->AddActor(actor);

    }

    void Draw_Regular_Polygon() {
        // Define parameters for the regular polygon
        double R = 5.0; // Radius of the regular polygon
        int numPoints = 6; // Number of points to approximate the regular polygon
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
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color of the regular polygon
        actor->GetProperty()->SetLineWidth(2.0); // Set line width of the regular polygon

        // Add the actor to the renderer
        renderer->AddActor(actor);
    }
    void DrawLine(vtkRenderer* Renderer, vtkPoints* points, vtkNamedColors* colors) {

       lineSource->SetPoint1(points->GetPoint(points->GetNumberOfPoints() - 2));
       lineSource->SetPoint2(points->GetPoint(points->GetNumberOfPoints() - 1));
        //lineSource->SetPoints(points);
        // Create a mapper and actor for the line

        mapper->SetInputConnection(lineSource->GetOutputPort());
        mapper->Update();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        actor->GetProperty()->SetLineWidth(3.0);

        // Add the actor to the scene
        renderer->AddActor(actor);
    }

    void Draw_Arc() {
        // Define parameters for the arc
        double R = 5.0; // Radius of the arc
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
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color of the arc
        actor->GetProperty()->SetLineWidth(2.0); // Set line width of the arc

        // Add the actor to the renderer
        renderer->AddActor(actor);

    }

    void Draw_Cylinder() {
        // Define parameters for the cylinder
        double R = 5.0; // Radius of the cylinder
        double H = 10.0; // Height of the cylinder
        int numPoints = 10000; // Number of points to approximate the cylinder

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
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color of the cylinder
        actor->GetProperty()->SetLineWidth(2.0); // Set line width of the cylinder

        // Add the actor to the renderer
        renderer->AddActor(actor);

    }

    void Draw_Cone() {
    }

    void Draw_Football() {
        // Define parameters for the sphere
        double R = 1.0; // Radius of the sphere
        int numPointsTheta = 100; // Number of points in theta direction
        int numPointsPhi = 100; // Number of points in phi direction

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
        actor->GetProperty()->SetColor(0.0, 1.0, 0.0); // Set color of the sphere

        // Add the actor to the renderer
        renderer->AddActor(actor);

    }

    void Draw_Square() {

    }
    //void DrawLine(vtkRenderer* renderer, int startX, int startY, int endX, int endY)
    //{
    //    // Convert start and end points from window coordinates to normalized device coordinates (NDC)
    //    double startXNDC = (double)startX / renderer->GetRenderWindow()->GetSize()[0] * 2.0 - 1.0;
    //    double startYNDC = (double)startY / renderer->GetRenderWindow()->GetSize()[1] * 2.0 - 1.0;
    //    double endXNDC = (double)endX / renderer->GetRenderWindow()->GetSize()[0] * 2.0 - 1.0;
    //    double endYNDC = (double)endY / renderer->GetRenderWindow()->GetSize()[1] * 2.0 - 1.0;
    //
    //    // Create points for the start and end points
    //    vtkNew<vtkPoints> points;
    //    points->InsertNextPoint(startXNDC, startYNDC, 0.0);
    //    points->InsertNextPoint(endXNDC, endYNDC, 0.0);
    //
    //    // Create a line source with the points
    //    vtkNew<vtkLineSource> lineSource;
    //    lineSource->SetPoints(points);
    //
    //    // Update the line source to compute the parametric coordinates
    //    lineSource->Update();
    //
    //    // Create a mapper for the line
    //    vtkNew<vtkPolyDataMapper> mapper;
    //    mapper->SetInputConnection(lineSource->GetOutputPort());
    //
    //    // Create an actor for the line
    //    vtkNew<vtkActor> actor;
    //    actor->SetMapper(mapper);
    //    actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set line color to red
    //
    //    // Add the actor to the renderer
    //    renderer->AddActor(actor);
    //
    //    // Render the scene
    //    renderer->GetRenderWindow()->Render();
    //}

    void Change_Shapes(QComboBox* comboBox,
        vtkGenericOpenGLRenderWindow* window)

    {
        vtkSmartPointer<vtkActor> Circleactor = vtkSmartPointer<vtkActor>::New();
        vtkDataSetMapper* Circlemapper = vtkDataSetMapper::New();
        vtkSmartPointer<vtkLineSource> CircleSource = vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkRenderer> Circlerenderer = vtkSmartPointer<vtkRenderer>::New();
        // Remove any existing actors from the renderer
        //renderer->RemoveAllViewProps();
        vtkNew<MouseInteractorStyleDrawLine> style;
        QString shape_name = comboBox->currentText();

        if (shape_name == "Circle")
        {
            //vtkNew<vtkRenderer> Circlerenderer = renderer;
            style->setFlag(false);
            style->setPolyFlag(false);
            /// Actor, Source, Mapper
            vtkSmartPointer<vtkActor> Circleactor = actor;
            vtkDataSetMapper* Circlemapper = mapper;
            vtkSmartPointer<vtkLineSource> CircleSource = lineSource;
            vtkSmartPointer<vtkRenderer> Circlerenderer = renderer;
            Draw_circle(Circleactor,Circlemapper,CircleSource,Circlerenderer);
            
            
        }
        else if (shape_name == "Sphere") {
            Draw_Football();
            //Flag = false;
        }
        else if (shape_name == "Arc")
        {
            Draw_Arc();
            //Flag = false;
        }
        else if (shape_name == "Hexahedron")
        {
            //bool ok;
            //// Get the center of the circle from the user using a QInputDialog
            //double len = QInputDialog::getDouble(nullptr, "Enter the length", "Enter the length of the each side of the Arc:", 0.0, -100.0, 100.0, 2, &ok);
            //if (!ok) {
            //    return;
            //}
            //QMessageBox messageBox;
            //messageBox.setText("Choose Region Type");
            //QAbstractButton* filledButton = messageBox.addButton(QMessageBox::tr("Filled"), QMessageBox::YesRole);
            //QAbstractButton* nonFilledButton = messageBox.addButton(QMessageBox::tr("Non-Filled"), QMessageBox::YesRole);
            //messageBox.exec();
            //QString buttonText = messageBox.clickedButton()->text();
            //std::string mode = buttonText.toStdString();
            //Draw_Hexahedron("Red", 1.0, mode, len);
           // Flag = false;
        }
        else if (shape_name == "Line")
        {
            //Draw_Line();
           /* if (Circleactor != NULL && Circlemapper != NULL && CircleSource != NULL && Circlerenderer != NULL) {
                Circleactor->Delete();
                Circlemapper->Delete();
                CircleSource-> Delete();
                Circlerenderer->Delete();
            }*/
            vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
            renderWindowInteractor->SetRenderWindow(window);
            // Set the custom interactor style
            
            style->SetRenderer(renderer);
            style->setFlag(true);
            style->setPolyFlag(false);
            renderWindowInteractor->SetInteractorStyle(style.Get());
            /*window->Render();
            renderWindowInteractor->Initialize();
            renderWindowInteractor->Start();*/
            
           
        }
        else if (shape_name == "Polyline")
        {
            //Draw_Line();
           /* if (Circleactor != NULL && Circlemapper != NULL && CircleSource != NULL && Circlerenderer != NULL) {
                Circleactor->Delete();
                Circlemapper->Delete();
                CircleSource-> Delete();
                Circlerenderer->Delete();
            }*/
            vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
            renderWindowInteractor->SetRenderWindow(window);
            // Set the custom interactor style

            style->SetRenderer(renderer);
            style->setFlag(false);
            style->setPolyFlag(true);
            renderWindowInteractor->SetInteractorStyle(style.Get());
            /*window->Render();
            renderWindowInteractor->Initialize();
            renderWindowInteractor->Start();*/


        }
        else if (shape_name == "Cone") {
            Draw_Cone();
           // Flag = false;
        }
        else if (shape_name == "Regular Polygon")
        {
            Draw_Regular_Polygon();
            //Flag = false;
        }
        else if (shape_name == "Cylinder") {
            Draw_Cylinder();
           // Flag = false;
        }
        else if (shape_name == "Ellipse") {
            Draw_Ellipse();
           // Flag = false;
        }
        else if (shape_name == "Triangle Strip") {
            //bool ok;
            // 
            //// Get the center of the circle from the user using a QInputDialog
            //double len = QInputDialog::getDouble(nullptr, "Enter the length", "Enter the length of the each side of the Triangle Strip:", 0.0, -100.0, 100.0, 2, &ok);
            //if (!ok) {
            //    return;
            //}
            //Draw_TriangleStrip("Red", 1.0, len);
            
        }
        else if (shape_name == "Text") {
            //// Prompt user for text and font size
            //QString text = QInputDialog::getText(nullptr, "Text Input", "Enter text:");
            //bool ok;
            //int fontSize = QInputDialog::getInt(nullptr, "Font Size Input", "Enter font size:", 24, 1, 100, 1, &ok);
            //if (!ok) return; // User cancelled input dialog
            //Draw_Text(text, fontSize, "Red");
           // Flag = false;
        }
        else if (shape_name == "Square") {
            Draw_Square();
           // Flag = false;
        }
        // Reset the camera and render the window
        //window->Render();
        window->Render();
        // 
        //renderer->ResetCamera();
    }
} // namespace



int main(int argc, char* argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

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
    uploadButton.setText("Upload");
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
    shapesdroplist->setCurrentIndex(0); // Set default value
    dockLayout->addWidget(shapesdroplist);

    // render area
    QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget =
        new QVTKOpenGLNativeWidget();
    mainWindow.setCentralWidget(vtkRenderWidget);

    // VTK part
    vtkNew<vtkGenericOpenGLRenderWindow> window;
    vtkRenderWidget->setRenderWindow(window.Get());

    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(1.0);
    sphere->SetThetaResolution(100);
    sphere->SetPhiResolution(100);

    mapper->SetInputConnection(sphere->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetEdgeVisibility(true);
    actor->GetProperty()->SetRepresentationToSurface();

    //vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);

    window->AddRenderer(renderer);

    // setup initial status
    std::mt19937 randEng(0);
    ::Randomize(sphere, mapper, window, randEng);

    // connect the buttons
    QObject::connect(&randomizeButton, &QPushButton::released,
        [&]() { ::Randomize(sphere, mapper, window, randEng); });

    // Use a lambda function with capture by reference
    bool istrue = true;
    QObject::connect(changeshapes, &QPushButton::clicked,
        [=, &shapesdroplist, &window]() { Change_Shapes(shapesdroplist, window); });

    //vtkNew<MouseInteractorStyleDrawLine> style;
    //style->setFlag(istrue);
    //if (style->getFLag() == true) {
    //    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    //    renderWindowInteractor->SetRenderWindow(window);
    //    // Set the custom interactor style

    //    style->SetRenderer(renderer);
    //    renderWindowInteractor->SetInteractorStyle(style.Get());
    //    window->Render();
    //    renderWindowInteractor->Initialize();
    //    renderWindowInteractor->Start();
    //}
    mainWindow.show();

    return app.exec();
}
