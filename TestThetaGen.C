#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>
#include <TStopwatch.h>
#include "ThetaGenerator.h"
/*
NOTA: COMPILARE ThetaGenerator.cxx (.L ThetaGenerator.cxx+)per generare la shared library ThetaGenerator_cxx.so.
Poi sostituire la PATH "/home/alebos/CERNROOT/rinstall/macros/ThetaGenerator_cxx.so" in R__LOAD_LIBRARY con quella personale.
Devo studiare un modo migliore per comunicare all'interpreter di caricare gli oggetti.
Per adesso almeno questo funziona
*/

R__LOAD_LIBRARY(/home/alebos/CERNROOT/rinstall/macros/ThetaGenerator_cxx.so)

//Per runnare il programma basta fare
//root[0] .x TestThetaGen.C

void TestThetaGen(int nEntries = 1000000){

	TH1D* hprova = new TH1D("name","title",200,0.,3.141592653589793);
	
	if(gRandom)delete gRandom;
	
	TStopwatch clock;
	//gROOT->ProcessLine(".L ThetaGenerator.cxx+");
	//gSystem->Load("ThetaGenerator_cxx.so");
	
	ThetaGenerator* thgen = new ThetaGenerator(111);
	
	for(int idx=0; idx<nEntries; idx++){
		hprova->Fill(thgen->ThetaByRejection());
	}
	
	clock.Stop();
	
	//Normalize histogram to compare it with pdf
	double factor = 1.;
	hprova->Scale(factor/hprova->Integral("width"));
	
	//TF1, theta probability density function:
	TF1* g = new TF1("g","(1+tan(x/2)*tan(x/2))/(2*tan(x/2))*(exp(-(-log(tan(x/2))-0.86)*(-log(tan(x/2))-0.86)/(2*1.007*1.007))+exp(-(-log(tan(x/2))+0.86)*(-log(tan(x/2))+0.86)/(2*1.007**2)))/(2*1.007*sqrt(2*TMath::Pi()))",0.,TMath::Pi());
	
	TCanvas* canvas = new TCanvas();
	
	canvas->SetTitle("Simulated data vs. pdf");
	
	g->Draw();
	hprova->Draw("SAME");
	
	clock.Print("u");
	
}
