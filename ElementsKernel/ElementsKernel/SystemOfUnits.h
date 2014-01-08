/**
 * @file SystemOfUnits.h
 * @brief Definition of the common units based on MKSA
 * @author Hubert Degaudenzi - The Euclid Consortium
 */

#ifndef ELEMENTSKERNEL_SYSTEMOFUNITS_H_
#define ELEMENTSKERNEL_SYSTEMOFUNITS_H_

#include "ElementsKernel/MathConstants.h"

namespace Elements {
  namespace Units {

    //
    // Length [L]
    //
    static const double meter  = 1.0;
    static const double meter2 = meter*meter;
    static const double meter3 = meter*meter*meter;


    static const double millimeter  = 1.e-3 * meter;
    static const double millimeter2 = millimeter*millimeter;
    static const double millimeter3 = millimeter*millimeter*millimeter;

    static const double centimeter  = 1.e-2 * meter;
    static const double centimeter2 = centimeter*centimeter;
    static const double centimeter3 = centimeter*centimeter*centimeter;

    static const double kilometer = 1000.*meter;
    static const double kilometer2 = kilometer*kilometer;
    static const double kilometer3 = kilometer*kilometer*kilometer;

    static const double parsec = 3.0856775807e+16*meter;

    static const double micrometer = 1.e-6 *meter;
    static const double  nanometer = 1.e-9 *meter;
    static const double  angstrom  = 1.e-10*meter;
    static const double  fermi     = 1.e-15*meter;

    static const double      barn = 1.e-28*meter2;
    static const double millibarn = 1.e-3 *barn;
    static const double microbarn = 1.e-6 *barn;
    static const double  nanobarn = 1.e-9 *barn;
    static const double  picobarn = 1.e-12*barn;

    // symbols
    static const double nm  = nanometer;
    static const double um  = micrometer;

    static const double mm  = millimeter;
    static const double mm2 = millimeter2;
    static const double mm3 = millimeter3;

    static const double cm  = centimeter;
    static const double cm2 = centimeter2;
    static const double cm3 = centimeter3;

    static const double m  = meter;
    static const double m2 = meter2;
    static const double m3 = meter3;

    static const double km  = kilometer;
    static const double km2 = kilometer2;
    static const double km3 = kilometer3;

    static const double pc = parsec;

    //
    // Angle
    //
    static const double radian      = 1.;
    static const double milliradian = 1.e-3*radian;
    static const double degree = (pi/180.0)*radian;

    static const double   steradian = 1.;

    // symbols
    static const double rad  = radian;
    static const double mrad = milliradian;
    static const double sr   = steradian;
    static const double deg  = degree;

    //
    // Time [T]
    //
    static const double second      = 1.;

    static const double nanosecond  = 1.e-9 *second;
    static const double millisecond = 1.e-3 *second;
    static const double microsecond = 1.e-6 *second;
    static const double  picosecond = 1.e-12*second;
    static const double femtosecond = 1.e-15*second;

    static const double hertz = 1./second;
    static const double kilohertz = 1.e+3*hertz;
    static const double megahertz = 1.e+6*hertz;

    // symbols
    static const double ns = nanosecond;
    static const double  s = second;
    static const double ms = millisecond;

    //
    // Electric current [I]
    //
    static const double      ampere = 1.0;
    static const double milliampere = 1.e-3*ampere;
    static const double microampere = 1.e-6*ampere;
    static const double  nanoampere = 1.e-9*ampere;


    //
    // Electric charge [I][T]
    //
    static const double coulomb = ampere * second;
    static const double eplus = 1. ;    // positron charge
    static const double e_SI  = 1.602176565e-19 * coulomb; // positron charge in coulomb


    //
    // Mass [M]
    //
    static const double  kilogram = 1.0;
    static const double      gram = 1.e-3*kilogram;
    static const double milligram = 1.e-3*gram;

    // symbols
    static const double  kg = kilogram;
    static const double   g = gram;
    static const double  mg = milligram;

    //
    // Energy [M][L^2][T^-2]
    //
    static const double joule = kilogram * (meter*meter)/(second*second);
    static const double     electronvolt = joule * e_SI;
    static const double megaelectronvolt = 1.e+6*electronvolt;
    static const double kiloelectronvolt = 1.e+3*electronvolt;
    static const double gigaelectronvolt = 1.e+9*electronvolt;
    static const double teraelectronvolt = 1.e+12*electronvolt;
    static const double petaelectronvolt = 1.e+15*electronvolt;

    // symbols
    static const double MeV = megaelectronvolt;
    static const double  eV = electronvolt;
    static const double keV = kiloelectronvolt;
    static const double GeV = gigaelectronvolt;
    static const double TeV = teraelectronvolt;
    static const double PeV = petaelectronvolt;

    //
    // Power [E][T^-1]
    //
    static const double watt = joule/second;

    //
    // Force [E][L^-1]
    //
    static const double newton = joule/meter;

    //
    // Pressure [E][L^-3]
    //
    static const double Pa         = newton/m2;
    static const double bar        = 100000*Pa;
    static const double atmosphere = 101325*Pa;

    //
    // Electric potential [E][Q^-1]
    //
    static const double     volt = electronvolt/eplus ;
    static const double megavolt = 1.e+6*volt;
    static const double kilovolt = 1.e+3*volt;

    //
    // Electric resistance [E][T][Q^-2]
    //
    static const double ohm = volt/ampere;

    //
    // Electric capacitance [Q^2][E^-1]
    //
    static const double farad = coulomb/volt;
    static const double millifarad = 1.e-3*farad;
    static const double microfarad = 1.e-6*farad;
    static const double  nanofarad = 1.e-9*farad;
    static const double  picofarad = 1.e-12*farad;

    //
    // Magnetic Flux [T][E][Q^-1]
    //
    static const double weber = volt*second;
    //
    // Magnetic Field [T][E][Q^-1][L^-2]
    //
    static const double tesla     = volt*second/meter2;

    static const double gauss     = 1.e-4*tesla;
    static const double kilogauss = 1.e-1*tesla;

    //
    // Inductance [T^2][E][Q^-2]
    //
    static const double henry = weber/ampere;

    //
    // Temperature
    //
    static const double kelvin = 1.;

    //
    // Amount of substance
    //
    static const double mole = 1.;

    //
    // Activity [T^-1]
    //
    static const double becquerel = 1./second ;
    static const double curie = 3.7e+10 * becquerel;

    //
    // Absorbed dose [L^2][T^-2]
    //
    static const double gray = joule/kilogram ;

    //
    // Luminous intensity [I]
    //
    static const double candela = 1.;

    //
    // Luminous flux [I]
    //
    static const double lumen = candela*steradian;

    //
    // Illuminance [I][L^-2]
    //
    static const double lux = lumen/meter2;

    //
    // Miscellaneous
    //
    static const double perCent     = 0.01 ;
    static const double perThousand = 0.001;
    static const double perMillion  = 0.000001;

  }  // namespace Units
} // namespace Elements




#endif /* ELEMENTSKERNEL_SYSTEMOFUNITS_H_ */
