/*
 * =====================================================================================
 *
 *       Filename:  escher.cpp
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
//#include <openbabel/mol.h>
// Escher
#include "include/representation.hpp"
#include "include/escher.hpp"

using namespace std;
using namespace OpenBabel;

/*
void Convert(const string infname )
{
  ifstream ifs(infname.c_str());
  if(!ifs)
  {
    cout << "Cannot open input file" << endl;
    return ;
  }
  ofstream ofs("out.gzmat");
  if(!ofs)
  {
    cout << "Cannot open output file" << endl;
    return ;
  }
  OpenBabel::OBConversion conv(&ifs, &ofs);
  if(!conv.SetInAndOutFormats("XYZ","GZMAT"))
  {
    cout << "Formats not available" << "InputFileName" << endl;
    return ;
  }
  int n = conv.Convert();
  cout << n << " molecules converted" << endl;

  OBConversion obconversion;
  obconversion.SetInFormat("XYZ");
  OBMol obmol;
  obconversion.ReadFile(&obmol,"input.xyz");

  MolecularRepresentation(obmol);


}
*/

string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

Escher::Escher()
{
    cout << "constructed Escher class" << endl;

}

void Escher::ReadFile(const string infname) 
{
  cout << "Reading " << infname << endl;
  ifstream ifs(infname.c_str());
  if(!ifs)
  {
    cout << "Cannot open input file" << endl;
    return ;
  }

  OBConversion obconversion;
  obconversion.SetInFormat(GetFileExtension(infname).c_str());

  obconversion.ReadFile(&obmol,infname);
  std::cout << "Molecular Weight: " << obmol.GetMolWt() << std::endl;
}

void Escher::ReadBasin(const string infname) 
{
  rep.BasinsRepresentation(infname);
}

void Escher::Show() 
{
  rep.MolecularRepresentation(obmol);
}

/*
Escher::~Escher()
{
    cout << "destructed Escher class" << endl;

}
*/
