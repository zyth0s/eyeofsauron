#!/usr/bin/env python

#from sys import path
#path.append('./lib')
#import libeyeos as eyeos

from lib import libeyeos as eyeos

mol = eyeos.EyeOfSauron()
mol.ReadFile("input.xyz")
#mol.Show()

mol = eyeos.EyeOfSauron()
mol.ReadFile("aragonite.9.4-grad.cube")
mol.ReadBasin("aragonite-1.basin",20)
mol.ReadBasin("aragonite-5.basin",6)
mol.ReadBasin("aragonite-9.basin",8)
mol.ReadBasin("aragonite-13.basin",8)
#mol.Show()

mol = eyeos.EyeOfSauron()
#mol.ReadFile("surf/ch4.gamess")
mol.ReadFile("surf/ch4.pmd")
# ("surfFile", atom type color, atom id for centering)
mol.ReadSurf("surf/ch4.wfn.surf-txt0001",6,1) 
#mol.ReadSurf("surf/ch4.wfn.surf-txt0002",1,2)
mol.Show()

