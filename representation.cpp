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

#include <string>
using std::string;
// Openbabel
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
void Representation::BasinsRepresentation(const std::string infname)
{
  string line;
  int no_vertices;
  cout << "Reading " << infname << endl;
  ifstream ifs(infname.c_str());
  if(ifs.is_open())
  {
    for (int i=0; i<11;i++)
    {
        getline(ifs,line);
    }
    ifs >> no_vertices;
    cout << no_vertices << endl;
  }
  getline(ifs,line); // line no. 11
  getline(ifs,line); // line no. 12
  getline(ifs,line); // line no. 13

  float x[3];
  vtkNew<vtkPoints> points;
  //for (vtkIdType i=1; i<no_vertices; i++)
  for (int i=0; i<no_vertices; i++)
  {
      ifs >> x[0] >> x[1] >> x[2];
      getline(ifs,line); // get the rest of line
      //cout << i << endl;
      //cout << x[0] << " " << x[1] << " " << x[2] << endl;
      
      //points->InsertPoint(i,x[3]); 
      points->InsertPoint(i,x[0]*0.529,x[1]*0.529,x[2]*0.529); 
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
  //triangulation->GetProperty()->SetColor(color)

  //self.ren.AddActor(triangulation)
  ren->AddActor(triangulation.GetPointer());

  ifs.close();
}






















/*
void MolecularRepresentation(OpenBabel::OBMol obmol)
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


  vtkNew<vtkRenderer> ren;
  vtkNew<vtkRenderWindow> win;
  win->AddRenderer(ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win.GetPointer());

  ren->AddActor(actor.GetPointer());
  //ren->AddActor(axesActor.GetPointer());

  ren->SetBackground(1.0, 1.0, 1.0);
  win->SetSize(450, 450);
  win->Render();
  ren->GetActiveCamera()->ParallelProjectionOn();
  //ren->SetActiveCamera(camera.GetPointer());
  ren->GetActiveCamera()->Zoom(2.2);

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

void BasinsRepresentation(const std::string infname)
{
  string line;
  int no_vertices;
  cout << "Reading " << infname << endl;
  ifstream ifs(infname.c_str());
  if(ifs.is_open())
  {
    for (int i=0; i<11;i++)
    {
        getline(ifs,line);
    }
    ifs >> no_vertices;
    cout << no_vertices << endl;
  }
  getline(ifs,line); // line no. 11
  getline(ifs,line); // line no. 12
  getline(ifs,line); // line no. 13

  float x[3];
  vtkNew<vtkPoints> points;
  //for (vtkIdType i=1; i<no_vertices; i++)
  for (int i=1; i<=no_vertices; i++)
  {
      ifs >> x[0] >> x[1] >> x[2];
      getline(ifs,line); // get the rest of line
      //cout << i << endl;
      //cout << x[0] << " " << x[1] << " " << x[2] << endl;
      
      //points->InsertPoint(i,x[3]); ///////////////////////////////////////////////////////
      points->InsertPoint(i,x[0],x[1],x[2]); ///////////////////////////////////////////////////////
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
  //triangulation->GetProperty()->SetColor(color)

  //self.ren.AddActor(triangulation)
  //ren->AddActor(triangulation.GetPointer());

  ifs.close();
}
*/
