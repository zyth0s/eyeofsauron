#!/usr/bin/env python

#from sys import path
#path.append('./lib')
#import libeyeos as eyeos

from lib import libeyeos as eyeos


eos = eyeos.EyeOfSauron()
eos.ReadFile("test/surf/ch4.pmd", "pmd") # optional format
eos.ShowData()
# ("surfFile", atom type color, atom id for centering)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0001",6,1) 
#eos.ReadSurf("surf/ch4.wfn.surf-txt0002",1,2)
eos.SeeAll()


