/**********************************************************************
  Copyright (C) 2004 by Chris Morley
  Copyright (C) 2016 by Daniel Menendez

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

/* This is a heavily commented template for a OpenBabel format class.

   Format classes are plugins: no modifications are needed to existing code
   to indroduce a new format. The code just needs to be compiled and linked
   with the rest of the OpenBabel code.
   Alternatively, they can be built (either singly or in groups) as DLLs
   or shared libraries. [**Extra build info**]

   Each file may contain more than one format.

   This compilable, but non-functional example is for a format which
   converts a molecule to and from OpenBabel's internal format OBMol.
   The conversion framework can handle other types of object, provided
   they are derived from OBBase, such as OBReaction, OBText.

*/

#define HARTREE_TO_KCAL_PER_MOL 627.509469

#include <string>
#include <openbabel/babelconfig.h>
#include <openbabel/obmolecformat.h>

using namespace std;
namespace OpenBabel
{

  class PMDFormat : public OBMoleculeFormat
                    // Derive directly from OBFormat for objects which are not molecules.
  {
    public:
      //Register this format type ID in the constructor
      PMDFormat()
      {
        /* PMD is the Promolden file extension and is case insensitive. A MIME type can be
           added as an optional third parameter.
           Multiple file extensions can be registered by adding extra statements.*/
        OBConversion::RegisterFormat("PMD",this);

        /* If there are any format specific options they should be registered here
           so that the commandline interface works properly.
           The first parameter is the option name. If it is a single letter it can be
           concatinated with other	single letter options. For output options it can be
           multicharcter and is then	written as --optionname on the command line.
           The third parameter is the number of parameters the option takes. Currently
           this is either 1 or 0 and if it is 0 can be omitted for output options.
           The parameter is always text and needs to be parsed to extract a number.

           Options can apply when writing - 4th parameter is OBConversion::OUTOPTIONS
           or can be omitted as shown. A single letter output option is preceded
           by -x on the command line.
           Or options can apply to the input format - the 4th parameter is
           OBConversion::INOPTIONS. They are then  preceded by -a on the command line.

           Each option letter may be reused in other formats, but within the same group,
           INOPTIONS or OUTOPTIONS, must take the same number of parameters (0 or 1).
           There will be an error message when OpenBabel	runs if there are conflicts
           between formats. A list of formats currently used (which may not be
           comprehensive) is in docs/options.html.
           */
        OBConversion::RegisterOptionParam("f", this, 1);
        OBConversion::RegisterOptionParam("n", this);
        OBConversion::RegisterOptionParam("b", this, 0, OBConversion::INOPTIONS);
        OBConversion::RegisterOptionParam("s", this, 0, OBConversion::INOPTIONS);

      }

      /* The first line of the description should be a brief identifier, <40 chars, because
         it is used in dropdown lists, etc. in some user interfaces. The rest is optional.

         Describe any format specific options here. This text is parsed to provide
         checkboxes, etc for the GUI (for details click the control menu),
         so please try to keep to a similar form.

         Write options are the most common, and the "Write" is optional.
         The option f takes a text parameter, so that it is essential that the option
         is registered in the constructor of the class.
         Finish the options with a blank line as shown, if there are more than one
         group of options, or if there are further comments after them.
         */
      virtual const char* Description() //required
      {
        return
          "PMD (Promolden) output format\n"
          "The PMD chemical file format contains an atoms-in-molecules analysis.\n"
          "No formal specification has been published\n"
          "Read Options e.g. -as\n"
          "  s  Output single bonds only\n"
          "  b  Disable bonding entirely\n\n"
          ;
      };

      //Optional URL where the file format is specified
      virtual const char* SpecificationURL(){return
        "http://azufre.quimica.uniovi.es";};

      //Optional
      virtual const char* GetMIMEType()
      { return "chemical/x-promolden"; };


      /* Flags() can return be any of the following combined by |
         or be omitted if none apply
         NOTREADABLE  READONEONLY  NOTWRITABLE  WRITEONEONLY  DEFAULTFORMAT
         READBINARY  WRITEBINARY  READXML  ZEROATOMSOK*/
      virtual unsigned int Flags()
      {
        return READONEONLY | NOTWRITABLE;
      };

      /* This optional function is for formats which can contain more than one
         molecule. It is used to quickly position the input stream after the nth
         molecule without have to convert and discard all the n molecules.
         See obconversion.cpp for details and mdlformat.cpp for an example.*/
      virtual int SkipObjects(int n, OBConversion* pConv)
      {
        return 0;
      };

      ////////////////////////////////////////////////////
      /// Declarations for the "API" interface functions. Definitions are below
      virtual bool ReadMolecule(OBBase* pOb, OBConversion* pConv);

    private:
      /* Add declarations for any local function or member variables used.
         Generally only a single instance of a format class is used. Keep this in
         mind if you employ member variables. */
  };
  ////////////////////////////////////////////////////

