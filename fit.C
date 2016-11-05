TH1 *h_Sig;
TH1 *h_Bkg;
TH1 *h_Data;

void fit(){
	TF1 *fitf = new TF1("fitf",func,-0.1,1.1,2);
	TFile *f_hist = new TFile("h.root","READ");

	h_Sig = (TH1*) f_hist->Get("h_Sig");	
	h_Sig->SetLineColor(kBlue);

	h_Bkg = (TH1*) f_hist->Get("h_Bkg");
	h_Bkg->SetLineColor(kGreen);

	h_Data = (TH1*) f_hist->Get("h_Data");	
	h_Data->SetLineColor(kBlack);
	h_Data->SetMarkerStyle(8);
	h_Data->SetMarkerSize(0.5);
	h_Data->Draw("E1");

	gPad->SetLogy();	
	h_Data->Fit(fitf,"0");

	h_Sig->Scale(fitf->GetParameter(0));
	h_Bkg->Scale(fitf->GetParameter(1));

	TH1 *h_Sum = h_Sig->Clone();
	h_Sum->Add(h_Bkg);
	h_Sum->SetLineColor(kRed);	

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

	c1->SaveAs("./plots/MLP_fit.png");
}

double func(double *x, double *par){
	double r1 = *(par);
	double r2 = *(par+1);
	int bin = h_Sig->FindBin(*x);
	return r1*h_Sig->GetBinContent(bin)+r2*h_Bkg->GetBinContent(bin);
}
