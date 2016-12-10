void MVA(){
	TFile *fout = new TFile("MVA.root","RECREATE");
	
	TMVA::Factory *fac = new TMVA::Factory("MVA",fout);
	
	TFile *fmc = new TFile("../../data/mcFormat.root","READ");

	TChain *acc_chain = new TChain("acc","acc_chain");
	acc_chain->Add("../acc/data_pair_sig/*.root");

	TTree *sigTree = (TTree*) fmc->Get("tr");
	//TTree *bkgTree = (TTree*) facc->Get("acc");
	TTree *bkgTree = (TTree*)acc_chain;

	double sigW = 1.0;
	double bkgW = 1.0;

	fac->AddSignalTree(sigTree,sigW);
	fac->AddBackgroundTree(bkgTree,bkgW);

	//fac->AddVariable("ep",'F');	
	fac->AddVariable("ed",'F');	
	fac->AddVariable("dt",'F');	
	fac->AddVariable("dist",'F');

	TCut epCut = "ep>0.7&&ep<12";
	TCut edCut = "ed>1.5&&ed<12";
	TCut dtCut = "dt>1&&dt<400";
	TCut distCut = "dist<2500";
	TCut preCut = epCut&&edCut&&dtCut&&distCut;

	int nTrainS = 4000000;
	int nTrainB = 4000000;
	int nTestS  = 2000000;
	int nTestB  = 2000000;

	TString trainOpts = TString::Format("nTrain_Signal=%d:nTrain_Background=%d",nTrainS,nTrainB);
	TString testOpts = TString::Format("nTest_Signal=%d:nTest_Background=%d",nTestS,nTestB);
	TString prepOpts = trainOpts+":"+testOpts;

	fac->PrepareTrainingAndTestTree(preCut,prepOpts);

	
	fac->BookMethod(TMVA::Types::kMLP,"TMVA_MLP_99_noep","VarTransform=Norm:HiddenLayers=9,9:UseRegulator=True:NCycles=500");

	fac->TrainAllMethods();
	fac->TestAllMethods();
	fac->EvaluateAllMethods();

	fout->Close();
}
