/*
 * =====================================================================================
 *
 *       Filename:  eyeos.cpp
 *
 *    Description:  A molecular visualizer for QTAIM/IQA and 
 *                  emergent scalar fields topology.
 *                  Special focus is paid to in-home programs:
 *                  Critic2 and Promolden.
 *
 *        Version:  1.0
 *        Created:  07/04/2016 12:33:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel (dmc), danielmail7@gmail.com
 *   Organization:  University of Oviedo
 *
 * =====================================================================================
 */



#include <iostream>
#include <fstream>
// Openbabel
#include <openbabel/obconversion.h>
#include <openbabel/internalcoord.h>
#include <openbabel/generic.h>
//#include <openbabel/mol.h>
// EyeOfSauron
#include "include/representation.hpp"
#include "include/eyeos.hpp"

using namespace std;
using namespace OpenBabel;


string GetFileExtension(const std::string& FileName)
{
  if(FileName.find_last_of(".") != std::string::npos)
    return FileName.substr(FileName.find_last_of(".")+1);
  return "";
}

EyeOfSauron::EyeOfSauron()
{
  //cout << "constructed EyeOfSauron class" << endl;

}

void EyeOfSauron::ReadFile(const string infname, string informat) 
{
  cout << "Reading " << infname << endl;
  ifstream ifs(infname.c_str());
  if(!ifs)
  {
    cout << "Cannot open input file" << endl;
    return ;
  }

  OBConversion obconversion;
  if (informat.empty())
    informat = GetFileExtension(infname);
  //cout << informat << endl;
  obconversion.SetInFormat(informat.c_str());

  obconversion.ReadFile(&obmol,infname);
}
void EyeOfSauron::ShowData()
{
  if (obmol.HasData(OBGenericDataType::UnitCell))
  {
    OBUnitCell* cell = (OBUnitCell*) obmol.GetData(OBGenericDataType::UnitCell);
    cout << "Alpha: " << cell->GetAlpha() << " deg" << endl;
  }
  if ( obmol.HasData("kinetic energy") ) 
  {
    //OBPairData *kineticE = dynamic_cast<OBPairData *> obmol.GetData("kinetic energy");
    OBPairData *kin = (OBPairData *) obmol.GetData("kinetic energy");
    cout << "KINETIC: " << kin->GetValue() << " Hartree" << endl;
    //
    OBPairData *pot = (OBPairData *) obmol.GetData("potential energy");
    cout << "potential: " << pot->GetValue() << " Hartree" << endl;
    //
    OBPairData *nucnuc = (OBPairData *) obmol.GetData("nuc-nuc energy");
    cout << "nuc-nuc: " << nucnuc->GetValue() << " Hartree" << endl;
    //
    OBPairData *nucel = (OBPairData *) obmol.GetData("nuc-el energy");
    cout << "nuc-el: " << nucel->GetValue() << " Hartree" << endl;
    //
    OBPairData *elel = (OBPairData *) obmol.GetData("el-el energy");
    cout << "el-el: " << elel->GetValue() << " Hartree" << endl;
    //
    cout << "TOTAL: " << obmol.GetEnergy() << " Kcal/mol" << endl;
    //
    OBPairData *vir = (OBPairData *) obmol.GetData("virial energy");
    cout << "virial: " << vir->GetValue() << " Hartree" << endl;
    //
    OBPairData *net = (OBPairData *) obmol.GetData("net energy");
    cout << "net: " << net->GetValue() << " Hartree" << endl;
    //
    OBPairData *inter = (OBPairData *) obmol.GetData("int energy");
    cout << "int: " << inter->GetValue() << " Hartree" << endl;

    FOR_ATOMS_OF_MOL(atom,obmol)
    {
      cout << "Atom" << endl;
      OBPairData *kinat = (OBPairData *) atom->GetData("kinetic energy");
      cout << "atomic kinetic: " << kinat->GetValue() << " Hartree" << endl;
      OBPairData *potat = (OBPairData *) atom->GetData("potential energy");
      cout << "atomic potential: " << potat->GetValue() << " Hartree" << endl;
      OBPairData *elrepat = (OBPairData *) atom->GetData("electron repulsion");
      cout << "atomic electron repulsion: " << elrepat->GetValue() << " Hartree" << endl;
      OBPairData *coulelrepat = (OBPairData *) atom->GetData("electron coulomb repulsion");
      cout << "atomic electron coulomb repulsion: " << coulelrepat->GetValue() << " Hartree" << endl;
      OBPairData *xcelrepat = (OBPairData *) atom->GetData("electron xc repulsion");
      cout << "atomic electron xc repulsion: " << xcelrepat->GetValue() << " Hartree" << endl;
      OBPairData *selfelrepat = (OBPairData *) atom->GetData("electron self repulsion");
      cout << "atomic electron self repulsion: " << selfelrepat->GetValue() << " Hartree" << endl;
    }
  }


  cout << "Number of atoms: " << obmol.NumAtoms() << endl;
  //std::cout << "Molecular Weight: " << obmol.GetMolWt() << std::endl;
}

void EyeOfSauron::ReadBasin(const string infname, const int atnum) 
{
  rep.BasinsRepresentation(infname, atnum);
}
void EyeOfSauron::ReadSurf(const string infname, const int atnum, const int atomid) 
{
  //int atomid = 2;
  OBAtom *atom;
  atom = obmol.GetAtom(atomid);
  float x[3] = {atom->GetX(),atom->GetY(),atom->GetZ()};
  cout << "Centering: " << x[0] << " " << x[1] << " " << x[2] << endl;
  rep.SurfRepresentation(infname, atnum, x);
}

void EyeOfSauron::SeeAll() 
{
  rep.MolecularRepresentation(obmol);
}

/*
   EyeOfSauron::~EyeOfSauron()
   {
   cout << "destructed EyeOfSauron class" << endl;

   }
   */
