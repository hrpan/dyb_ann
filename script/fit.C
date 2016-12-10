TH1 *h_Sig;
TH1 *h_Bkg;
TH1 *h_Data;
TH1 *h_Sum;
// MVA_TMVA_MLP_5_5_5_5.weights.xml
void fit(TString weightFile){

	gStyle->SetOptStat(0);

	weightFile.ReplaceAll("./","");
	weightFile.ReplaceAll("weights/MVA_T","T");
	weightFile.ReplaceAll(".weights.xml","");

	TString histFile = TString::Format("./hists/hist_%s.root",weightFile.Data());
	TString pngFile = TString::Format("./plots/%s.png",weightFile.Data());
	TString resFile = TString::Format("./plots/%s_res.png",weightFile.Data());
	TString ratFile = TString::Format("./plots/%s_rat.png",weightFile.Data());
	TString asyFile = TString::Format("./plots/%s_asy.png",weightFile.Data());

	TF1 *fitf = new TF1("fitf",func,-0.1,1.1,2);
	fitf->SetParameter(0,1);
	fitf->SetParameter(1,1);
	TFile *f_hist = new TFile(histFile,"READ");

	h_Sig = (TH1*) f_hist->Get("h_Sig");	
	h_Sig->SetLineColor(kBlue);

	h_Bkg = (TH1*) f_hist->Get("h_Bkg");
	h_Bkg->SetLineColor(kGreen);

	h_Data = (TH1*) f_hist->Get("h_Data");
	h_Data->Sumw2();	
	h_Data->SetLineColor(kBlack);
	h_Data->SetMarkerStyle(8);
	h_Data->SetMarkerSize(0.3);
	h_Data->Draw("E1");

	gPad->SetLogy();	
	TFitResultPtr r = h_Data->Fit(fitf,"0 S","",0.1,1.1);
	r->Print("v");

	h_Sig->Scale(fitf->GetParameter(0));
	h_Bkg->Scale(fitf->GetParameter(1));

	h_Sum = (TH1*)h_Sig->Clone();	
	h_Sum->Add(h_Bkg);
	h_Sum->SetLineColor(kRed);

	double res[10000];
	h_Data->Chi2Test(h_Sum,"NORM P UU",res);

	TH1 *h_Res = h_Sum->Clone();
	h_Res->SetTitle("Residual");
	for(int i=1;i<=h_Data->GetNbinsX();++i)
		h_Res->SetBinContent(i,res[i-1]);
	

	TH1 *h_Rat = h_Sum->Clone();
	h_Rat->SetTitle("Ratio (MC/DATA)");
	h_Rat->SetMaximum(1.1);
	h_Rat->SetMinimum(0.6);
	h_Rat->Sumw2();
	h_Rat->Divide(h_Data);

	TH1 *h_Asy = h_Sum->GetAsymmetry(h_Data);
	h_Asy->SetTitle("Asymmetry");

	h_Sig->SetFillColor(kBlue);
	h_Sig->SetFillStyle(3005);
	h_Bkg->SetFillColor(kGreen);
	h_Bkg->SetFillStyle(3004);

	h_Sig->Draw("same");
	h_Bkg->Draw("same");
	h_Sum->Draw("same");

	TLegend *leg = new TLegend(0.45,0.6,0.75,0.85);
	leg->AddEntry(h_Sig,"IBD");
	leg->AddEntry(h_Bkg,"Accidental");
	leg->AddEntry(h_Data,"Data");
	leg->AddEntry(h_Sum,"IBD+Accidental");
	leg->SetBorderSize(0);
	leg->Draw("same");
	
	c1->SaveAs(pngFile);
	
	gPad->SetLogy(0);
	h_Res->Draw();	
	c1->SaveAs(resFile);
	h_Rat->Draw("E0");
	c1->SaveAs(ratFile);
	h_Asy->Draw();
	c1->SaveAs(asyFile);
}

double func(double *x, double *par){
	double r1 = *(par);
	double r2 = *(par+1);
	int bin = h_Data->FindBin(*x);

	return r1*h_Sig->GetBinContent(bin)+r2*h_Bkg->GetBinContent(bin);
}

