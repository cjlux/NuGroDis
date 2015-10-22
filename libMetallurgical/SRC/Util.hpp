//
// Author(s)    : - Jean-luc CHARLES  LAMEFIP/ENSAM Talence France
//                 <jlc@lamef.bordeaux.ensam.fr>,
//                - Henri BLANC,  ENSAM Talence France,
//                 <henri.blanc@bordeaux.ensam.fr>
//
// Copyright (C) 2007 Jean-luc CHARLES
// Copyright (C) 2007 Henri BLANC
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
//

//
// $Id: Util.hh,v 1.2 2007-11-19 15:51:28 hb Exp $
//
// $Log: Util.hh,v $
// Revision 1.2  2007-11-19 15:51:28  hb
// all reference elements
//
// Revision 1.1  2007-10-17 13:28:58  jlc
// Adding Util class in Util namespace for the resolution of polynomial equations of degree 4
//
//

#ifndef __Util__hpp__
#define __Util__hpp__

#include <cassert>
#include <string>
#include <complex>

namespace Util
  {
  extern  const std::string slash;
  extern  const std::string emptyString;


  class Util
    {
    public:

      Util();
      ~Util();

      static bool GetTheGreatestRealRootOfFourthDegreePolynomialEquation(double a,
          double b,
          double c,
          double d,
          double e,
          double & root);

      static bool GetTheLowestRealRootOfFourthDegreePolynomialEquation(double a,
          double b,
          double c,
          double d,
          double e,
          double & root);

      static double cubeRoot(double);

      static int SolveFirstDegreeEquation(double a, double b,
                                          std::complex<double> & x1);

      static int SolveSecondDegreeEquation(double a, double b, double c,
                                           std::complex<double> & x1,
                                           std::complex<double> & x2,bool complexSolution=true);

      static int SolveThirdDegreeEquation(double a, double b, double c, double d,
                                          std::complex<double> & x1,
                                          std::complex<double> & x2,
                                          std::complex<double> & x3);

      static int SolveFourthDegreeEquation(double a,double b,double c,double d,double e,
                                           std::complex<double> & x1,
                                           std::complex<double> & x2,
                                           std::complex<double> & x3,
                                           std::complex<double> & x4);
      
      static double DichotomousMethodForSecondDegreeEquation(double a, double b, double c,
					   double solutionToImprove,double leftValue, double rightValue, double epsilon=1.e-16, double alpha=0.1);
    };

}


#endif // _CommonLibs_libUtil_Util_hpp_
