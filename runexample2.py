#!/usr/bin/env python

#from sys import path
#path.append('./lib')
#import libeyeos as eyeos

from lib import libeyeos as eyeos


eos = eyeos.EyeOfSauron()
#eos.ReadFile("test/aragonite.9.4-grad.cube")
eos.ReadFile("test/aragonite.9.4.outcritic", "critic") # optional format
eos.ReadBasin("test/aragonite-1.basin",20)
eos.ReadBasin("test/aragonite-5.basin",6)
eos.ReadBasin("test/aragonite-9.basin",8)
#eos.ReadBasin("test/aragonite-13.basin",8)
eos.SeeAll()

