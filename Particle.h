#ifndef _PARTICLE_
#define _PARTICLE_
#include <TVector3.h>

class Particle{
	private:
		//Particle "Has-A" TVector3
		TVector3 fPos; //position three-vector
		TVector3 fMom; //momentum three-vector
		double fKinE; //kinetic energy
		double fMass; //mass
		//double fCharge; //electric charge
		
	public:
		Particle(); //Default constructor
		Particle(TVector3 aPos, TVector3 aMom); //standard constructor
		Particle(double x, double y, double z, double px, double py, double pz);
		Particle(TVector3 aPos, TVector3 aDir, double kinE, double fMass);
		
		Particle(const Particle& aPar); //copy constructor
		Particle& operator=(const Particle& aPar); // = operator
		virtual ~Particle(); //destructor
		
		//Methods
		//GETTERS
		inline double Particle::GetX() const {return fPos.X();} 
		//get x coordinate of particle position
		inline double Particle::GetY() const {return fPos.Y();} 
		//get y coordinate of particle position
		inline double Particle::GetZ() const {return fPos.Z();} 
		//get z coordinate of particle position
		inline double Particle::GetTheta() const {return fMom.Theta();} 
		//get theta angle of particle direction
		inline double Particle::GetPhi() const {return fMom.Phi();}
		//get phi angle of particle direction
		inline TVector3 Particle::GetPosition() const {return fPos;}
		//return position vector (3D)
		inline TVector3 Particle::GetDirection() const {return fMom.Unit();}
		//return direction vector (3D)
	
		//SETTERS
		inline void Particle::SetPosition(TVector3 aPos){fPos = aPos;}
		inline void Particle::SetXYZ(double xx, double yy, double zz){fPos = {xx,yy,zz};}
		inline void Particle::SetDirection()
		void 
		
}
