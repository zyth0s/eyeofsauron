/*
 * =====================================================================================
 *
 *       Filename:  representation.cxx
 *
 *    Description:  A VTK molecular representation
 *
 *        Version:  1.0
 *        Created:  07/05/2016 01:54:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel (dmc), danielmail7@gmail.com
 *   Organization:  University of Oviedo
 *
 * =====================================================================================
 */
#define BOHR_TO_ANGSTROM .529177

#include <string>
using std::string;
// Openbabel
//#include <openbabel/obiter.h>
#include <openbabel/mol.h>
// VTK
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkMolecule.h"
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include "vtkLight.h"
#include "vtkMoleculeMapper.h"
//#include "vtkNew.h"
#include "vtkProperty.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkPeriodicTable.h"
//#include "vtkBlueObeliskData.h"
//#include "vtkCleanPolyData.h"
//#include "vtkRenderer.h"
// Interatomic surfaces
#include "vtkPoints.h"
#include <vtkPolyData.h>
#include <vtkDelaunay3D.h>
#include <vtkDataSetMapper.h>
// Escher
#include "include/representation.hpp"

Representation::Representation()
{
  vtkNew<vtkRenderer> ren;
}

void Representation::MolecularRepresentation(OpenBabel::OBMol obmol)
{


  vtkNew<vtkMolecule> repmol;
  repmol->Initialize();

  FOR_ATOMS_OF_MOL(atom, obmol)
  {
     repmol->AppendAtom(atom->GetAtomicNum(), atom->GetX(), atom->GetY(), atom->GetZ());
  }

  FOR_BONDS_OF_MOL(bond, obmol)
  {
    // A tricky point of OpenBabel has to be accounted here:
    // OBAtom  indices begin at 1.
    // OBBond  indices begin at 0.
    // vtkAtom indices begin at 0.
    vtkAtom beginAtom = repmol->GetAtom(bond->GetBeginAtom()->GetIdx()-1);
    vtkAtom endAtom   = repmol->GetAtom(bond->GetEndAtom()->GetIdx()-1);

    repmol->AppendBond( beginAtom, endAtom, bond->GetBondOrder());
  }


  vtkNew<vtkMoleculeMapper> molmapper;
  molmapper->SetInputData(repmol.GetPointer());

  //molmapper->UseFastSettings();
  molmapper->UseBallAndStickSettings();
  //molmapper->RenderAtomsOff();
  //molmapper->UseMultiCylindersForBondsOff();

  vtkNew<vtkActor> actor;
  actor->SetMapper(molmapper.GetPointer());
  actor->GetProperty()->SetAmbient(0.0);
  actor->GetProperty()->SetDiffuse(0.0);
  actor->GetProperty()->SetSpecular(0.0);
  actor->GetProperty()->SetSpecularPower(40);


  vtkNew<vtkLight> light;
  light->SetLightTypeToCameraLight();
  light->SetPosition(1.0, 1.0, 1.0);


  vtkNew<vtkRenderWindow> win;
  win->AddRenderer(this->ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win.GetPointer());

  this->ren->AddActor(actor.GetPointer());
  //ren->AddActor(axesActor.GetPointer());

  this->ren->SetBackground(1.0, 1.0, 1.0);
  win->SetSize(450, 450);
  win->Render();
  this->ren->GetActiveCamera()->ParallelProjectionOn();
  //ren->SetActiveCamera(camera.GetPointer());
  this->ren->GetActiveCamera()->Zoom(2.2);

  // place axes at lower left corner
  vtkNew<vtkAxesActor> axesActor;
  axesActor->SetShaftTypeToCylinder();
  //axesActor->GetXAxisShaftProperty()->SetColor(0.5,0.5,0.5);
  axesActor->AxisLabelsOff();
  vtkNew<vtkOrientationMarkerWidget> axesWidget;
  axesWidget->SetOrientationMarker( axesActor.GetPointer() );
  axesWidget->SetInteractor( iren.GetPointer() );
  axesWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  axesWidget->SetViewport( 0.0, 0.0, 0.3, 0.3 );
  axesWidget->SetEnabled( 1);
  axesWidget->InteractiveOn();
  //axesWidget->KeyPressActivationOff();

  // Finally render the scene and compare the image to a reference image
  win->SetMultiSamples(0);
  win->GetInteractor()->Initialize();
  win->GetInteractor()->Start();

}

