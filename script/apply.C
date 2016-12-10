#include "readCFG.h"
float ep,ed,dt,dist;

void apply(TString weightFile,TString inputFile,TString inputTree){

	cout << "INPUT FILE:" << inputFile << endl;
	cout << "INPUT TREE:" << inputTree << endl;

	TMVA::Reader *reader = new TMVA::Reader();
	reader->AddVariable("ep",&ep);
	reader->AddVariable("ed",&ed);
	reader->AddVariable("dt",&dt);
	reader->AddVariable("dist",&dist);

	reader->BookMVA("kMLP",weightFile.Data());

	TChain *chainData = new TChain(inputTree);
	
	chainData->Add(inputFile);

	TTree *tr_Data = (TTree*) chainData;
	setAddress(tr_Data);

	TString evalData(inputFile);
	evalData.ReplaceAll(".root","_kMLP.root");

	cout << "EVAL FILE:" << evalData << endl;
	
	fillTree(reader,tr_Data,evalData);

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
		if(i%128==0) printProgress(i,nevts);
		tr_in->GetEntry(i);
		eval_kMLP = reader->EvaluateMVA("kMLP");
		tr_out->Fill();
	}
	cout << "Evaluation complete" << endl;
	fout->Write();
	fout->Close();
}

void printProgress(int i,const int nevts){
	printf("\rProgress: %d/%d",i+1,nevts);
	fflush(stdout);
}
