/**
 * @file Kernel.h
 * @brief defines the collection of fundamental macros from Export.h,
 *        Likely.h and Deprecated.h
 * @author Hubert Degaudenzi - The Euclid Consortium
 */

#ifndef ELEMENTSKERNEL_KERNEL_H
#define ELEMENTSKERNEL_KERNEL_H


#if defined(__linux) || defined(__APPLE__)
#define TEMPLATE_SPECIALIZATION
#endif

#include "ElementsKernel/Export.h"
#include "ElementsKernel/Likely.h"
#include "ElementsKernel/Deprecated.h"

#endif  // ELEMENTSKERNEL_KERNEL_H
