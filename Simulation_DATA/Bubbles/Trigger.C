#include <iostream>
#include <stdlib.h>
using namespace std;


void Trigger(){



TCanvas*can = new TCanvas("can","",1024,768);
can->Divide(1,1);

///////////////////////X axis

TH1F*EventNumber = new TH1F("EventNumber","Number of Events in SiPM - X axis; No of Events",101,-1,100);


TChain* t[100];

TH1F*GetInfo[100];

int Nbins=0;

for(int i=0;i<100;i++){

GetInfo[i] = new TH1F(Form("GetInfo_%d",i),"",40,0,40);

t[i]= new TChain("Hits_X");

t[i]->Add(Form("output%d.root",i));

t[i]->Draw(Form("Detector_Number_X>>GetInfo_%d",i));

Nbins = GetInfo[i]->GetNbinsX();

    for(int j = 0; j<Nbins; j++){

        EventNumber->Fill(GetInfo[i]->GetBinContent(j));

    }


}

double EntriesX = EventNumber->GetEntries();

double totalX = 0.;

TGraph*PlotXpercent = new TGraph();

for(int i=0; i < EventNumber->GetNbinsX(); i++){

    totalX+=EventNumber->GetBinContent(i);

    PlotXpercent->SetPoint(i,EventNumber->GetBinCenter(i),100*totalX/EntriesX);

}


PlotXpercent->SetMarkerStyle(20);
PlotXpercent->SetMarkerColor(4);
PlotXpercent->SetTitle("Percentage integral of Number of photons;Number of photons;Percentage");

TLine*percentLine = new TLine(-1,70,100,70);
percentLine->SetLineColor(3);


TLine*percentLine95 = new TLine(-1,95,100,95);
percentLine95->SetLineColor(2);

EventNumber->Fit("landau");

can->cd(1);

//gPad->SetLogy();

EventNumber->Draw("");
can->Print("NmberOfPhotons_Xaxis.png");


PlotXpercent->Draw("AP");
percentLine->Draw("SAME");
percentLine95->Draw("SAME");

can->Print("PercentPointsXaxis.png");



/////////////////////////////////Y axis


TH1F*EventNumberY = new TH1F("EventNumberY","Number of Events in SiPM - Y axis; No of Events",101,-1,100);


TChain* tY[100];

TH1F*GetInfoY[100];

int NbinsY=0;

for(int i=0;i<100;i++){

GetInfoY[i] = new TH1F(Form("GetInfoY_%d",i),"",40,0,40);

tY[i]= new TChain("Hits_Y");

tY[i]->Add(Form("output%d.root",i));

tY[i]->Draw(Form("Detector_Number_Y>>GetInfoY_%d",i));

NbinsY = GetInfoY[i]->GetNbinsX();

    for(int j = 0; j<NbinsY; j++){

        EventNumberY->Fill(GetInfoY[i]->GetBinContent(j));

    }


}



EventNumberY->Fit("landau");

can->cd(1);

gPad->SetLogy();

EventNumberY->Draw("");
can->Print("NmberOfPhotons_Yaxis.png");



}