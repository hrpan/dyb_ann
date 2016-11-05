void plot(){
	TFile *f_train = new TFile("MVA.root","READ");
	TTree *tr_train = (TTree*) f_train->Get("TrainTree");
	TFile *f_data = new TFile("out.root","READ");
	TTree *tr_data = (TTree*) f_data->Get("MLP");
	TFile *f_out = new TFile("h.root","RECREATE");
	f_out->cd();
	tr_train->Draw("TMVA_MLP>>h_Sig(50,-0.1,1.1)","classID==0&&dist<2500&&dt>1");
	tr_train->Draw("TMVA_MLP>>h_Bkg(50,-0.1,1.1)","classID==1&&dist<2500&&dt>1");	
	tr_data->Draw("eval_kMLP>>h_Data(50,-0.1,1.1)");
	f_out->Write();
	f_out->Close();
}
