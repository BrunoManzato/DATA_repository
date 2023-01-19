#include <iostream>
#include <stdlib.h>
using namespace std;


void GetXPeak(int ex = 0, double arr[2]={}){

//TCanvas*can = new TCanvas("can","",1024,768);
//can->Divide(1,1);


TChain* t = new TChain("Hits_X");

t->Add(Form("output%d.root",ex));

double FiberNumber = 40.;

TH1F*HitsX = new TH1F("HitsX","Hits_X_direction",FiberNumber+6,-1.,FiberNumber+5);

t->Draw("Detector_Position_X>>HitsX");

//cout<<"Fiber Number = "<< FiberNumber<<endl;


//HitsX->Draw("");


TF1*fitter1 = new TF1("fitter1","gaus");
TF1*fitter2 = new TF1("fitter2","gaus");

HitsX->Fit(fitter1);

double mean1 = fitter1->GetParameter(1);
double sigma1 = fitter1->GetParameter(2);

fitter2->SetParameter(0,fitter1->GetParameter(0));
fitter2->SetParameter(1,fitter1->GetParameter(1));
fitter2->SetParameter(2,fitter1->GetParameter(2));


HitsX->Fit(fitter1,"","",mean1-sigma1,mean1+sigma1);

TF1*FinalFit = new TF1("FinalFit","gaus(0)+gaus(3)");

FinalFit->SetParameter(0,fitter1->GetParameter(0));
FinalFit->SetParameter(1,fitter1->GetParameter(1));
FinalFit->SetParameter(2,fitter1->GetParameter(2));


FinalFit->SetParameter(3,fitter2->GetParameter(0));
FinalFit->SetParameter(4,fitter2->GetParameter(1));
FinalFit->SetParameter(5,fitter2->GetParameter(2));


HitsX->Fit(FinalFit,"","",mean1-2*sigma1,mean1+2*sigma1);


fitter1->SetParameter(0,FinalFit->GetParameter(0));
fitter1->SetParameter(1,FinalFit->GetParameter(1));
fitter1->SetParameter(2,FinalFit->GetParameter(2));


fitter2->SetParameter(0,FinalFit->GetParameter(3));
fitter2->SetParameter(1,FinalFit->GetParameter(4));
fitter2->SetParameter(2,FinalFit->GetParameter(5));


//can->cd(1);
//HitsX->Draw("");
//fitter1->Draw("SAME");
//fitter2->Draw("SAME");
//can->Print(Form("Plot_%1.f_Fibers.png",FiberNumber));

arr[0] = FinalFit->GetParameter(0)+FinalFit->GetParameter(3);
arr[1] = FinalFit->GetParameter(1);


}



void GetYPeak(int ex = 0, double arr[2]={}){

//TCanvas*can = new TCanvas("can","",1024,768);
//can->Divide(1,1);


TChain* t = new TChain("Hits_Y");

t->Add(Form("output%d.root",ex));

double FiberNumber = 40.;

TH1F*HitsY = new TH1F("HitsY","Hits_Y_direction",FiberNumber+6,-1.,FiberNumber+5);

t->Draw("Detector_Position_Y>>HitsY");

//cout<<"Fiber Number = "<< FiberNumber<<endl;


//HitsY->Draw("");


TF1*fitter1 = new TF1("fitter1","gaus");
TF1*fitter2 = new TF1("fitter2","gaus");

HitsY->Fit(fitter1);

double mean1 = fitter1->GetParameter(1);
double sigma1 = fitter1->GetParameter(2);

fitter2->SetParameter(0,fitter1->GetParameter(0));
fitter2->SetParameter(1,fitter1->GetParameter(1));
fitter2->SetParameter(2,fitter1->GetParameter(2));


HitsY->Fit(fitter1,"","",mean1-sigma1,mean1+sigma1);

TF1*FinalFit = new TF1("FinalFit","gaus(0)+gaus(3)");

FinalFit->SetParameter(0,fitter1->GetParameter(0));
FinalFit->SetParameter(1,fitter1->GetParameter(1));
FinalFit->SetParameter(2,fitter1->GetParameter(2));


FinalFit->SetParameter(3,fitter2->GetParameter(0));
FinalFit->SetParameter(4,fitter2->GetParameter(1));
FinalFit->SetParameter(5,fitter2->GetParameter(2));


HitsY->Fit(FinalFit,"","",mean1-2*sigma1,mean1+2*sigma1);


fitter1->SetParameter(0,FinalFit->GetParameter(0));
fitter1->SetParameter(1,FinalFit->GetParameter(1));
fitter1->SetParameter(2,FinalFit->GetParameter(2));


fitter2->SetParameter(0,FinalFit->GetParameter(3));
fitter2->SetParameter(1,FinalFit->GetParameter(4));
fitter2->SetParameter(2,FinalFit->GetParameter(5));


//can->cd(1);
//HitsX->Draw("");
//fitter1->Draw("SAME");
//fitter2->Draw("SAME");
//can->Print(Form("Plot_%1.f_Fibers.png",FiberNumber));

arr[0] = FinalFit->GetParameter(0)+FinalFit->GetParameter(3);
arr[1] = FinalFit->GetParameter(1);



}





void Peak(){


TCanvas*can2 = new TCanvas("can2","",1024,768);
can2->Divide(1,1);


TH1F*PeakX = new TH1F("PeakX","Peak - Muon counts - X axis;Number of Photons;Counts",101,-1,100);


TH1F*PeakY = new TH1F("PeakY","Peak - Muon counts - Y axis;Number of Photons;Counts",101,-1,100);


TH1F*MeanPosX = new TH1F("MeanPosX","Mean Position - X axis;Position (mm);Counts",40,-40,40);

TH1F*MeanPosY= new TH1F("MeanPosY","Mean Position - Y axis;Position (mm);Counts",40,-40,40);

for(int i=0;i<100;i++){
double arr[2] = {0,0};
GetXPeak(i,arr);

//cout<<"Mean1 = "<<arr[0] <<endl;
//cout<<"Mean2 = "<<arr[1]<<endl;

PeakX->Fill(arr[0]);
MeanPosX->Fill(arr[1]);
}


for(int i=0;i<100;i++){
double arr[2] = {0,0};
GetYPeak(i,arr);

//cout<<"Mean1 = "<<arr[0] <<endl;
//cout<<"Mean2 = "<<arr[1]<<endl;

PeakY->Fill(arr[0]);
MeanPosY->Fill(arr[1]);
}


can2->cd(1);

gPad->SetGridx(1);
gPad->SetGridy(1);

PeakX->Draw("");
can2->Print("Peak_Xaxis.png");


PeakY->Draw("");
can2->Print("Peak_Yaxis.png");

MeanPosX->Draw("");
can2->Print("PosXaxis.png");

MeanPosY->Draw("");
can2->Print("PosYaxis.png");


}