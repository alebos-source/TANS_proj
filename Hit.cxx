//ROOT includes
#include <TObject.h>
#include <TVector3.h>

ClassImp(Hit)

Hit::Hit():
	fX(0.0),
	fY(0.0),
	fZ(0.0)
{
//Default ctor
}

Hit::Hit(double xx, double yy, double zz):
	fX(xx),
	fY(yy),
	fZ(zz)
{
//Standard ctor
}

Hit::Hit(TVector3 aHit):
	fX(aHit.X();)
	fY(aHit.Y();)
	fZ(aHit.Z();)
{
//Using TVector3 (ROOT)
}
