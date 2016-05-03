/*
 * @file ElementsKernel/PhysConstants.h
 *
 *  Created on: Dec 18, 2013
 *      Author: hubert
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

#ifndef ELEMENTSKERNEL_PHYSCONSTANTS_H_
#define ELEMENTSKERNEL_PHYSCONSTANTS_H_

#include "ElementsKernel/SystemOfUnits.h"

namespace Elements {
  namespace Units {

    /// @brief \f$N_A\f$
    constexpr double Avogadro = 6.0221367e+23/mole;

    /// @brief \f$c\f$
    constexpr double c_light   = 2.99792458e+8 * m/s;
    /// @brief \f$c^2\f$
    constexpr double c_squared = c_light * c_light;

    /// @brief \f$ h \f$
    constexpr double h_Planck      = 6.62606957e-34 * joule * s;
    /// @brief \f$ \hbar=h/2\pi\f$
    constexpr double hbar_Planck   = h_Planck/twopi;
    /// @brief \f$ \hbar c\f$
    constexpr double hbarc         = hbar_Planck * c_light;
    /// @brief \f$ (\hbar c)^2\f$
    constexpr double hbarc_squared = hbarc * hbarc;

    /// @brief \f$ -e \f$
    constexpr double electron_charge = - eplus; // see SystemOfUnits.h
    /// @brief \f$ e^2 \f$
    constexpr double e_squared = eplus * eplus;

    /// @brief \f$ m_e \f$
    constexpr double electron_mass = 9.10938291e-31 * kg;
    /// @brief \f$ m_p \f$
    constexpr double   proton_mass = 1.672621777e-27 * kg;
    constexpr double           amu = 1.660538921e-27 * kg;

    /// @brief permeability of free space \f$\mu_0\f$
    constexpr double mu0      = 4*pi*1.e-7 * henry/m;
    /// @brief permeability of free space \f$\epsilon_0=1/(c^2\mu_0)\f$
    constexpr double epsilon0 = 1./(c_squared*mu0);

    constexpr double elm_coupling           = e_squared/(4*pi*epsilon0);
    /// @brief \f$ \alpha=e^2/(4\pi\epsilon_0\hbar c)\f$
    constexpr double fine_structure_const   = elm_coupling/hbarc;


    constexpr double k_Boltzmann = 1.3806488e-23 * joule/kelvin;

    /// @brief Standard Temperature and Pressure
    constexpr double STP_Temperature = 273.15*kelvin;
    constexpr double STP_Pressure    = 1.*atmosphere;
    constexpr double kGasThreshold   = 10.*mg/cm3;

    constexpr double universe_mean_density = 1.e-25*g/cm3;

  }  // namespace Units
} // namespace Elements





#endif /* ELEMENTSKERNEL_PHYSCONSTANTS_H_ */
