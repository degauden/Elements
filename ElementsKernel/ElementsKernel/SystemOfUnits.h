/**
 * @file SystemOfUnits.h
 * @brief Definition of the common units based on MKSA
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef ELEMENTSKERNEL_SYSTEMOFUNITS_H_
#define ELEMENTSKERNEL_SYSTEMOFUNITS_H_

#include "ElementsKernel/MathConstants.h"

namespace Elements {
  namespace Units {

    //
    // Length [L]
    //
    constexpr double meter  = 1.0;
    constexpr double meter2 = meter*meter;
    constexpr double meter3 = meter*meter*meter;


    constexpr double millimeter  = 1.e-3 * meter;
    constexpr double millimeter2 = millimeter*millimeter;
    constexpr double millimeter3 = millimeter*millimeter*millimeter;

    constexpr double centimeter  = 1.e-2 * meter;
    constexpr double centimeter2 = centimeter*centimeter;
    constexpr double centimeter3 = centimeter*centimeter*centimeter;

    constexpr double kilometer = 1000.*meter;
    constexpr double kilometer2 = kilometer*kilometer;
    constexpr double kilometer3 = kilometer*kilometer*kilometer;

    constexpr double parsec = 3.0856775807e+16*meter;

    constexpr double micrometer = 1.e-6 *meter;
    constexpr double  nanometer = 1.e-9 *meter;
    constexpr double  angstrom  = 1.e-10*meter;
    constexpr double  fermi     = 1.e-15*meter;

    constexpr double      barn = 1.e-28*meter2;
    constexpr double millibarn = 1.e-3 *barn;
    constexpr double microbarn = 1.e-6 *barn;
    constexpr double  nanobarn = 1.e-9 *barn;
    constexpr double  picobarn = 1.e-12*barn;

    // symbols
    constexpr double nm  = nanometer;
    constexpr double um  = micrometer;

    constexpr double mm  = millimeter;
    constexpr double mm2 = millimeter2;
    constexpr double mm3 = millimeter3;

    constexpr double cm  = centimeter;
    constexpr double cm2 = centimeter2;
    constexpr double cm3 = centimeter3;

    constexpr double m  = meter;
    constexpr double m2 = meter2;
    constexpr double m3 = meter3;

    constexpr double km  = kilometer;
    constexpr double km2 = kilometer2;
    constexpr double km3 = kilometer3;

    constexpr double pc = parsec;

    //
    // Angle
    //
    constexpr double radian      = 1.;
    constexpr double milliradian = 1.e-3*radian;
    constexpr double degree = (pi/180.0)*radian;

    constexpr double   steradian = 1.;

    // symbols
    constexpr double rad  = radian;
    constexpr double mrad = milliradian;
    constexpr double sr   = steradian;
    constexpr double deg  = degree;

    //
    // Time [T]
    //
    constexpr double second      = 1.;

    constexpr double nanosecond  = 1.e-9 *second;
    constexpr double millisecond = 1.e-3 *second;
    constexpr double microsecond = 1.e-6 *second;
    constexpr double  picosecond = 1.e-12*second;
    constexpr double femtosecond = 1.e-15*second;

    constexpr double hertz = 1./second;
    constexpr double kilohertz = 1.e+3*hertz;
    constexpr double megahertz = 1.e+6*hertz;

    // symbols
    constexpr double ns = nanosecond;
    constexpr double  s = second;
    constexpr double ms = millisecond;

    //
    // Electric current [I]
    //
    constexpr double      ampere = 1.0;
    constexpr double milliampere = 1.e-3*ampere;
    constexpr double microampere = 1.e-6*ampere;
    constexpr double  nanoampere = 1.e-9*ampere;


    //
    // Electric charge [I][T]
    //
    constexpr double coulomb = ampere * second;
    constexpr double eplus = 1. ;    // positron charge
    constexpr double e_SI  = 1.602176565e-19 * coulomb; // positron charge in coulomb


    //
    // Mass [M]
    //
    constexpr double  kilogram = 1.0;
    constexpr double      gram = 1.e-3*kilogram;
    constexpr double milligram = 1.e-3*gram;

    // symbols
    constexpr double  kg = kilogram;
    constexpr double   g = gram;
    constexpr double  mg = milligram;

    //
    // Energy [M][L^2][T^-2]
    //
    constexpr double joule = kilogram * (meter*meter)/(second*second);
    constexpr double     electronvolt = joule * e_SI;
    constexpr double megaelectronvolt = 1.e+6*electronvolt;
    constexpr double kiloelectronvolt = 1.e+3*electronvolt;
    constexpr double gigaelectronvolt = 1.e+9*electronvolt;
    constexpr double teraelectronvolt = 1.e+12*electronvolt;
    constexpr double petaelectronvolt = 1.e+15*electronvolt;
    constexpr double erg              = 1.e-7*joule;

    // symbols
    constexpr double MeV = megaelectronvolt;
    constexpr double  eV = electronvolt;
    constexpr double keV = kiloelectronvolt;
    constexpr double GeV = gigaelectronvolt;
    constexpr double TeV = teraelectronvolt;
    constexpr double PeV = petaelectronvolt;

    //
    // Power [E][T^-1]
    //
    constexpr double watt = joule/second;

    //
    // Force [E][L^-1]
    //
    constexpr double newton = joule/meter;

    //
    // Pressure [E][L^-3]
    //
    constexpr double Pa         = newton/m2;
    constexpr double bar        = 100000*Pa;
    constexpr double atmosphere = 101325*Pa;

    //
    // Electric potential [E][Q^-1]
    //
    constexpr double     volt = electronvolt/eplus ;
    constexpr double megavolt = 1.e+6*volt;
    constexpr double kilovolt = 1.e+3*volt;

    //
    // Electric resistance [E][T][Q^-2]
    //
    constexpr double ohm = volt/ampere;

    //
    // Electric capacitance [Q^2][E^-1]
    //
    constexpr double farad = coulomb/volt;
    constexpr double millifarad = 1.e-3*farad;
    constexpr double microfarad = 1.e-6*farad;
    constexpr double  nanofarad = 1.e-9*farad;
    constexpr double  picofarad = 1.e-12*farad;

    //
    // Magnetic Flux [T][E][Q^-1]
    //
    constexpr double weber = volt*second;
    //
    // Magnetic Field [T][E][Q^-1][L^-2]
    //
    constexpr double tesla     = volt*second/meter2;

    constexpr double gauss     = 1.e-4*tesla;
    constexpr double kilogauss = 1.e-1*tesla;

    //
    // Inductance [T^2][E][Q^-2]
    //
    constexpr double henry = weber/ampere;

    //
    // Temperature
    //
    constexpr double kelvin = 1.;

    //
    // Amount of substance
    //
    constexpr double mole = 1.;

    //
    // Activity [T^-1]
    //
    constexpr double becquerel = 1./second ;
    constexpr double curie = 3.7e+10 * becquerel;

    //
    // Absorbed dose [L^2][T^-2]
    //
    constexpr double gray = joule/kilogram ;

    //
    // Luminous intensity [I]
    //
    constexpr double candela = 1.;

    //
    // Luminous flux [I]
    //
    constexpr double lumen  = candela*steradian;
    constexpr double jansky = 1.e-26 * watt / (m2*hertz);
    constexpr double microjansky = 1.e-6 * jansky;

    //
    // Illuminance [I][L^-2]
    //
    constexpr double lux = lumen/meter2;

    //
    // Miscellaneous
    //
    constexpr double perCent     = 0.01 ;
    constexpr double perThousand = 0.001;
    constexpr double perMillion  = 0.000001;

  }  // namespace Units
} // namespace Elements




#endif /* ELEMENTSKERNEL_SYSTEMOFUNITS_H_ */
