//#include "Point.h"
#include "Particle.h"
#include <Riostream.h>
#include <TVector3.h>
#include <TRandom3.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include "Hit.h"

//CONSTANTS
const double pi = 3.141592653589793;
const double c = 29979245800.; // cm/s
const int no_events = 1000;
//Detector & pipe geometries
//All values are in cm
//R: radius (cyl. coord.)
//T: thickness
//L: length
const double R_beampipe = 3.;
const double T_beampipe = .8;
const double R_layer1 = 4.;
const double L_layers = 27.;
const double T_layer1 = .2;
const double R_layer2 = 7.;

/*_____________________________________________________________
_______________________________________________________________
_______________________________________________________________
_______________________________________________________________
_____________MAIN______________________________________________*/

void Simulation(bool multiple_scattering=true, unsigned int seed=8){
	//Simulation parameters
	//All values are in cm
	double sigmaXY = .01;
	double sigmaZ = 5.3;

	if(gRandom){delete gRandom;}
	gRandom = new TRandom3();
	gRandom->SetSeed(seed);
	
	//Creating ROOT file
	TFile f_out("Sim.root","RECREATE");
	
	//Creating tree to save event information
	TTree* tree = new TTree("simulation_tree");
	
	//Allocating memory for TClonesArray
	TClonesArray* pVertex = new TClonesArray("TVector3", 1);
	TClonesArray* pBeamPipe = new TClonesArray("Hit", 100);
	TClonesArray* pLayer1 = new TClonesArray("Hit", 100);
	TClonesArray* pLayer2 = new TClonesArray("Hit", 100);
	
	//Constructing TClonesArrays refernces
	TClonesArray& Vertex = *pVertex;
	TClonesArray& BeamPipe = *pBeamPipe;
	TClonesArray& Layer1 = *pLayer1;
	TClonesArray& Layer2 = *pLayer2;
	
	//Number of particles initialized to 0
	int Npar = 0;
	
	//Branching the tree
	tree->Branch("Multiplicity", &Npar);
	tree->Branch("Vertex", &pVertex);
	tree->Branch("BeamPipe", &pBeamPipe);
	tree->Branch("HitLayer1", &pLayer1);
	tree->Branch("HitLayer2", &pLayer2);
	
	//Initializing vertex
	double xVtx = 0.;
	double yVtx = 0.;
	double zVtx = 0.;
	
	//Initializing direction angles
	double dir_th = 0.;
	double dir_ph = 0.;
	
	for(int ev_idx=0; ev_idx<no_events; ev_idx++){
	
		//Randomly generate 3D point (collision vertex)
		xVtx = gRandom->Gaus(0.,sigmaXY);
		yVtx = gRandom->Gaus(0.,sigmaXY);
		zVtx = gRandom->Gaus(0.,sigmaZ);
		
		//TVector3 particle(xVtx, yVtx, zVtx);
		new(Vertex[0])TVector3(xVtx, yVtx, zVtx);
		
		//Generate multiplicity
		Npar = gRandom->Poisson(99.);
		
		for(int p_idx=0; p_idx<Npar; p_idx++){
			
			//Generate random direction
			dir_th = 2.*atan(exp(2. - 4.*gRandom->Rndm())) + 0.5*pi;
			dir_ph = 2.*pi*gRandom->Uniform(0.,2.*pi);
			
			
			//Direction unit vector
			//TVector3 tmp_dir(sin(dir_th)*cos(dir_ph),sin(dir_th)*sin(dir_ph),cos(dir_ph));
			
			Particle temp(xVtx, yVtx, zVtx, dir_th, dir_ph);			
			
			//Find intersection
			double t_int_bp = FindCylIntersection(temp, R_beampipe);

			//Propagate(particle, t_int_bp*tmp_dir);
			temp.Propagate(t_int_bp);
			
			//Save hit into tree
			new(BeamPipe[p_idx])__Hit__(temp.GetX(),temp.GetY(),temp.GetZ());
			
			//Compute traversed thickness
			double eff_thck = T_beampipe/temp.GetTheta();
			//Propagate to outside layer
			temp.Propagate(eff_thck);
			
			if(multiple_scattering){

				double th0 = Theta0(eff_thck);
				double ph0 = gRandom->Uniform(0.,2.*pi);
				//Change direction
				temp.ChangeDir(th0, ph0);
			
			}
			
			double_t t_int_L1 = FindCylIntersection(temp, R_layer1);
			
			temp.Propagate(t_int_L1);
			
			//if particle exits the system continue loop
			//if particle gets inside the detector save a hit
			if(temp.GetZ()<-L_layers/2 || temp.GetZ()>L_layers/2){continue;}
			else{
				new(HitLayer1[p_idx])Hit(...);
			}
			
			//outer side of cylinder
			eff_thck = T_layer1/temp.GetTheta();
			temp.Propagate(eff_thck);
			
			
			if(multiple_scattering){
			
				double th0 = Theta0(eff_thck);
				double ph0 = gRandom->Gaus();
				//Change direction
				temp.ChangeDir(th0, ph0);
				
			}

			double_t t_int_L2 = FindCylIntersection(temp, R_layer2);
			temp.Propagate(t_int_L2);
			
			//if particle gets inside the detector save a hit
			if(temp.GetZ()<.5*L_layers && temp.GetZ()>-L_layers/2.){
				new(HitLayer2[p_idx])Hit(...);
			}


		}

		Vertex.Clear();
		BeamPipe.Clear();
		Layer1.Clear();
		Layer2.Clear();
		Npar = 0;		
		
	}
	
}

/*_____________________________________________________________
_______________________________________________________________
_______________________________________________________________
_______________________________________________________________
_____________FUNCTIONS_________________________________________*/

double FindCylIntersection(Particle& P0, double R){

	double x0 = P0.GetX();
	double y0 = P0.GetY();
	double z0 = P0.GetZ();
	
	double cd[3] = {sin(P0.GetTheta())*cos(P0.GetPhi()),
					sin(P0.GetTheta())*cos(P0.GetPhi()),
					cos(P0.GetTheta())};
	
	
	//cd[0]=c1, cd[1]=c1, cd[2]=c3
	double delta = (x0*cd[0]+y0*cd[1])*(x0*cd[0]+y0*cd[1])-(x0*x0+y0*y0-R*R)*(cd[0]*cd[0]+cd[1]*cd[1]);
	double t = (-(cd[0]*x0+cd[1]*y0)+sqrt(delta))/(cd[0]*cd[0]+cd[1]*cd[1]);
	
	return t;

}

double IntersectCylinder(TVector3 aPos, TVector3 aDir, double rho){
	double delta = (aPos[0]*aDir[0]+aPos[1]*aDir[1])*(aPos[0]*aDir[0]+aPos[1]*aDir[1])-(aPos[0]*aPos[0]+aPos[1]*aPos[1]-rho*rho)*(aDir[0]*aDir[0]+aDir[1]*aDir[1]);
	double t = (-(aDir[0]*aPos[0]+aDir[1]*aPos[1])+sqrt(delta))/(aDir[0]*aDir[0]+aDir[1]*aDir[1]);
	return t;
}

double Theta0(double thick, double X0, double mom){
	return gRandom->Gaus(13.6*sqrt(thick/X0)*(1.+0.038*ln(thick/X0))/(c*mom));
}
