#include <string>
// Openbabel
#include <openbabel/mol.h>
// Escher
#include "representation.hpp"

#ifndef __Escher_H_INCLUDED__   // if x.h hasn't been included yet...
#define __Escher_H_INCLUDED__   //   #define this so the compiler knows it has been included


class Escher  
{
    protected:
    Representation rep;
    public:
    std::string infname;
    OpenBabel::OBMol obmol;
    Escher();
    //~Escher();
    void ReadFile(const std::string infname) ;
    void ReadBasin(const std::string infname) ;
    void Show() ;
};

#endif // __Escher_H_INCLUDED__ 
