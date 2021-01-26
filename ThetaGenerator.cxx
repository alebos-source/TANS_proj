//Generator.cxx
//ROOT includes
#include <TRandom3.h>
#include <TMath.h>
#include <Riostream.h>
//
#include "ThetaGenerator.h"



ClassImp(ThetaGenerator);

const double PI = 3.141592653589793;

/*
NOTE ON ASSIGNED PSEUDORAPIDITY DISTRIBUTION

The rapidity p.d.f. can be chosen to be the sum of two Gaussian functions symmetrically displaced with respect to midrapidity, 0., by y0:
f(y) = [exp(-(y-y0)**2/(2*sigma**2))+exp(-(y+y0)**2/(2*sigma**2))]/(2*sigma*sqrt(2*PI)

(See for example https://www.hindawi.com/journals/ahep/2014/863863/ )


We choose:
y0 = 0.860,
sigma = 1.007


In this case, the p.d.f. for theta = th = 2*arctan(exp(-y)) is (function of random variable):

g(th) = f(y(th)) * abs(d(th)/dy) = 
= (1+tan(th/2)**2)/(2*tan(th/2)) * (exp(-(-ln(tan(th/2))-y0)**2/(2*sigma**2))+exp(-(-ln(tan(th/2))+y0)**2/(2*sigma**2)))/(2*sigma*sqrt(2*PI))

for 0 < theta < PI.

To draw g(theta) on ROOT:

TF1* g = new TF1("g","(1+tan(x/2)*tan(x/2))/(2*tan(x/2))*(exp(-(-log(tan(x/2))-0.86)*(-log(tan(x/2))-0.86)/(2*1.007*1.007))+exp(-(-log(tan(x/2))+0.86)*(-log(tan(x/2))+0.86)/(2*1.007**2)))/(2*1.007*sqrt(2*TMath::Pi()))",0.,TMath::Pi());

g->Draw();

In order to generate random directions, theta is extracted from this g using the Acceptance/Rejection method. 
*/

//Standard ctor
ThetaGenerator::ThetaGenerator(unsigned int seed){
	this->SetSeed(seed);
}

//Dtor
ThetaGenerator::~ThetaGenerator() {
	if (gRandom == this) gRandom = 0;
}

//PDF for theta
double ThetaGenerator::ThetaPDF(double xx, double DISPL, double SIGMA){
	//We want this function to be defined for 0 < xx < PI.
	if(xx<0. || xx>PI){
		std::cout << "ERROR: Theta out of range (0, pi) in Generator::ThetaPDF()" << endl;
		return -1;
	}

	else return (1.+tan(xx/2.)*tan(xx/2.))/(2.*tan(xx/2.))*(exp(-(-log(tan(xx/2.))-DISPL)*(-log(tan(xx/2.))-DISPL)/(2.*SIGMA*SIGMA))+exp(-(-log(tan(xx/2.))+DISPL)*(-log(tan(xx/2.))+DISPL)/(2.*SIGMA*SIGMA)))/(2.*SIGMA*sqrt(2.*PI));
}


double ThetaGenerator::ThetaByRejection(){

	//Defining the box
	//double th_min = 0.000000000000000;
	double th_max = 3.141592653589793;
	double f_max = 0.417840086189;
	
	double XX, uu;
	
	do{ //Algorithm:
		//Generate random deviate uniformly distributed between th_min and th_max:
		XX = /*th_min + */ (th_max /*-th_min*/ )*this->Rndm();
		//Generate a second independent random number uniformly distributed between 0 and f_max;
		uu = f_max*this->Rndm();
		
	} while(uu > this->ThetaPDF(XX,.86,1.007));//If uu < ThetaPDF(XX), then accept XX. If not, reject XX and repeat.

	return XX;
}
