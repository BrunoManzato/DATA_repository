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


int BinNumber = HitsX->GetNbinsX();

double Nentries = HitsX->GetEntries();

double P_Average, P_Sigma = 0.;

for(int i=0;i<BinNumber;i++){

    P_Average+=(HitsX->GetBinCenter(i))*HitsX->GetBinContent(i)/Nentries;

    P_Sigma += (0.5*HitsX->GetBinContent(i)/Nentries)*(0.5*HitsX->GetBinContent(i)/Nentries)+
               (TMath::Sqrt(HitsX->GetBinContent(i))*HitsX->GetBinCenter(i)/Nentries)*(TMath::Sqrt(HitsX->GetBinContent(i))*HitsX->GetBinCenter(i)/Nentries)+
               ( TMath::Sqrt(Nentries)* (HitsX->GetBinCenter(i))*HitsX->GetBinContent(i)/(Nentries*Nentries) )*( TMath::Sqrt(Nentries)* (HitsX->GetBinCenter(i))*HitsX->GetBinContent(i)/(Nentries*Nentries) )
               ;
}


P_Sigma = TMath::Sqrt(P_Sigma);



arr[0] = P_Sigma;
arr[1] = P_Average;




}


void PondAverage(){


TCanvas*can2 = new TCanvas("can2","",1024,768);
can2->Divide(1,1);



float Sigmas[18] = {0.};

float Means[18] = {0.,0.};

for(int ex = 0; ex<19;ex++){
double arr[2] = {0,0};
GetSigma(ex,arr);

cout<<"Sigma = "<<arr[0] <<endl;
cout<<"Mean = "<<arr[1]<<endl;

Sigmas[ex]=arr[0];
Means[ex] = arr[1];

}


TGraphErrors* PlotMean = new TGraphErrors();
TGraphErrors* CompareSigma = new TGraphErrors();
TGraphErrors* CompareSigmaX = new TGraphErrors();


PlotMean->SetTitle("Average position (fitted) x Number of fibers;Number of Fibers;Position (cm)");

CompareSigma->SetTitle("Sigma of fibers;Number of Fibers;Sigma_Fibers (in Number Of Fibers)");

CompareSigmaX->SetTitle("Sigma of fibers in relation to the number of fibers - Position convertion;Number of Fibers;100cm/FiberNumber * Sigma_Fibers/(Number Of Fibers)");


for(int ex = 0; ex<19;ex++){
    cout<<"Fiber number: "<<10+ex*5<<"     ::   Avrg pos.: "<<Means[ex]<<"   =  "<<Means[ex]/(10+ex*5) <<" +/- "<< Sigmas[ex]/(10+ex*5)<<"      ::   CalculatedMean: "<<100.*Means[ex]/(10+ex*5) <<" +/- "<<100*Sigmas[ex]/(10+ex*5)<<endl;

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
can2->Print("Pond_Avrg_Plot_Compare_Fibers.png");


CompareSigma->Draw("AP");
can2->Print("Pond_Avrg_Plot_Compare_Sigma_Fibers.png");


CompareSigmaX->Draw("AP");
can2->Print("Pond_Avrg_Plot_Compare_Sigma_Fibers_RealPosition.png");


}