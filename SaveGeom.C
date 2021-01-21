#include "TCanvas.h"


void SaveGeom(){
	
	TCanvas* c = new TCanvas();

	TGeoManager* geo_mana = new TGeoManager("Assembler", "Detector Setup");
	
	//Useful elements & materials
	//Vacuum
	TGeoMaterial* mat_vac = new TGeoMaterial("Vacuum", 0, 0, 0);
	
	//Beam pipe
	TGeoElement* beryll = new TGeoElement("Beryllium", "Be", 4, 9, 9.012);
	TGeoMaterial* mat_Be = new TGeoMaterial("Beam Pipe", beryll, 1.85);
	
	//Detectors
	TGeoElement* silicon = new TGeoElement("Silicon", "Si", 14, 28, 28.085);
	TGeoMaterial* mat_Si = new TGeoMaterial("Det Layer", silicon, 2.3290);
	
	//Radiation lengths
	mat_Be->SetRadLen(0.);
	mat_Si->SetRadLen(0.);
	
	//Define media
	TGeoMedium* vac = new TGeoMedium("Vacuum", 1, mat_vac);
	TGeoMedium* beam_pipe = new TGeoMedium("Beam Pipe layer", 2, mat_Be);
	TGeoMedium* det_layer = new TGeoMedium("Detector layers", 3, mat_Si);
	
	//Make container volume
	TGeoVolume* top = geo_mana->MakeBox("TOP", vac, 20., 20., 50.);
	geo_mana->SetTopVolume(top);
	
	//Make other volumes
	//Beam pipe
	const double BP_R = 3.; //cm
	const double BP_thck = .08; //cm
	const double BP_half_len = 20.; //cm
	TGeoVolume* bpipe = geo_mana->MakeTube("BeamPipe", beam_pipe, BP_R, BP_R+BP_thck, BP_half_len);
	//Add node
	top->AddNode(bpipe, 1);
	
	//Detector layers
	const double half_len = 13.5; //cm
	const double L_thck = .02; //cm
	
	//First layer
	const double L1_R = 4.; //cm
	TGeoVolume* L1 = geo_mana->MakeTube("Layer1", det_layer, L1_R, L1_R+L_thck, half_len);
	top->AddNode(L1, 1);
	//Second layer
	const double L2_R = 7.; //cm
	TGeoVolume* L2 = geo_mana->MakeTube("Layer2", det_layer, L2_R, L2_R+L_thck, half_len);
	top->AddNode(L2, 1);
	
	top->Draw();
	
}
