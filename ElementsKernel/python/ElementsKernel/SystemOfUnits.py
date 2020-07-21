#
# Copyright (C) 2012-2020 Euclid Science Ground Segment
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#

""" System of Units Constants


 This script is a direct adaptation of CLHEP/Units/SystemOfUnits.h
 and the following is the original CLHEP comment:

 HEP coherent system of Units

 This file has been provided to CLHEP by Geant4 (simulation toolkit for HEP).

 The basic units are :
               millimeter              (millimeter)
               nanosecond              (nanosecond)
               Mega electron Volt      (MeV)
               positron charge         (eplus)
               degree Kelvin           (kelvin)
               the amount of substance (mole)
               luminous intensity      (candela)
               radian                  (radian)
               steradian               (steradian)

 Below is a non exhaustive list of derived and pratical units
 (i.e. mostly the SI units).
 You can add your own units.

 The SI numerical value of the positron charge is defined here,
 as it is needed for conversion factor : positron charge = e_SI (coulomb)

 The others physical constants are defined in the header file :
                       PhysicalConstants.h

:file: ElementsKernel/SystemOfUnits.py
:author: Wim Lavrijsen (LBNL, WLavrijsen@lbl.gov)
:author: Hubert Degaudenzi (ISDC -- Hubert.Degaudenzi@unige.ch)
:date: 01/21/04

"""

#
# Length [L]
#
millimeter = 1.
millimeter2 = millimeter * millimeter
millimeter3 = millimeter * millimeter * millimeter

centimeter = 10. * millimeter
centimeter2 = centimeter * centimeter
centimeter3 = centimeter * centimeter * centimeter

meter = 1000. * millimeter
meter2 = meter * meter
meter3 = meter * meter * meter

kilometer = 1000. * meter
kilometer2 = kilometer * kilometer
kilometer3 = kilometer * kilometer * kilometer

parsec = 3.0856775807e+16 * meter

micrometer = 1.e-6 * meter
nanometer = 1.e-9 * meter
angstrom = 1.e-10 * meter
fermi = 1.e-15 * meter

barn = 1.e-28 * meter2
millibarn = 1.e-3 * barn
microbarn = 1.e-6 * barn
nanobarn = 1.e-9 * barn
picobarn = 1.e-12 * barn

# symbols
mm = millimeter
mm2 = millimeter2
mm3 = millimeter3

cm = centimeter
cm2 = centimeter2
cm3 = centimeter3

m = meter
m2 = meter2
m3 = meter3

km = kilometer
km2 = kilometer2
km3 = kilometer3

pc = parsec

#
# Angle
#
radian = 1.
milliradian = 1.e-3 * radian
degree = (3.14159265358979323846 / 180.0) * radian

steradian = 1.

# symbols
rad = radian
mrad = milliradian
sr = steradian
deg = degree

#
# Time [T]
#
nanosecond = 1.
second = 1.e+9 * nanosecond
millisecond = 1.e-3 * second
microsecond = 1.e-6 * second
picosecond = 1.e-12 * second
femtosecond = 1.e-15 * second

hertz = 1. / second
kilohertz = 1.e+3 * hertz
megahertz = 1.e+6 * hertz

# symbols
ns = nanosecond
s = second
ms = millisecond

#
# Electric charge [Q]
#

# positron charge
eplus = 1.
# positron charge in coulomb
e_SI = 1.60217733e-19
# coulomb = 6.24150 e+18 * eplus
coulomb = eplus / e_SI

#
# Energy [E]
#
megaelectronvolt = 1.
electronvolt = 1.e-6 * megaelectronvolt
kiloelectronvolt = 1.e-3 * megaelectronvolt
gigaelectronvolt = 1.e+3 * megaelectronvolt
teraelectronvolt = 1.e+6 * megaelectronvolt
petaelectronvolt = 1.e+9 * megaelectronvolt

# joule = 6.24150 e+12 * MeV
joule = electronvolt / e_SI

# symbols
MeV = megaelectronvolt
eV = electronvolt
keV = kiloelectronvolt
GeV = gigaelectronvolt
TeV = teraelectronvolt
PeV = petaelectronvolt

#
# Mass [E][T^2][L^-2]
#
kilogram = joule * second * second / (meter * meter)
gram = 1.e-3 * kilogram
milligram = 1.e-3 * gram

# symbols
kg = kilogram
g = gram
mg = milligram

#
# Power [E][T^-1]
#

# watt = 6.24150 e+3 * MeV/ns
watt = joule / second

#
# Force [E][L^-1]
#

# newton = 6.24150 e+9 * MeV/mm
newton = joule / meter

#
# Pressure [E][L^-3]
#

# pascal = 6.24150 e+3 * MeV/mm3
hep_pascal = newton / m2
# a trick to avoid warnings
pascal = hep_pascal
# bar    = 6.24150 e+8 * MeV/mm3
bar = 100000 * pascal
# atm    = 6.32420 e+8 * MeV/mm3
atmosphere = 101325 * pascal

#
# Electric current [Q][T^-1]
#

# ampere = 6.24150 e+9 * eplus/ns
ampere = coulomb / second
milliampere = 1.e-3 * ampere
microampere = 1.e-6 * ampere
nanoampere = 1.e-9 * ampere

#
# Electric potential [E][Q^-1]
#
megavolt = megaelectronvolt / eplus
kilovolt = 1.e-3 * megavolt
volt = 1.e-6 * megavolt

#
# Electric resistance [E][T][Q^-2]
#

# Reference: ohm = 1.60217e-16*(MeV/eplus)/(eplus/ns)
ohm = volt / ampere

#
# Electric capacitance [Q^2][E^-1]
#

# Reference: farad = 6.24150e+24 * eplus/Megavolt
farad = coulomb / volt
millifarad = 1.e-3 * farad
microfarad = 1.e-6 * farad
nanofarad = 1.e-9 * farad
picofarad = 1.e-12 * farad

#
# Magnetic Flux [T][E][Q^-1]
#

# Reference: weber = 1000*megavolt*ns
weber = volt * second

#
# Magnetic Field [T][E][Q^-1][L^-2]
#

# Reference: tesla =0.001*megavolt*ns/mm2
tesla = volt * second / meter2

gauss = 1.e-4 * tesla
kilogauss = 1.e-1 * tesla

#
# Inductance [T^2][E][Q^-2]
#
# Reference: henry = 1.60217e-7*MeV*(ns/eplus)**2
henry = weber / ampere

#
# Temperature
#
kelvin = 1.

#
# Amount of substance
#
mole = 1.

#
# Activity [T^-1]
#
becquerel = 1. / second
curie = 3.7e+10 * becquerel

#
# Absorbed dose [L^2][T^-2]
#
gray = joule / kilogram

#
# Luminous intensity [I]
#
candela = 1.

#
# Luminous flux [I]
#
lumen = candela * steradian

#
# Illuminance [I][L^-2]
#
lux = lumen / meter2

#
# Miscellaneous
#
perCent = 0.01
perThousand = 0.001
perMillion = 0.000001
