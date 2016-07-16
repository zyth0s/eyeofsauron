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
  EyeOfSauron eyeos;
  //string infname = "input.xyz";
  string infname = "test/aragonite.9.4-grad.cube";
  eyeos.ReadFile(infname);
  infname = "test/aragonite-1.basin";
  eyeos.ReadBasin(infname, 20); // Ca
  infname = "test/aragonite-5.basin";
  eyeos.ReadBasin(infname, 6); // C
  infname = "test/aragonite-9.basin";
  eyeos.ReadBasin(infname, 8); // O
  eyeos.Show();

  return EXIT_SUCCESS;

}
