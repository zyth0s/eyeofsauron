#!/usr/bin/env python

#from sys import path
#path.append('./lib')
#import libeyeos as eyeos

from lib import libeyeos as eyeos

eos = eyeos.EyeOfSauron()
eos.ReadFile("test/input.xyz")
#mol.SeeAll()

eos = eyeos.EyeOfSauron()
eos.ReadFile("test/aragonite.9.4-grad.cube")
eos.ReadBasin("test/aragonite-1.basin",20)
eos.ReadBasin("test/aragonite-5.basin",6)
eos.ReadBasin("test/aragonite-9.basin",8)
eos.ReadBasin("test/aragonite-13.basin",8)
#eos.SeeAll()

eos = eyeos.EyeOfSauron()
#eos.ReadFile("surf/ch4.gamess")
eos.ReadFile("test/surf/ch4.pmd", "pmd") # optional format
# ("surfFile", atom type color, atom id for centering)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0001",6,1) 
#eos.ReadSurf("surf/ch4.wfn.surf-txt0002",1,2)
eos.SeeAll()

