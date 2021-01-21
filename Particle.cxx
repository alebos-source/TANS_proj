//Particle.cxx
#include <TVector3.h>
#include <Particle.h>
#include <TMath.h>

ClassImp(Particle)

//Default ctor
Particle::Particle():
	fPos(0.,0.,0.),
	fMom(0.,0.,0.)
{}

//Standard ctor
Particle::Particle(double x, double y, double z, double th, double ph):
	fPos(x,y,z),
	fMom(sin(th)*cos(ph),sin(th)*sin(ph),cos(ph))
{}

//ctor
Particle::Particle(TVector3 position, TVector3 direction):
	fPos(position),
	fMom(direction)
{}

//Copy constructor
Particle::Particle(const Particle& par){
	fPos(par.fPos),
	fMom(par.fMom)
}


//Dtor
Particle::~Particle()
{}

void Particle::ChangeDir(double thp, double php){
	double RM[3][3];
	double th = fMom.Theta();
	double ph = fMom.Phi();
	
	RM[0][0] = -sin(ph);
	RM[0][1] = -cos(ph)*cos(th);
	RM[0][2] = cos(ph)*sin(th);
	RM[1][0] = cos(ph);
	RM[1][1] = -sin(ph)*cos(th);
	RM[1][2] = sin(th)*sin(ph);
	RM[2][0] = 0.;
	RM[2][1] = sin(th);
	RM[2][2] = cos(th);
	
	double dcp[3];
	dcp[0] = sin(thp)*cos(php);
	dcp[1] = sin(thp)*sin(php);
	dcp[2] = cos(thp);
	
	for(int i=0; i<3; i++){
		fMom[i] = 0.;
		for(int j=0; j<3; j++){
			fMom[i] += RM[i][j]*dcp[j];
		}
	}
}

void Particle::Propagate(double dist){
	fPos += this->GetDirection()*dist;
}
