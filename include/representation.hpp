#include <string>
// Openbabel
#include <openbabel/mol.h>
// VTK
//#include "vtkMolecule.h"
#include "vtkRenderer.h"
#include "vtkNew.h"

#ifndef __MolecularRepresentation_H_INCLUDED__   // if x.h hasn't been included yet...
#define __MolecularRepresentation_H_INCLUDED__   //   #define this so the compiler knows it has been included

void MolecularRepresentation(OpenBabel::OBMol obmol);
void BasinsRepresentation(const std::string infname);

class Representation
{
    protected:
    //vtkNew<vtkMolecule> repmol;
    vtkNew<vtkRenderer> ren;
    public:
    Representation();
    void MolecularRepresentation(OpenBabel::OBMol obmol);
    void BasinsRepresentation(const std::string infname);
};

#endif // __MolecularRepresentation_H_INCLUDED__ 