  //Make an instance of the format class
  PMDFormat thePMDFormat;

  /////////////////////////////////////////////////////////////////

  bool PMDFormat::ReadMolecule(OBBase* pOb, OBConversion* pConv)
  {
    OBMol* pmol = pOb->CastAndClear<OBMol>();
    if(pmol==NULL)
      return false;

    istream& ifs = *pConv->GetInStream();

    OBMol &mol = *pmol;

    pmol->BeginModify();

    /** Parse the input stream and use the OpenBabel API to populate the OBMol **/

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% dmc %%%%%%%%%%%%%%%%%%%%%%%%%
    unsigned int natoms;	
    char buffer[BUFF_SIZE];
    string line;

    stringstream errorMsg;

    if (!ifs)
      return false; // we're attempting to read past the end of the file
    std::size_t found;
    do { // Skip lines until beginning of data.
      getline(ifs,line);
      found = line.find("Number of Centers:");

    } while (found==string::npos);
    string tmpstring;
    tmpstring = line.substr(line.find_last_of(":")+1);
    istringstream ( tmpstring ) >> natoms;
    mol.ReserveAtoms(natoms);
    do { // Skip lines until beginning of data.
      getline(ifs,line);
      found = line.find("Cartesian coordinates of centers:");

    } while (found==string::npos);
    getline(ifs,line); // skip blank line

    // The next lines contain five items each, separated by white
    // spaces: the atom index, the atom type, and the coordinates of the atom
    vector<string> vs;
    for (unsigned int i = 1; i <= natoms; i ++)
    {
      if (!ifs.getline(buffer,BUFF_SIZE))
      {
        errorMsg << "Problems reading a PMD file: "
          << " According to Number of Centers, there should be " << natoms
          << " atoms, and therefore " << natoms << " coordinates in the file.";

        obErrorLog.ThrowError(__FUNCTION__, errorMsg.str() , obWarning);
        return(false);
      }
      tokenize(vs,buffer);
      if (vs.size() < 5) // ignore extra columns which some applications add
      {
        errorMsg << "Problems reading an XYZ file: "
          << "Could not read coordinates line #" << i << "." << endl
          << "OpenBabel found the line '" << buffer << "'" << endl
          << "According to the specifications, this line should contain exactly 5 entries, separated by white space." << endl
          << "However, OpenBabel found " << vs.size() << " items.";

        obErrorLog.ThrowError(__FUNCTION__, errorMsg.str() , obWarning);
        return(false);
      }

      // Atom Type: get the atomic number from the element table, using
      // the second entry in the currently read line. If the entry makes
      // sense, set the atomic number and leave the atomic type open
      // (the type is then later faulted in when atom->GetType() is
      // called). If the entry does not make sense to use, set the atom
      // type manually, assuming that the author of the pmd-file had
      // something "special" in mind.
      OBAtom *atom  = mol.NewAtom();

      int atomicNum = etab.GetAtomicNum(vs[1].c_str());
      //set atomic number, or '0' if the atom type is not recognized
      if (atomicNum == 0) {
        // i.e., the first column is the atomic number, not a symbol
        // so we'll first check if we can convert this to an element number
        atomicNum = atoi(vs[1].c_str());
      }

      atom->SetAtomicNum(atomicNum);
      if (atomicNum == 0) // still strange, try using an atom type
        atom->SetType(vs[1]);

      // Read the atom coordinates
      char *endptr;
      double x = strtod((char*)vs[2].c_str(),&endptr);
      if (endptr == (char*)vs[2].c_str())
      {
        errorMsg << "Problems reading an PMD file: "
          << "OpenBabel found the line '" << buffer << "'" << endl
          << "According to the specifications, this line should contain exactly 5 entries, separated by white space." << endl
          << "OpenBabel could not interpret item #2 as a number.";

        obErrorLog.ThrowError(__FUNCTION__, errorMsg.str() , obWarning);
        return(false);
      }
      double y = strtod((char*)vs[3].c_str(),&endptr);
      if (endptr == (char*)vs[3].c_str())
      {
        errorMsg << "Problems reading an XYZ file: "
          << "OpenBabel found the line '" << buffer << "'" << endl
          << "According to the specifications, this line should contain exactly 5 entries, separated by white space." << endl
          << "OpenBabel could not interpret item #3 as a number.";

        obErrorLog.ThrowError(__FUNCTION__, errorMsg.str() , obWarning);
        return(false);
      }
      double z = strtod((char*)vs[4].c_str(),&endptr);
      if (endptr == (char*)vs[4].c_str())
      {
        errorMsg << "Problems reading an XYZ file: "
          << "OpenBabel found the line '" << buffer << "'" << endl
          << "According to the specifications, this line should contain exactly 5 entries, separated by white space." << endl
          << "OpenBabel could not interpret item #3 as a number.";

        obErrorLog.ThrowError(__FUNCTION__, errorMsg.str() , obWarning);
        return(false);
      }
      atom->SetVector(x,y,z); //set coordinates
    }

    bool tes_partition = false;
    //OBPairData *tmpair = new OBPairData();
    int atomid = 99999;
    vector<OBPairData*> kineticAtE;
    vector<OBPairData*> potentialAtE;
    vector<OBPairData*> elRepAtE;
    vector<OBPairData*> coul_elRepAtE;
    vector<OBPairData*> xc_elRepAtE;
    vector<OBPairData*> selfelRepAtE;
    OBAtom* atom = new OBAtom();
    while (ifs.getline(buffer,BUFF_SIZE))
    {
      if (strstr(buffer, "------------ TES PARTITION --------------------")) 
      {
        tes_partition = true;
        // Maybe should be changed to an array (fixed size=natoms)
        //unsigned long int* atomid = NULL;
      }
        
      if (strstr(buffer, "Atomic Contributions for neq:")) 
      { // Promolden reconstructed kinetic energy
        ifs.getline(buffer,BUFF_SIZE); // Jump undesired lines
        tokenize(vs, buffer);
        atomid = atoi(vs[3].c_str());
        // Openbabel starts at 0 and Promolden at 1
        atom = mol.GetAtomById(atomid-1); 

        if (atom->HasData("kinetic energy"))
        {  
          cout << "Your file contains concatenated outputs.\n" 
               << "The atomic kinetic energy is repeated.\n" 
               << "The first occurrence of atomic contributions for atom " 
               << atomid << " was taken!!" << endl;
          continue; // skip
        }

        ifs.getline(buffer,BUFF_SIZE); // Jump undesired lines

        // Atomic Kinetic energy
        ifs.getline(buffer,BUFF_SIZE);
        tokenize(vs, buffer);
        kineticAtE.push_back( new OBPairData);
        kineticAtE[atomid-1]->SetAttribute("kinetic energy");
        kineticAtE[atomid-1]->SetValue( vs[3]);
        atom->SetData(kineticAtE[atomid-1]);
        // Atomic Potential energy
        ifs.getline(buffer,BUFF_SIZE); // Next line
        tokenize(vs, buffer);
        potentialAtE.push_back( new OBPairData);
        potentialAtE[atomid-1]->SetAttribute("potential energy");
        potentialAtE[atomid-1]->SetValue( vs[3]);
        atom->SetData(potentialAtE[atomid-1]);
        // Atomic Electron repulsion
        ifs.getline(buffer,BUFF_SIZE); // Next line
        tokenize(vs, buffer);
        elRepAtE.push_back( new OBPairData);
        elRepAtE[atomid-1]->SetAttribute("electron repulsion");
        elRepAtE[atomid-1]->SetValue( vs[3]);
        atom->SetData(elRepAtE[atomid-1]);
        // Atomic Electron Coulomb repulsion
        ifs.getline(buffer,BUFF_SIZE); // Next line
        tokenize(vs, buffer);
        coul_elRepAtE.push_back( new OBPairData);
        coul_elRepAtE[atomid-1]->SetAttribute("electron coulomb repulsion");
        coul_elRepAtE[atomid-1]->SetValue( vs[2]);
        atom->SetData(coul_elRepAtE[atomid-1]);
        // Atomic Electron Exchange-Correlation repulsion
        ifs.getline(buffer,BUFF_SIZE); // Next line
        tokenize(vs, buffer);
        xc_elRepAtE.push_back( new OBPairData);
        xc_elRepAtE[atomid-1]->SetAttribute("electron xc repulsion");
        xc_elRepAtE[atomid-1]->SetValue( vs[2]);
        atom->SetData(xc_elRepAtE[atomid-1]);
        // Atomic Electron self repulsion
        ifs.getline(buffer,BUFF_SIZE); // Next line
        tokenize(vs, buffer);
        selfelRepAtE.push_back( new OBPairData);
        selfelRepAtE[atomid-1]->SetAttribute("electron self repulsion");
        selfelRepAtE[atomid-1]->SetValue( vs[2]);
        atom->SetData(selfelRepAtE[atomid-1]);
        //
        //delete tmpair;
        atomid = 99999;
        //delete atomicid;
      }

      if (strstr(buffer, "      KINETIC           =")) 
      { // Promolden reconstructed kinetic energy
        tokenize(vs, buffer);
        OBPairData *kineticE = new OBPairData();
        kineticE->SetAttribute("kinetic energy");
        //string tmp = std::to_string(atof(vs[2].c_str()) * HARTREE_TO_KCAL_PER_MOL);
        kineticE->SetValue( vs[2]);
        mol.SetData(kineticE);
      }
      if (strstr(buffer, "     POTENTIAL         =")) 
      { // Promolden reconstructed potential energy
        tokenize(vs, buffer);
        OBPairData *potentialE = new OBPairData();
        potentialE->SetAttribute("potential energy");
        potentialE->SetValue( vs[2]);
        mol.SetData(potentialE);
      }
      if (strstr(buffer, "     NUC-NUC           =")) 
      { // Promolden reconstructed nuclear-nuclear energy
        tokenize(vs, buffer);
        OBPairData *nucNucE = new OBPairData();
        nucNucE->SetAttribute("nuc-nuc energy");
        nucNucE->SetValue( vs[2]);
        mol.SetData(nucNucE);
      }
      if (strstr(buffer, "     NUC-EL            =")) 
      { // Promolden reconstructed nuclear-electron energy
        tokenize(vs, buffer);
        OBPairData *nucElE = new OBPairData();
        nucElE->SetAttribute("nuc-el energy");
        nucElE->SetValue( vs[2]);
        mol.SetData(nucElE);
      }
      if (strstr(buffer, "     EL-EL             =")) 
      { // Promolden reconstructed electron-electron energy
        tokenize(vs, buffer);
        OBPairData *elElE = new OBPairData();
        elElE->SetAttribute("el-el energy");
        elElE->SetValue( vs[2]);
        mol.SetData(elElE);
      }
      if (strstr(buffer, "      2T+V              =")) 
      { // Promolden reconstructed virial energy
        tokenize(vs, buffer);
        OBPairData *virialE = new OBPairData();
        virialE->SetAttribute("virial energy");
        virialE->SetValue( vs[2]);
        mol.SetData(virialE);
      }
      if (strstr(buffer, "      ENET              =")) 
      { // Promolden reconstructed net energy
        tokenize(vs, buffer);
        OBPairData *netE = new OBPairData();
        netE->SetAttribute("net energy");
        netE->SetValue( vs[2]);
        mol.SetData(netE);
      }
      if (strstr(buffer, "      EINT              =")) 
      { // Promolden reconstructed interaction energy
        tokenize(vs, buffer);
        OBPairData *intE = new OBPairData();
        intE->SetAttribute("int energy");
        intE->SetValue( vs[2]);
        mol.SetData(intE);
      }
      if (strstr(buffer, "      TOTAL             =")) 
      { // Promolden reconstructed total energy
        tokenize(vs, buffer);
        pmol->SetEnergy(atof(vs[2].c_str()) * HARTREE_TO_KCAL_PER_MOL);
      }
      if (strstr(buffer, " TotQ=")) 
      { // Promolden reconstructed charge
        tokenize(vs, buffer);
        pmol->SetTotalCharge( atof(vs[1].c_str() ) );
      }
    }



    // clean out any remaining blank lines
    while(ifs.peek() != EOF && ifs.good() &&
        (ifs.peek() == '\n' || ifs.peek() == '\r'))
      ifs.getline(buffer,BUFF_SIZE);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% dmc %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    // To use an input option
    if (!pConv->IsOption("b",OBConversion::INOPTIONS))
      mol.ConnectTheDots();
    if (!pConv->IsOption("s",OBConversion::INOPTIONS) && !pConv->IsOption("b",OBConversion::INOPTIONS))
      mol.PerceiveBondOrders();


    /* If the molecule has other than 3D coordinates for its atoms, it
       is necessary to set the dimension to 0, or 2 */
    int dim;
    pmol->SetDimension(dim);

    pmol->EndModify();

    /* For multi-molecule formats, leave the input stream at the start of the
       next molecule, ready for this routine to be called again.

    /* Return true if ok. Returning false means discard the OBMol and stop
    converting, unless the -e option is set. With a multi-molecule inputstream
    this will skip the current molecule and continue with the next, if SkipObjects()
    has been defined. If it has not, and continuation after errors is still required,
    it is necessary to leave the input stream at the beginning of next object when
    returning false;*/
    return true;
  }

  ////////////////////////////////////////////////////////////////

  // There is no Promolden input with molecular information.
  // The info is taken from the output of other codes.

} //namespace OpenBabel

