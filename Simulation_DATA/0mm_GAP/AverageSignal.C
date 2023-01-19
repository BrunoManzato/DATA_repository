#include <iostream>
#include <stdlib.h>
using namespace std;


void AverageSignal(){


TCanvas*can2 = new TCanvas("can2","",1024,768);
can2->Divide(1,1);


TH1F*MeanPosX = new TH1F("MeanPosX","Mean Position - X axis;Position (mm);Counts",40,-500,500);

TH1F*MeanPosY= new TH1F("MeanPosY","Mean Position - Y axis;Position (mm);Counts",40,-500,500);




TChain* ty = new TChain("Hits_Y");

ty->Add("output*");


ty->Draw("Detector_Position_Y>>MeanPosY");



TChain* tx = new TChain("Hits_X");

tx->Add("output*");


tx->Draw("Detector_Position_X>>MeanPosX");


MeanPosX->Scale(1./100.);

MeanPosY->Scale(1./100.);


can2->cd(1);

gPad->SetGridx(1);
gPad->SetGridy(1);

MeanPosX->Draw("B E L");
can2->Print("MeanSIgnal_X.png");

MeanPosY->Draw("B E L");
can2->Print("MeanSIgnal_Y.png");


}
