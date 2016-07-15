#include <string>
// Openbabel
#include <openbabel/mol.h>
// VTK
//#include "vtkMolecule.h"
#include "vtkRenderer.h"
#include "vtkNew.h"

#ifndef __MolecularRepresentation_H_INCLUDED__   // if x.h hasn't been included yet...
#define __MolecularRepresentation_H_INCLUDED__   //   #define this so the compiler knows it has been included


class Representation
{
    protected:
    //vtkNew<vtkMolecule> repmol;
    vtkNew<vtkRenderer> ren;
    public:
    Representation();
    void MolecularRepresentation(OpenBabel::OBMol obmol);
    void BasinsRepresentation(const std::string infname, const int atnum);
    void SurfRepresentation(const std::string infname, const int atnum, const float xcenter[3]);
};

#endif // __MolecularRepresentation_H_INCLUDED__ 
