#!/usr/bin/env python

#from sys import path
#path.append('./lib')
#import libeyeos as eyeos

from lib import libeyeos as eyeos


eos = eyeos.EyeOfSauron()
#eos.ReadFile("test/surf/ch4.pmd", "pmd") # optional format
eos.ReadFile("test/surf/ch4.pmd") # optional format
#eos.ShowData()
# ("surfFile", atom type color, atom id for centering)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0002",1,2)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0003",1,3)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0004",1,4)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0005",1,5)
eos.ReadSurf("test/surf/ch4.wfn.surf-txt0001",6,1) 
eos.SeeAll()


