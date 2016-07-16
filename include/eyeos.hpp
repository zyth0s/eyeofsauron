#include <string>
// Openbabel
#include <openbabel/mol.h>
// EyeOfSauron
#include "representation.hpp"

#ifndef __EyeOfSauron_H_INCLUDED__   // if x.h hasn't been included yet...
#define __EyeOfSauron_H_INCLUDED__   //   #define this so the compiler knows it has been included


class EyeOfSauron  
{
    protected:
    Representation rep;
    public:
    std::string infname;
    OpenBabel::OBMol obmol;
    EyeOfSauron();
    //~EyeOfSauron();
    void ReadFile(const std::string infname) ;
    void ReadBasin(const std::string infname, const int atnum) ;
    void ReadSurf(const std::string infname, const int atnum, const int atomid) ;
    void Show() ;
};

#endif // __EyeOfSauron_H_INCLUDED__ 
