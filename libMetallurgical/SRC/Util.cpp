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
// $Id: Util.cc,v 1.3 2007-11-19 15:51:28 hb Exp $
//
// $Log: Util.cc,v $
// Revision 1.3  2007-11-19 15:51:28  hb
// all reference elements
//
// Revision 1.2  2007-10-17 13:41:49  jlc
// Adding epsilon = 0 in GetTheGreatest/Lowest/RealRootOfPolynomialEquation
//
// Revision 1.1  2007-10-17 13:28:58  jlc
// Adding Util class in Util namespace for the resolution of polynomial equations of degree 4
//
//

#ifndef __Util__cpp__
#define __Util__cpp__

#include <cmath>
#include <iostream>
#include <iomanip>



#include "Util.hpp"

#include "PolynomialRootFinder.hpp"

namespace Util
  {
  const std::string slash("/");
  const std::string emptyString("");


  Util::Util()
  {}

  Util::~Util()
  {}


  // Original C++ code from Maegis Instinct (maegisinstinct@free.fr)
  // Adapted by J.L CHARLES for std:complex<double> usage.


  double
  Util::cubeRoot(double nb)
  {
    if (nb>0)
      return pow(nb,1.0/3.0);     //pow ne marche pas bien avec des nombre negatifs
    else
      return -pow(-nb,1.0/3.0);
  }


  int
  Util::SolveFirstDegreeEquation(double a, double b,
                                 std::complex<double> & x1)
  {
    assert(a != 0.);
    x1.real() = -b/a;
    x1.imag() = 0.;
    return 1;
  }

  int
  Util::SolveSecondDegreeEquation(double a, double b, double c,
                                  std::complex<double> & x1,
                                  std::complex<double> & x2,bool complexSolution)
  {
    
    /*DEBUG*/ std::cout<<"Solving Second Order Degree Equation with A="<<a<<" B="<<b<<" C="<<c<<"\n";
    if (a == 0)
      return SolveFirstDegreeEquation(b, c, x1);

    const double delta = b*b-4.*a*c;
    const double oneOver2a = 1./(2.*a);
    
     /*DEBUG*/ std::cout<<"sqrt(delta) = "<<sqrt(delta)<<"\n";

    if (delta>=0)   // 2 real roots
      {
        const double sqrtDelta = sqrt(delta);
       
	/*Debut Modifs par MG*/ 
	//Pour eviter erreur d'arrondi: sources: https://fr.wikipedia.org/wiki/%C3%89quation_du_second_degr%C3%A9      Michel Pignat et Jean Vignès, Ingénierie du contrôle de la précision des calculs sur ordinateur.
	/*
	if (b>0)
	{
	  x1.real() = (-b-sqrtDelta)*oneOver2a;
	  x1.imag() = 0.;
	  x2.real() = c/(a*x1.real());
	  x2.imag() = 0.;
	}
	else
	{
	  x1.real() = (-b+sqrtDelta)*oneOver2a;
	  x1.imag() = 0.;
	  x2.real() = c/(a*x1.real());
	  x2.imag() = 0.;
	};
	*/
	/* Fin Modifs par MG*/
	
	/* Debut: Avec method Jenkins-Traub */
	/*
	int degree=2;
	double coefs[degree+1];
	double zeror[degree];
	double zeroi[degree];
	coefs[0]=a;
	coefs[1]=b;
	coefs[2]=c;
	
	rpoly_ak1(coefs, &degree,zeror,zeroi);
	x1.real() = zeror[0];
        x1.imag() = 0.;
        x2.real() = zeror[1];
        x2.imag() = 0.;
	*/
	/* Fin: Avec method Jenkins-Traub */
	
	
	/*Debut Ancienne version par JLC*/
	
	x1.real() = (-b-sqrtDelta)*oneOver2a;
        x1.imag() = 0.;
        x2.real() = (-b+sqrtDelta)*oneOver2a;
        x2.imag() = 0.;
	
	
	/* //debut: dicho
	double f1left, f1right, f2left, f2right, delta, half1, half2, fhalf1, fhalf2, epsilon;
	epsilon= 1e-30;
	delta=0.1;
	f1left= x1.real()- delta;
	f1right=x1.real()+ delta;
	f2left= x2.real()- delta;
	f2right=x2.real()+ delta;
	
	//assert ( (f1left*f1right<0)&& (f2left*f2right<0));
	
	while ( (f1left*f1right<0) && ( (f1right-f1left)<=epsilon ) )
	{
	  half1= (f1left+f1right)/2.;
	  half2= (f2left+f2right)/2.;
	  
	  fhalf1= a*half1*half1 +b*half1 +c;
	  fhalf2= a*half2*half2 +b*half2 +c;
	  
	  if ( fhalf1*f1right <0)
	  {
	    f1left= fhalf1;
	    fhalf1= (fhalf1+f1right)/2.;
	    
	  }
	  else
	  {
	    f1right= fhalf1;
	    fhalf1= (fhalf1+f1left)/2.;
	  };
	  	  
	}
	x1.real()= f1left;
	
	
	while ( (f2left*f2right<0) && ( (f2right-f2left)<=epsilon ) )
	{

	  half2= (f2left+f2right)/2.;
	  
	  fhalf2= a*half2*half2 +b*half2 +c;
	  
	  if ( fhalf2*f2right <0)
	  {
	    f2left= fhalf2;
	    fhalf2= (fhalf2+f2right)/2.;
	  }
	  else
	  {
	    f2right= fhalf2;
	    fhalf2= (fhalf2+f2left)/2.;
	  };
	  
	}
	x2.real()= f2left;
	//Fin: dicho
	*/
	
	/*Fin Ancienne version par JLC*/
      }
    else      // 2 conjugate complex roots
      {
	if (complexSolution)
	{
        x1.real() = -b*oneOver2a;
        x1.imag() = -sqrt(-delta)*oneOver2a;
        x2.real() =  x1.real();
        x2.imag() = -x1.imag();
	}
	else
	{
	  assert (!("No real solutions!!!!"));
	};
      }

    return 2;
  }

  // Solving 3 degrre equation: ax^3 + bx^2 + cx + d = 0
  // There is always at least one real root.
  int Util::SolveThirdDegreeEquation(double a, double b, double c, double d,
                                     std::complex<double> & x1,
                                     std::complex<double> & x2,
                                     std::complex<double> & x3)
  {
    if (a == 0)
      return SolveSecondDegreeEquation(b, c, d, x1, x2);

    // divide all terms by "a" and let x = X - b/(3a)
    // We have then to solve: X^3+pX+q = 0, with

    const double p = (-b*b)/(3*a*a) + c/a;
    const double q = (2*b*b*b)/(27*a*a*a) - (b*c)/(3*a*a) + d/a;

    // relations between roots:
    // X1+X2+X3 = 0
    // X1*X2 + X2*X3 + X1*X3 = p
    // X1*X2*X3 = -q

    // let X = u+v and 3u*v + p = 0
    // By substituting, we get: u^3 + v^3 = q
    // whereas u*v = -p/3 ie u^3*v^3 = -p^3/27

    // We've got sum and product, so we can find u^3 et v^3:
    // u^3 et v^3 are roots of Y^2 + qY -p^3/27 = 0

    const double delta = q*q + 4*p*p*p/27.0;

    // X1 = u+v, is the sum of the cubicroot of solutions
    // And as seen before: X2+X3 = -X1 and X2*X3 = -q/X1
    // so X2 and X3 are roots of equation: Z^2+X1*Z-q/X1
    if (delta>0)
      {
        x1.real() = cubeRoot((-q+sqrt(delta))/2)+cubeRoot((-q-sqrt(delta))/2);
        x1.imag() = 0;
        if (x1.real() != 0)
          {
            SolveSecondDegreeEquation(1, x1.real(), -q/x1.real(), x2, x3);
          }
        else  //sinon on peut factoriser par X (ca donne X^2+p=0)
          {
            SolveSecondDegreeEquation(1, 0, p, x2, x3);
          }
      }
    else if (delta == 0)
      {
        //on ne peut pas rallier ce cas a celui du dessus car le radical cubique d'un complexe
        //est difficile a exprimer (on peut le faire en fonction d'arctan et de sin)
        //Mais si delta == 0 on a une solution double donc en cette solution la dérivée
        //du polynome s'annule aussi en ce point ie
        //3X^2+p=0
        x1.real() = sqrt(-p/3);
        x1.imag() = 0;
        if (x1.real() != 0)
          {
            SolveSecondDegreeEquation(1, x1.real(), -q/x1.real(), x2, x3);
          }
        else  //sinon on peut factoriser par X
          {
            SolveSecondDegreeEquation(1, 0, p, x2, x3);
          }
      }
    else      //trois racines réelles
      {
        //si delta < 0 on fait une autre méthode
        //u^3 et v^3 solutions de l'équation sont des complexes conjugés (delta<0)
        //ecrivons u^3 sous forme exponentielle
        //u^3 = r*e^(it) = -q/2+i*sqrt(delta) (solution de l'eq du deuxieme deg)
        //r est le module egal a la racine carrée de la somme des carrées de la partie
        //imaginaire et réelle, en reprenant l'expression de delta en trouve r=sqrt(-p^3/27)
        //(p<0  car delta < 0)
        //e^(it) = cos(t)+i*sint(t), en identifiant partie imaginaire et réelle
        //on trouve cos(t) = -q/(2r) (t =acos(-q/(2r)))
        //u^3 et v^3 conjugées donc les solutions sont réelles
        //les trois solutions sont les uk+vk avec uk = sqrt(-p/3)*e((t+2*k*M_PI)/3)
        //          vk = uk(barre)
        //Xk = 2*sqrt(-p/3)*cos((t+2k*M_PI)/3)
        //xk = Xk - b/(3a)

        const double r = sqrt(-p/3);
        const double arg = -q/(2*sqrt(-p*p*p/27));
        const double t = acos(arg);

        x1.real() = 2*r*cos(t/3.0);
        x2.real() = 2*r*cos((t+2*M_PI)/3.0);
        x3.real() = 2*r*cos((t+4*M_PI)/3.0);
        x1.imag() = x2.imag() = x3.imag() = 0;
      }

    // Xe've got 3 roots for the equation in term of X, as we stated x=X-b/3a
    // we to compute roots in term of x, adding -b/3a to the real parts:
    const double coef = -b/(3.*a);
    x1.real() += coef;
    x2.real() += coef;
    x3.real() += coef;

    return 3;
  }

  int
  Util::SolveFourthDegreeEquation(double a,double b,double c,double d,double e,
                                  std::complex<double> & x1,
                                  std::complex<double> & x2,
                                  std::complex<double> & x3,
                                  std::complex<double> & x4)
  {
    double Z;
    double u;
    double x,y;

    std::complex<double> xsol1, xsol2, xsol3;
    //
    // TODO
    // Change the way to check a == 0
    //
    if (a == 0)
      return SolveThirdDegreeEquation(b, c, d, e, x1, x2, x3);

    // We divide equation by "a", then replace x by x = X - b/4a
    // It gives an expression like: X^4+AX^2+BX+C with

    const double A = (-3*b*b)/(8*a*a) + c/a;
    const double B = (b*b*b)/(8*a*a*a) - (b*c)/(2*a*a) + d/a;
    const double C = -3*(b*b*b*b)/(256*a*a*a*a) + (c*b*b)/(16*a*a*a) - (d*b)/(4*a*a) +e/a;

    //Si B=0 on sait resoudre ! en prenant Z = X^2
    if (B == 0)
      {
        // Solve equation in Z terms
        SolveSecondDegreeEquation(1, A, C, xsol1, xsol2);

        // Then take + or - the squareroot of the Z sol to get X sol
        // Warning !! sqrt(a+i*b) != sqrt(a)+i*sqrt(b)
        // In general:
        // sqrt (x+iy) =  sqrt(2*(sqrt(x^2+y^2)+x))/2 + i*sqrt(2*(sqrt(x^2+y^2)-x))/2 * sign of y
        // (don't forget to restitute -b/4a to get x sol)

        x = xsol1.real();
        y = xsol1.imag();

        x1.real() = sqrt(2*(sqrt(x*x + y*y)+x))/2;
        x1.imag() = sqrt(2*(sqrt(x*x + y*y)-x))/2;
        if (y < 0) x1.imag() = -x1.imag();

        x2.real() = -x1.real();
        x2.imag() = -x1.imag();

        x = xsol2.real();
        y = xsol2.imag();

        x3.real() = sqrt(2*(sqrt(x*x + y*y)+x))/2;
        x3.imag() = sqrt(2*(sqrt(x*x + y*y)-x))/2;
        if (y < 0) x3.imag() = -x3.imag();

        x4.real() = -x3.real();
        x4.imag() = -x3.imag();
      }
    else
      {

        // On suppose X racine  : on essaye de factoriser X^4+AX^2 en (X^2+u/2)^2
        // or (X^2+u/2)^2 = X^4 + uX^2 + u^2/4
        // et X^4 = -AX^2 - BX - C (car X racine du polynome)
        // Donc au final (X^2+u/2)^2 = (u-A)X^2 - BX + (u^2)/4 - C
        // On a une equation du second degre, on cherche u tel que
        //   * delta = 0
        //  *  u != A

        // On calcule le discriminant et on veut u = 0
        // on tombe sur  u^3 - A*u^2 -4 *C*u + 4*A*C - B^2 = 0
        // si u = A alors B serait egal à 0 ce qui n'est pas le cas (distinction faite plus haut)
        // on calcule les valeurs possibles de u
        SolveThirdDegreeEquation(1, -A, -4*C, 4*A*C-B*B, xsol1, xsol2, xsol3);

        //on prend le u réel le plus grand (voir pour la suite)
        u=xsol1.real();
        //Par convention, ma fonction troisième construit sol3 tq sol3[0] est reel

        if (xsol2.imag() == 0 && xsol2.real() > u)
          u = xsol2.real();
        else if (xsol3.imag() == 0 && xsol3.real() > u)
          u = xsol3.real();


        //delta = 0 donc Z est solution double de (u-A)X^2 - bX + (u^2)/4 - C
        // avec Z = B/(2*(u-A))
        //on peut factoriser

        Z = B/(2*(u-A));

        //(X^2+u/2)^2 = (u-A)(X-Z)^2
        //Il existe au moins un u tel que u>A d'apres l'équation dont u est racine
        //les X solutions sont donc les sols de
        // X^2+u/2 = sqrt (u-A)*(X-Z)
        // X^2+u/2 = -sqrt (u-A)*(X-Z)
        SolveSecondDegreeEquation(1, -sqrt(u-A), u/2 + Z*sqrt(u-A), x1, x2);
        SolveSecondDegreeEquation(1,  sqrt(u-A), u/2 - Z*sqrt(u-A), x3, x4);

        //on fait -4b/a pour chaque solution pour avoir les x
      }
    const double coef = -b/(4*a);
    x1.real() += coef;
    x2.real() += coef;
    x3.real() += coef;
    x4.real() += coef;

    return 4;
  }

  bool
  Util::GetTheGreatestRealRootOfFourthDegreePolynomialEquation(double a,double b,double c,double d,double e, double & root)
  {
    std::complex<double> x[4];
    int n=SolveFourthDegreeEquation(a, b, c, d, e, x[0], x[1], x[2], x[3]);
    assert((a == 0 && n == 3) || (a != 0 && n == 4));

    const double epsilon = 0.;
    //
    // Looking for a real root to initialize root, and later find the greatest
    // real root:
    //
    bool found=false;
    for (int i=0; i<4; ++i)
      {
        if (x[i].imag() <= epsilon)
          {
            root = x[i].real();
            found=true;
            break;
          }
      }
    if (!found)
      {
        // all roots are complex!
        return false;
      }


    //
    // now looking for the greatest real root:
    //
    for (int i=0; i<4; ++i)
      {
        if (x[i].imag() <= epsilon && x[i].real() > root)
          {
            root = x[i].real();
          }
      }
    return true;
  }

  bool
  Util::GetTheLowestRealRootOfFourthDegreePolynomialEquation(double a,double b,double c,double d,double e, double & root)
  {
    std::complex<double> x[4];
    int n=SolveFourthDegreeEquation(a, b, c, d, e, x[0], x[1], x[2], x[3]);
    assert((a == 0 && n == 3) || (a != 0 && n == 4));

    const double epsilon = 0.;
    //
    // Looking for a real root to initialize root, and later find the lowest
    // real root:
    //
    bool found=false;
    for (int i=0; i<4; ++i)
      {
        if (x[i].imag() <= epsilon)
          {
            root = x[i].real();
            found=true;
            break;
          }
      }
    if (!found)
      {
        // all roots are complex!
        return false;
      }

    //
    // now looking for the gretest real root:
    //
    for (int i=0; i<4; ++i)
      {
        if (x[i].imag() <= epsilon && x[i].real() < root)
          {
            root = x[i].real();
          }
      }
    return true;
  }
  
  double 
  Util::DichotomousMethodForSecondDegreeEquation(double a, double b, double c,double solutionToImprove, double leftValue, double rightValue, double epsilon, double alpha)
  {
	//debut: dicho
	double f1left, f1right, delta, f_left, f_right, half, f_half ;
	
	assert((alpha>0)&&(alpha<=1));
	
	

	delta=alpha*(std::abs(rightValue-leftValue));
	f1left= solutionToImprove - delta;
	if ( f1left<leftValue )
	{f1left=leftValue;}
	
	f1right=solutionToImprove + delta;
	if (f1right>rightValue)
	{f1right=rightValue;}
	
	/*DEBUG*/ std::cout<<"In Util::DichotomousMethodForSecondDegreeEquation(): \n ";
	/*DEBUG*/std::cout<<" delta is "<<delta<<"left value is "<<f1left<<" right value is "<< f1right <<"\n";


	
	f_left= a*f1left*f1left + b*f1left + c;
	f_right= a*f1right*f1right + b*f1right +c;
	
	
	/*debug*/ std::cout<<" fleft*fright= "<<f_left*f_right<<" fleft =  "<<f_left<<" fright = "<<f_right<<"\n";
	assert ( (f_left*f_right<0)&&"Solution is not in the range compute from given argument leftValue and rightValue");
	
	while ( (f_left*f_right<0) && ( std::abs(f1right-f1left)>=epsilon ) )
	{
	  /*debug*/ std::cout<<" f1left= "<<f1left<<" f1right ="<<f1right<<"\n";
	  half= (f1left+f1right)/2.;
	  
	  f_half= a*half*half + b*half +c;
	  
	  if ( f_half*f_right <0)
	  {
	    f1left= half;
	    half= (half+f1right)/2.;
	    f_left= a*f1left*f1left + b*f1left + c;
	  }
	  else
	  {
	    f1right= half;
	    half= (half+f1left)/2.;
	    f_right= a*f1right*f1right +b*f1right +c;
	  };
   	  
	}
	/*debug*/
	solutionToImprove= f1left; //or f1right, it is the same
	//Fin: dicho
    
    /*DEBUG:*/std::cout<<"Improved Value Found using dichotomous is ---> "<<solutionToImprove<<" <---\n";
    std::cout<< "P(solutionToImprove)= "<< a*solutionToImprove*solutionToImprove +b*solutionToImprove + c <<"\n";
    return solutionToImprove;
  }
  
  
}


#endif // _CommonLibs_libUtil_Util_hpp_
