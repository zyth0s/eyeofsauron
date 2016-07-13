#!/usr/bin/env python

import libescherpy as escher

#escher.Convert("input.xyz")

mol = escher.Escher()
mol.ReadFile("input.xyz")
#mol.Show()

mol = escher.Escher()
mol.ReadFile("aragonite.9.4-grad.cube")
mol.ReadBasin("aragonite-1.basin")
mol.Show()
