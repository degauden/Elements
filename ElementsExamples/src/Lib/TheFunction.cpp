/*
 * TheFunction.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: hubert
 */

#include <iostream>

using namespace std;

#include "ElementsExamples/TheFunction.h"

int theFunction(int a) {
  cout << "This is a:" << a << endl ;
  return a++ ;
}
