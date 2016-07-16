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
// EyeOfSauron
#include "include/eyeos.hpp"

using namespace std;
using namespace OpenBabel;

int main(int argc, char **argv)
{
  EyeOfSauron mol;
  //string infname = "input.xyz";
  string infname = "aragonite.9.4-grad.cube";
  mol.ReadFile(infname);
  infname = "aragonite-1.basin";
  mol.ReadBasin(infname, 20); // Ca
  infname = "aragonite-5.basin";
  mol.ReadBasin(infname, 6); // C
  infname = "aragonite-9.basin";
  mol.ReadBasin(infname, 8); // O
  mol.Show();

  return EXIT_SUCCESS;

}
