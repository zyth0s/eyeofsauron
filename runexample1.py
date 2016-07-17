#!/usr/bin/env python

#from sys import path
#path.append('./lib')
#import libeyeos as eyeos

from lib import libeyeos as eyeos

eos = eyeos.EyeOfSauron()
eos.ReadFile("test/input.xyz")
eos.SeeAll()

