
void GetPointX(int ex = 0, double arr[2]={}){


TChain* t = new TChain("Hits_X");

t->Add(Form("output%d.root",ex));


TH1F*HitsX = new TH1F(Form("HitsX_%d",ex),"Hits_X_direction;Position (cm);counts ",500,-500,500);

t->Draw(Form("Detector_Position_X>>HitsX_%d",ex));



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



arr[0] = P_Average;
arr[1] = P_Sigma;

P_Sigma=0.;
P_Average=0.;



}




void GetPointY(int ex = 0, double arr[2]={}){


TChain* t = new TChain("Hits_Y");

t->Add(Form("output%d.root",ex));


TH1F*HitsY = new TH1F(Form("HitsY_%d",ex),"Hits_Y_direction;Position (cm);counts ",500,-500,500);

t->Draw(Form("Detector_Position_Y>>HitsY_%d",ex));



int BinNumber = HitsY->GetNbinsX();

double Nentries = HitsY->GetEntries();

double P_Average, P_Sigma = 0.;

for(int i=0;i<BinNumber;i++){

    P_Average+=(HitsY->GetBinCenter(i))*HitsY->GetBinContent(i)/Nentries;

    P_Sigma += (0.5*HitsY->GetBinContent(i)/Nentries)*(0.5*HitsY->GetBinContent(i)/Nentries)+
               (TMath::Sqrt(HitsY->GetBinContent(i))*HitsY->GetBinCenter(i)/Nentries)*(TMath::Sqrt(HitsY->GetBinContent(i))*HitsY->GetBinCenter(i)/Nentries)+
               ( TMath::Sqrt(Nentries)* (HitsY->GetBinCenter(i))*HitsY->GetBinContent(i)/(Nentries*Nentries) )*( TMath::Sqrt(Nentries)* (HitsY->GetBinCenter(i))*HitsY->GetBinContent(i)/(Nentries*Nentries) )
               ;


}


P_Sigma = TMath::Sqrt(P_Sigma);



arr[0] = P_Average;
arr[1] = P_Sigma;

P_Sigma=0.;
P_Average=0.;



}


void PlotPosition(){
    
TCanvas*can = new TCanvas("can","",1024,768);
can->Divide(1,1);

TGraphErrors* PlotPos = new TGraphErrors();


double arrX[2] = {0,0};
double arrY[2] = {0,0};

for(int i=0;i<14;i++){

arrX[0] = 0.;
arrX[1] = 0.;

arrY[0] = 0.;
arrY[1] = 0.;

GetPointX(i,arrX);
GetPointY(i,arrY);

PlotPos->SetPoint(i,50. + arrX[0]/10.,50. + arrY[0]/10.);
PlotPos->SetPointError(i,arrX[1]/10.,arrY[1]/10.);

}

PlotPos->GetXaxis()->SetTitle("Position - X axis (cm)");
PlotPos->GetYaxis()->SetTitle("Position - Y axis (cm)");



TGraph* PlotRef = new TGraph();


double posXref[5] = {10.,30.,50.,70.,90.};
double posYref[3] = {25.,50.,75.};


for(int x = 0; x<5; x++){
    for(int y = 0; y<3; y++){
        PlotRef->SetPoint(3*x+y, posXref[x], posYref[y]);
    }
}

PlotRef->SetMarkerStyle(25);

can->cd(1);

PlotPos->Draw("AP");
PlotRef->Draw("P SAME");

can->Print("Position_Plot.png");

}