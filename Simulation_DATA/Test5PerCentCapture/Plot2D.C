#include <iostream>
#include <stdlib.h>
using namespace std;


void Plot2D(int ex = 0){

ex=18;

TCanvas*can = new TCanvas("can","",1024,768);
can->Divide(1,1);


TChain* Xside = new TChain("Hits_X");

Xside->Add(Form("output%d.root",ex));



TChain* Yside = new TChain("Hits_Y");

Yside->Add(Form("output%d.root",ex));


double FiberNumber = 10+5*ex;

TH1F*HitsX = new TH1F("HitsX","Hits_X_direction",FiberNumber+6,-1.,FiberNumber+5);

Xside->Draw("Detector_Number_X>>HitsX");


TH1F*HitsY = new TH1F("HitsY","Hits_Y_direction",FiberNumber+6,-1.,FiberNumber+5);

Yside->Draw("Detector_Number_Y>>HitsY");

int nBins = FiberNumber+5;
cout<<"Number of bins: "<<nBins<<endl;
double HitsXValue[nBins];
double HitsXPosition[nBins];
double HitsYValue[nBins];
double HitsYPosition[nBins];

for(int i=0;i<nBins;i++){

HitsXValue[i]=HitsX->GetBinContent(i);
HitsXPosition[i] = HitsX->GetBinCenter(i);


HitsYValue[i]=HitsY->GetBinContent(i);
HitsYPosition[i] = HitsY->GetBinCenter(i);
cout<<"Position: "<<HitsYPosition[i]<<"    ::   Value: "<<HitsYValue[i]<<endl;
}


TH2F*HitsXxY = new TH2F("HitsXxY","Hits X position x Y position",FiberNumber+6,-1.,FiberNumber+5,FiberNumber+6,-1.,FiberNumber+5);

for(int i=0;i<nBins;i++){
    for(int j=0;j<nBins;j++){

        HitsXxY->Fill(HitsXPosition[i],HitsYPosition[j],HitsXValue[i]+HitsYValue[j]);

    }


}

can->cd(1);
HitsXxY->Draw("");

can->Print(Form("2DPlot_%dFibers.png",ex));

}