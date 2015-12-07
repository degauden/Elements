# @file ElementsKernel/PhysicalConstants.py
""" This script is a direct adaptation of CLHEP/Units/PhysicalConstants.h
and the following is the originial CLHEP comment:

-----
This file has been provided by Geant4 (simulation toolkit for HEP).

The basic units are :
              millimeter
              nanosecond
              Mega electron Volt
              positon charge
              degree Kelvin
              amount of substance (mole)
              luminous intensity (candela)
              radian
              steradian

Below is a non exhaustive list of Physical CONSTANTS,
computed in the Internal HEP System Of Units.

Most of them are extracted from the Particle Data Book :
       Phys. Rev. D  volume 50 3-1 (1994) page 1233

       ...with a meaningful (?) name ...

You can add your own constants.

Author: M.Maire
"""


from ElementsKernel.SystemOfUnits import mole, m, s
from ElementsKernel.SystemOfUnits import joule, eplus, MeV
from ElementsKernel.SystemOfUnits import henry, kelvin, atmosphere, mg
from ElementsKernel.SystemOfUnits import cm3, g


#
#
#
pi = 3.14159265358979323846
twopi = 2 * pi
halfpi = pi / 2
pi2 = pi * pi

#
#
#
Avogadro = 6.0221367e+23 / mole

#
# c   = 299.792458 mm/ns
# c^2 = 898.7404 (mm/ns)^2
#
c_light = 2.99792458e+8 * m / s
c_squared = c_light * c_light

#
# h     = 4.13566e-12 MeV*ns
# hbar  = 6.58212e-13 MeV*ns
# hbarc = 197.32705e-12 MeV*mm
#
h_Planck = 6.6260755e-34 * joule * s
hbar_Planck = h_Planck / twopi
hbarc = hbar_Planck * c_light
hbarc_squared = hbarc * hbarc

#
#
# see SystemOfUnits.h
electron_charge = -eplus  # see SystemOfUnits.h
e_squared = eplus * eplus

#
# amu_c2 - atomic equivalent mass unit
# amu    - atomic mass unit
#
electron_mass_c2 = 0.51099906 * MeV
proton_mass_c2 = 938.27231 * MeV
neutron_mass_c2 = 939.56563 * MeV
amu_c2 = 931.49432 * MeV
amu = amu_c2 / c_squared

#
# permeability of free space mu0    = 2.01334e-16 Mev*(ns*eplus)^2/mm
# permittivity of free space epsil0 = 5.52636e+10 eplus^2/(MeV*mm)
#
mu0 = 4 * pi * 1.e-7 * henry / m
epsilon0 = 1. / (c_squared * mu0)

#
# electromagnetic coupling = 1.43996e-12 MeV*mm/(eplus^2)
#
elm_coupling = e_squared / (4 * pi * epsilon0)
fine_structure_const = elm_coupling / hbarc
classic_electr_radius = elm_coupling / electron_mass_c2
electron_Compton_length = hbarc / electron_mass_c2
Bohr_radius = electron_Compton_length / fine_structure_const

alpha_rcl2 = fine_structure_const * \
    classic_electr_radius * classic_electr_radius

twopi_mc2_rcl2 = twopi * electron_mass_c2 * \
    classic_electr_radius * classic_electr_radius
#
#
#
k_Boltzmann = 8.617385e-11 * MeV / kelvin

#
#
#
STP_Temperature = 273.15 * kelvin
STP_Pressure = 1. * atmosphere
kGasThreshold = 10. * mg / cm3

#
#
#
universe_mean_density = 1.e-25 * g / cm3