void Representation::BasinsRepresentation(const std::string infname, const int atnum)
{
  string line;
  int no_vertices;
  cout << "Reading " << infname << endl;
  ifstream ifs(infname.c_str());
  for (int i=0; i<11;i++)
  {
      getline(ifs,line);
  }
  ifs >> no_vertices;
  cout << "Number of vertices: " << no_vertices << endl;
  getline(ifs,line); // line no. 11
  getline(ifs,line); // line no. 12
  getline(ifs,line); // line no. 13

  float x[3];
  vtkNew<vtkPoints> points;
  //for (vtkIdType i=1; i<no_vertices; i++)
  for (int zzz=0; zzz<no_vertices; zzz++)
  {
      ifs >> x[0] >> x[1] >> x[2];
      getline(ifs,line); // get the rest of line
      //cout << i << endl;
      //cout << x[0] << " " << x[1] << " " << x[2] << endl;
      
      //points->InsertPoint(i,x[3]); 
      points->InsertPoint(zzz,x[0]*BOHR_TO_ANGSTROM,x[1]*BOHR_TO_ANGSTROM,x[2]*BOHR_TO_ANGSTROM); 
  }
  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points.GetPointer());
  vtkNew<vtkDelaunay3D> delny;
  delny->SetInputData(polydata.GetPointer());
  delny->SetTolerance(0.01);
  delny->SetAlpha(0.0);
  delny->BoundingTriangulationOff();

  vtkNew<vtkDataSetMapper> surfmap;
  surfmap->SetInputConnection(delny->GetOutputPort());

  vtkNew<vtkActor> triangulation;
  triangulation->SetMapper(surfmap.GetPointer());
  triangulation->GetProperty()->SetOpacity(0.7);
  //triangulation->GetProperty()->SetRepresentationToWireframe();
  float rgb[3];
  vtkNew<vtkPeriodicTable> eltable;
  eltable->GetDefaultRGBTuple(atnum, rgb);
  triangulation->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);

  ren->AddActor(triangulation.GetPointer());

  ifs.close();
}

void Representation::SurfRepresentation(const std::string infname, const int atnum, const float xcenter[3])
{
  float rmaxsurf = 10.0;
  float eps = 0.01;
  string line;

  cout << "Reading " << infname << endl;
  ifstream ifs(infname.c_str());
  int no_vertices;
  ifs >> no_vertices;
  cout << "Number of vertices: " << no_vertices << endl;
  std::size_t found;
  do { // Skip lines until beginning of data.
      getline(ifs,line);
      found = line.find("cos");

  } while (found==string::npos);

  // Read points into a vtkPoints object.
  vtkNew<vtkPoints> points;
  float costh, sinth, cosph, sinph, r;
  float tmp; 
  float x[3];
  int i = 0; // Index # for Points in vtkPoints
  for (int zzz=0; zzz<no_vertices; zzz++)
  {   // Loop over all available intersections in the file
      ifs >> costh >> sinth >> cosph >> sinph;
      ifs >> tmp; //ifs.ignore(); discard theta weight
      ifs >> r;
      if (fabs(r-rmaxsurf)<eps) // Discard rmax values
          continue;
      else i++;                 // keep i indices.
      getline(ifs,line);        // get the rest of line
      //cout << i << " " << costh << " " << sinth << " " 
      //                 << cosph << " " << sinph << " " << r << endl;
      // Spherical to cartesian
      x[0] = r * sinth * cosph * BOHR_TO_ANGSTROM;
      x[1] = r * sinth * sinph * BOHR_TO_ANGSTROM;
      x[2] = r * costh         * BOHR_TO_ANGSTROM;
      //cout  << xcenter[0] << " " << xcenter[1] << " " << xcenter[2] << endl;
      x[0] = x[0] + xcenter[0]; // Center around the nuclei
      x[1] = x[1] + xcenter[1];
      x[2] = x[2] + xcenter[2];
      //cout  << x[0] << " " << x[1] << " " << x[2] << endl;
      points->InsertPoint(i,x[0],x[1],x[2]); 
  }

  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points.GetPointer());

  // Clean the polydata. This will remove duplicate points that may be
  // present in the input data.
  //vtkNew<vtkCleanPolyData> cleaner;
  //cleaner->SetInputData(polydata.GetPointer());

  vtkNew<vtkDelaunay3D> delny;
  delny->SetInputData(polydata.GetPointer());
  //delny->SetInputConnection(cleaner->GetOutputPort());
  delny->SetTolerance(0.01);
  delny->SetAlpha(0.0);
  delny->BoundingTriangulationOff();

  vtkNew<vtkDataSetMapper> surfmap;
  surfmap->SetInputConnection(delny->GetOutputPort());

  vtkNew<vtkActor> triangulation;
  triangulation->SetMapper(surfmap.GetPointer());
  triangulation->GetProperty()->SetOpacity(0.5);
  //triangulation->GetProperty()->SetRepresentationToWireframe();
  float rgb[3];
  vtkNew<vtkPeriodicTable> eltable;
  eltable->GetDefaultRGBTuple(atnum, rgb);
  triangulation->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);

  ren->AddActor(triangulation.GetPointer());

  ifs.close();
}















