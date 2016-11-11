// MVA_TMVA_MLP_5_5_5_5.weights.xml
// out_TMVA_MLP_5_5_5_5.root
// h_TMVA_MLP_5_5_5_5.root
//

#include "readCFG.h"

TCut epCut = "ep<12&&ep>0.7";
TCut edCut = "ed>1.5&&ed<12";
TCut dtCut = "dt>1&&dt<400";
TCut distCut = "dist<2500";
TCut sigClass = "classID==0";
TCut bkgClass = "classID==1";

void hist(TString weightFile){

	readCFG cfg;

	bool useTrainSet = cfg.useTrainTree;
	int bins = cfg.bins;
	float xmin = cfg.xmin;
	float xmax = cfg.xmax;

	weightFile.ReplaceAll("./","");
	weightFile.ReplaceAll("weights/MVA_T","T");
	weightFile.ReplaceAll(".weights.xml","");

	TString histFile = TString::Format("./hists/hist_%s.root",weightFile.Data());
	cout << "HISTOGRAM FILE: " << histFile << endl;
	TFile *f_out = new TFile(histFile,"RECREATE");

	TString evalData = TString::Format("./evals/data/evalData_%s.root",weightFile.Data());
	TFile *f_Data = new TFile(evalData,"READ");
	TTree *tr_Data = (TTree*) f_Data->Get("MLP");
	TString s_Data = TString::Format("eval_kMLP>>h_Data(%d,%f,%f)",bins,xmin,xmax); 

	TTree *tr_Sig;
	TTree *tr_Bkg;
	TString s_Sig;
	TString s_Bkg;
	TCut sigCut;
	TCut bkgCut;

	if(useTrainSet){
		TString trainFile = cfg.trainFile;
		TFile *f_Train = new TFile(trainFile,"READ");
		tr_Sig = (TTree*) f_Train->Get("TrainTree");
		tr_Bkg = (TTree*) f_Train->Get("TrainTree");

		
		sigCut = epCut && edCut && dtCut && distCut && sigClass;
		bkgCut = epCut && edCut && dtCut && distCut && bkgClass;

		s_Sig = TString::Format("%s>>h_Sig(%d,%f,%f)",weightFile.Data(),bins,xmin,xmax);	
		s_Bkg = TString::Format("%s>>h_Bkg(%d,%f,%f)",weightFile.Data(),bins,xmin,xmax);	

	

	}else{
		TString evalSig = TString::Format("./evals/sig/evalSig_%s.root",weightFile.Data());
		TString evalBkg = TString::Format("./evals/bkg/evalBkg_%s.root",weightFile.Data());


		TChain *chainSig = new TChain("tr");
		TChain *chainBkg = new TChain("acc");

		chainSig->Add(cfg.sigFile.c_str());
		chainBkg->Add(cfg.bkgFile.c_str());

		TFile *f_Sig = new TFile(evalSig,"READ");
		TFile *f_Bkg = new TFile(evalBkg,"READ");

		tr_Sig = (TTree*) f_Sig->Get("MLP");
		tr_Bkg = (TTree*) f_Bkg->Get("MLP");

		tr_Sig->AddFriend("tr","../../data/mcFormat.root");
		tr_Bkg->AddFriend((TTree*)chainBkg);

		sigCut = epCut&&edCut&&dtCut&&distCut;
		bkgCut = epCut&&edCut&&dtCut&&distCut;

		s_Sig = TString::Format("eval_kMLP>>h_Sig(%d,%f,%f)",bins,xmin,xmax);	
		s_Bkg = TString::Format("eval_kMLP>>h_Bkg(%d,%f,%f)",bins,xmin,xmax);	


	}

	f_out->cd();

	tr_Sig->Draw(s_Sig,sigCut);
	tr_Bkg->Draw(s_Bkg,bkgCut);	
	tr_Data->Draw(s_Data);

	f_out->Write();

	f_out->Close();
	
}

