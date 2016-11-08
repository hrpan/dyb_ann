// MVA_TMVA_MLP_5_5_5_5.weights.xml
// out_TMVA_MLP_5_5_5_5.root
// h_TMVA_MLP_5_5_5_5.root
void hist(TString weightFile){

	weightFile.ReplaceAll("./","");
	weightFile.ReplaceAll("weights/MVA_T","T");
	weightFile.ReplaceAll(".weights.xml","");

	int bins = 500;
	float xmin = -0.1;
	float xmax = 1.1;

	TString evalSig = TString::Format("./evals/sig/evalSig_%s.root",weightFile.Data());
	TFile *f_Sig = new TFile(evalSig,"READ");
	TTree *tr_Sig = (TTree*) f_Sig->Get("MLP");
	tr_Sig->AddFriend("tr","../../data/mcFormat.root");

	TString evalBkg = TString::Format("./evals/bkg/evalBkg_%s.root",weightFile.Data());
	TFile *f_Bkg = new TFile(evalBkg,"READ");
	TTree *tr_Bkg = (TTree*) f_Bkg->Get("MLP");
	TChain *chainBkg = new TChain("acc");
	chainBkg->Add("../acc/data_pair_sig/*.root");
	tr_Bkg->AddFriend((TTree*)chainBkg);


	TString evalData = TString::Format("./evals/data/evalData_%s.root",weightFile.Data());
	TFile *f_Data = new TFile(evalData,"READ");
	TTree *tr_Data = (TTree*) f_Data->Get("MLP");


	TString histFile = TString::Format("./hists/hist_%s.root",weightFile.Data());
	cout << "HISTOGRAM FILE: " << histFile << endl;
	TFile *f_out = new TFile(histFile,"RECREATE");
	f_out->cd();
	
	TCut epCut = "ep<12&&ep>0.7";
	TCut edCut = "ed>1.5&&ed<12";
	TCut dtCut = "dt>1&&dt<400";
	TCut distCut = "dist<2500";
	TCut fullCut = epCut&&edCut&&dtCut&&distCut;

	TString s_SigTree = TString::Format("eval_kMLP>>h_Sig(%d,%f,%f)",bins,xmin,xmax);	
	TString s_BkgTree = TString::Format("eval_kMLP>>h_Bkg(%d,%f,%f)",bins,xmin,xmax);	
	TString s_DataTree = TString::Format("eval_kMLP>>h_Data(%d,%f,%f)",bins,xmin,xmax); 
	tr_Sig->Draw(s_SigTree,fullCut);
	tr_Bkg->Draw(s_BkgTree,fullCut);	
	tr_Data->Draw(s_DataTree);
	f_out->Write();
	f_out->Close();
}
