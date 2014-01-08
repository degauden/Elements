/*
 * PhysConstants.h
 *
 *  Created on: Dec 18, 2013
 *      Author: hubert
 */

#ifndef ELEMENTSKERNEL_PHYSCONSTANTS_H_
#define ELEMENTSKERNEL_PHYSCONSTANTS_H_

#include "ElementsKernel/SystemOfUnits.h"

namespace Elements {
  namespace Units {

    /// @brief \f$N_A\f$
    static const double Avogadro = 6.0221367e+23/mole;

    /// @brief \f$c\f$
    static const double c_light   = 2.99792458e+8 * m/s;
    /// @brief \f$c^2\f$
    static const double c_squared = c_light * c_light;

    /// @brief \f$ h \f$
    static const double h_Planck      = 6.62606957e-34 * joule * s;
    /// @brief \f$ \hbar=h/2\pi\f$
    static const double hbar_Planck   = h_Planck/twopi;
    /// @brief \f$ \hbar c\f$
    static const double hbarc         = hbar_Planck * c_light;
    /// @brief \f$ (\hbar c)^2\f$
    static const double hbarc_squared = hbarc * hbarc;

    /// @brief \f$ -e \f$
    static const double electron_charge = - eplus; // see SystemOfUnits.h
    /// @brief \f$ e^2 \f$
    static const double e_squared = eplus * eplus;

    /// @brief \f$ m_e \f$
    static const double electron_mass = 9.10938291e-31 * kg;
    /// @brief \f$ m_p \f$
    static const double   proton_mass = 1.672621777e-27 * kg;
    static const double           amu = 1.660538921e-27 * kg;

    /// @brief permeability of free space \f$\mu_0\f$
    static const double mu0      = 4*pi*1.e-7 * henry/m;
    /// @brief permeability of free space \f$\epsilon_0=1/(c^2\mu_0)\f$
    static const double epsilon0 = 1./(c_squared*mu0);

    static const double elm_coupling           = e_squared/(4*pi*epsilon0);
    /// @brief \f$ \alpha=e^2/(4\pi\epsilon_0\hbar c)\f$
    static const double fine_structure_const   = elm_coupling/hbarc;


    static const double k_Boltzmann = 1.3806488e-23 * joule/kelvin ;

    /// @brief Standard Temperature and Pressure
    static const double STP_Temperature = 273.15*kelvin;
    static const double STP_Pressure    = 1.*atmosphere;
    static const double kGasThreshold   = 10.*mg/cm3;

    static const double universe_mean_density = 1.e-25*g/cm3;

  }  // namespace Units
} // namespace Elements





#endif /* ELEMENTSKERNEL_PHYSCONSTANTS_H_ */
