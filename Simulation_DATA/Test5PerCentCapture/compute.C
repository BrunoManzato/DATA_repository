#include <iostream>
#include <stdlib.h>
using namespace std;


void GetSigma(int ex = 0, double arr[2]={}){


TCanvas*can = new TCanvas("can","",1024,768);
can->Divide(1,1);


TChain* t = new TChain("Hits_X");

t->Add(Form("output%d.root",ex));

double FiberNumber = 10+5*ex;

TH1F*HitsX = new TH1F("HitsX","Hits_X_direction",FiberNumber+6,-1.,FiberNumber+5);

t->Draw("Detector_Number_X>>HitsX");

cout<<"Fiber Number = "<< FiberNumber<<endl;


HitsX->Draw("");


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

//FinalFit->Draw("SAME");
//HitsX->Fit(FinalFit,"","",mean1-2*sigma1,mean1+2*sigma1);
HitsX->Fit(FinalFit,"","",mean1-2*sigma1,mean1+2*sigma1);



fitter1->SetParameter(0,FinalFit->GetParameter(0));
fitter1->SetParameter(1,FinalFit->GetParameter(1));
fitter1->SetParameter(2,FinalFit->GetParameter(2));


fitter2->SetParameter(0,FinalFit->GetParameter(3));
fitter2->SetParameter(1,FinalFit->GetParameter(4));
fitter2->SetParameter(2,FinalFit->GetParameter(5));


can->cd(1);
HitsX->Draw("");
fitter1->Draw("SAME");
fitter2->Draw("SAME");
can->Print(Form("Plot_%1.f_Fibers.png",FiberNumber));

arr[0] = FinalFit->GetParameter(1);
arr[1] = abs(FinalFit->GetParameter(2));




}


void compute(){


TCanvas*can2 = new TCanvas("can2","",1024,768);
can2->Divide(1,1);



float Sigmas[18] = {0.};

float Means[18] = {0.,0.};

for(int ex = 0; ex<19;ex++){
double arr[2] = {0,0};
GetSigma(ex,arr);

cout<<"Sigma = "<<arr[0] <<endl;
cout<<"Mean = "<<arr[1]<<endl;

Sigmas[ex]=arr[1];
Means[ex] = arr[0];

}


TGraphErrors* PlotMean = new TGraphErrors();
TGraphErrors* CompareSigma = new TGraphErrors();
TGraphErrors* CompareSigmaX = new TGraphErrors();


PlotMean->SetTitle("Average position (fitted) x Number of fibers;Number of Fibers;Position (cm)");

CompareSigma->SetTitle("Sigma of fibers;Number of Fibers;Sigma_Fibers (in Number Of Fibers)");

CompareSigmaX->SetTitle("Sigma of fibers in relation to the number of fibers - Position convertion;Number of Fibers;100cm/FiberNumber * Sigma_Fibers/(Number Of Fibers)");


for(int ex = 0; ex<19;ex++){
    cout<<"Fiber number: "<<10+ex*5<<"     ::   Avrg pos.: "<<Means[ex]<<"   =  "<<Means[ex]/(10+ex*5) <<" +/- "<< Sigmas[ex]/(10+ex*5)<<"      ::   CalculatedMean: "<<100.*Means[ex]/(10+ex*5) <<endl;

}

for(int ex = 1; ex<19;ex++){


    PlotMean->SetPoint(ex-1,10+ex*5,100.*Means[ex]/(10+ex*5));
    PlotMean->SetPointError(ex-1,2.5,100.*Sigmas[ex]/(10+ex*5));

    CompareSigma->SetPoint(ex-1,10+ex*5,Sigmas[ex]);
    CompareSigma->SetPointError(ex-1,2.5,Sigmas[ex]*0.1);


    CompareSigmaX->SetPoint(ex-1,10+ex*5,100*Sigmas[ex]/(10+ex*5));
    CompareSigmaX->SetPointError(ex-1,2.5,100*(Sigmas[ex]/TMath::Sqrt(10+ex*5))/(10+ex*5));




}

PlotMean->SetMarkerStyle(25);
PlotMean->SetMarkerColor(4);

CompareSigma->SetMarkerStyle(25);
CompareSigma->SetMarkerColor(2);


CompareSigmaX->SetMarkerStyle(25);
CompareSigmaX->SetMarkerColor(2);



can2->cd(1);

gPad->SetGridx(1);
gPad->SetGridy(1);

PlotMean->Draw("AP");
can2->Print("Plot_Compare_Fibers.png");


CompareSigma->Draw("AP");
can2->Print("Plot_Compare_Sigma_Fibers.png");


CompareSigmaX->Draw("AP");
can2->Print("Plot_Compare_Sigma_Fibers_RealPosition.png");


}