#include "readCFG.h"
float ep,ed,dt,dist;

void eval(TString weightFile){

	readCFG cfg;

	TMVA::Reader *reader = new TMVA::Reader();
	reader->AddVariable("ep",&ep);
	reader->AddVariable("ed",&ed);
	reader->AddVariable("dt",&dt);
	reader->AddVariable("dist",&dist);

	reader->BookMVA("kMLP",weightFile.Data());

	weightFile.ReplaceAll("./","");
	weightFile.ReplaceAll("weights/MVA_","");
	weightFile.ReplaceAll(".weights.xml","");


	TChain *chainData = new TChain("IBD");
	TChain *chainBkg = new TChain("acc");
	TChain *chainSig = new TChain("tr");
	
	cout << cfg.dataFile << endl;
	chainData->Add(cfg.dataFile.c_str());
	chainSig->Add(cfg.sigFile.c_str());
	chainBkg->Add(cfg.bkgFile.c_str());	

	//TTree *tr_Data = (TTree*) fin->Get("IBD");
	TTree *tr_Data = (TTree*) chainData;
	setAddress(tr_Data);

	TTree *tr_Sig = (TTree*) chainSig;
	setAddress(tr_Sig);

	TTree *tr_Bkg = (TTree*) chainBkg;
	setAddress(tr_Bkg);

	TString evalData = TString::Format("./evals/data/evalData_%s.root",weightFile.Data());
	if(cfg.evalData)
		fillTree(reader,tr_Data,evalData);


	TString evalSig = TString::Format("./evals/sig/evalSig_%s.root",weightFile.Data());
	if(cfg.evalSig)
		fillTree(reader,tr_Sig,evalSig);

	TString evalBkg = TString::Format("./evals/bkg/evalBkg_%s.root",weightFile.Data());
	if(cfg.evalBkg)
		fillTree(reader,tr_Bkg,evalBkg);
}

void setAddress(TTree *tr){
	tr->SetBranchAddress("ep",&ep);
	tr->SetBranchAddress("ed",&ed);
	tr->SetBranchAddress("dt",&dt);
	tr->SetBranchAddress("dist",&dist);
}

void fillTree(TMVA::Reader *reader, TTree *tr_in, TString &fileName){

	cout << "EVALUATING FILE:" << fileName << endl;

	TFile *fout = new TFile(fileName,"RECREATE");
	TTree *tr_out = new TTree("MLP","MLP");

	double eval_kMLP = 0;
	tr_out->Branch("eval_kMLP",&eval_kMLP);

	int nevts = tr_in->GetEntries();
	for(int i=0;i<nevts;++i){
		tr_in->GetEntry(i);
		eval_kMLP = reader->EvaluateMVA("kMLP");
		tr_out->Fill();
	}
	fout->Write();
	fout->Close();
}
