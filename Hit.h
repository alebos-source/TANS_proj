#ifndef _HIT_
#define _HIT_

//ROOT includes
#include <TObject.h>
#include <TVector3.h>

class Hit: public TObject{

public:

	Hit();
	Hit(double xx, double yy, double zz);
	Hit(TVector3 aHit);
	virtual ~Hit() {}
	
	//Getters
	double GetX() const {return fX;}
	double GetY() const {return fY;}
	double GetZ() const {return fZ;}
	
	//Other member functions
	//void GausSmear();
	
protected:

	double fX;
	double fY;
	double fZ;
	

ClassDef(Hit,1);

};
#endif
