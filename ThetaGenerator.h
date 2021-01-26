#ifndef _THETAGENERATOR_
#define _THETAGENERATOR_

#include <TRandom3.h>
//


class ThetaGenerator: public TRandom3{ //ThetaGenerator "Is-A" TRandom3

public:

	//Constructor and destructor
	ThetaGenerator(unsigned int seed=2021);//Default ctor
	virtual ~ThetaGenerator(); //Dtor
	
	
	//Probability density functions
	double ThetaPDF(double xx, double DISPL=0.86, double SIGMA=1.007);
	
	//Meth(ods)
	//Return theta angle from assigned pseudorapidity p.d.f.
	//double ThetaByInversion();
	double ThetaByRejection();
	
//private:
	//void Init();
	
	//Data members (servono?)
	
	

ClassDef(ThetaGenerator, 3)

};

#endif
