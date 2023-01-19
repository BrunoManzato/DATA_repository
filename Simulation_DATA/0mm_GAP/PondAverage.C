#include <iostream>
#include <stdlib.h>
using namespace std;


void GetSigmaPond(int ex = 0, double arr[2]={}){


TCanvas*can = new TCanvas("can","",1024,768);
can->Divide(1,1);


TChain* t = new TChain("Hits_X");

t->Add(Form("output%d.root",ex));

double FiberNumber = 40;

TH1F*HitsX = new TH1F("HitsX","Hits_X_direction",FiberNumber+6,-1.,FiberNumber+5);

t->Draw("Detector_Position_X>>HitsX");



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

P_Sigma=0.;
P_Average=0.;

can->cd(1);
HitsX->Draw("AP");


}


void GetPeakFIT(int ex = 0, double arr[2]={}){

//TCanvas*can = new TCanvas("can","",1024,768);
//can->Divide(1,1);


TChain* t = new TChain("Hits_X");

t->Add(Form("output%d.root",ex));

double FiberNumber = 40.;

TH1F*HitsX = new TH1F("HitsX","Hits_X_direction",FiberNumber+6,-1.,FiberNumber+5);

t->Draw("Detector_Number_X>>HitsX");

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


void PondAverage(){


TCanvas*can2 = new TCanvas("can2","",1024,768);
can2->Divide(1,1);


TGraphErrors* PlotMean = new TGraphErrors();
//PlotMean->SetTitle("Average position (Pond Average) using Number of fibers;Run;Position (cm)");

//TH1F*PlotFitMean = new TH1F("PlotFitMean","Mean fitted with 2 gaussians;Mean;Counts",100,46,60);

//TH1F*HistMean = new TH1F("HistMean","Mean pond. avrg.;Mean;Counts",100,30,50);


PlotMean->SetTitle("Average position (Pond Average in mm) ;Run;Position (mm)");

TH1F*PlotFitMean = new TH1F("PlotFitMean","Mean fitted with 2 gaussians;Mean (channel);Counts",50,16,24);

TH1F*HistMean = new TH1F("HistMean","Mean pond. avrg.;Mean (Position from center in mm);Counts",20,-2,6);

for(int ex = 0; ex<100;ex++){
double arr[2] = {0,0};
double arrfit[2] = {0,0};


GetSigmaPond(ex,arr);

//cout<<"Sigma = "<<arr[0] <<endl;
//cout<<"Mean = "<<arr[1]<<endl;


//PlotMean->SetPoint(ex,ex,(100/40)*arr[1]+0.5*(100/40));
//PlotMean->SetPointError(ex,0.5,(100/40)*arr[0]);

//HistMean->Fill((100/40)*arr[1]+0.5*(100/40));

//GetPeakFIT(ex,arrfit);
//PlotFitMean->Fill(100./40. + (100.*arrfit[1]/40.));



PlotMean->SetPoint(ex,ex,arr[1]);
PlotMean->SetPointError(ex,0.5,arr[0]);

HistMean->Fill(arr[1]);

GetPeakFIT(ex,arrfit);
PlotFitMean->Fill(arrfit[1]);



}


PlotFitMean->Fit("gaus");

PlotMean->SetMarkerStyle(25);
PlotMean->SetMarkerColor(4);

PlotMean->Fit("pol0","","",0,100);

can2->cd(1);

PlotMean->Draw("AP");
can2->Print("Pond_Avrg_Mean.png");

PlotFitMean->Draw("");
can2->Print("Fitted_Mean.png");

HistMean->Draw("");
can2->Print("Hist_Mean.png");

}
