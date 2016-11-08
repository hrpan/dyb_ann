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

	fac->AddVariable("ep",'F');	
	fac->AddVariable("ed",'F');	
	fac->AddVariable("dt",'F');	
	fac->AddVariable("dist",'F');

	TCut epCut = "ep>0.7&&ep<12";
	TCut edCut = "ed>1.5&&ed<12";
	TCut dtCut = "dt>1&&dt<400";
	TCut distCut = "dist<2500";
	TCut preCut = epCut&&edCut&&dtCut&&distCut;

	int nTrainS = 1000000;
	int nTrainB = 1000000;
	int nTestS  = 1000000;
	int nTestB  = 1000000;

	TString trainOpts = TString::Format("nTrain_Signal=%d:nTrain_Background=%d",nTrainS,nTrainB);
	TString testOpts = TString::Format("nTest_Signal=%d:nTest_Background=%d",nTestS,nTestB);
	TString prepOpts = trainOpts+":"+testOpts;

	fac->PrepareTrainingAndTestTree(preCut,prepOpts);

	
	//fac->BookMethod(TMVA::Types::kBDT,"TMVA_BDT","VarTransform=Norm");
	//fac->BookMethod(TMVA::Types::kSVM,"TMVA_SVM","VarTransform=Norm");
	
	fac->BookMethod(TMVA::Types::kMLP,"TMVA_MLP_9_9","VarTransform=Norm:HiddenLayers=9,9:UseRegulator=True");
	fac->BookMethod(TMVA::Types::kMLP,"TMVA_MLP_9_9_9","VarTransform=Norm:HiddenLayers=9,9,9:UseRegulator=True");
	//fac->BookMethod(TMVA::Types::kMLP,"TMVA_MLP_5_5_5_reg","VarTransform=Norm:HiddenLayers=5,5,5:UseRegulator=True");
	//fac->BookMethod(TMVA::Types::kMLP,"TMVA_MLP_5_5_5_5","VarTransform=Norm:HiddenLayers=5,5,5,5");
	//fac->BookMethod(TMVA::Types::kTMlpANN,"ROOT_MLP","!V:HiddenLayers=N+1,N+1");

	fac->TrainAllMethods();
	fac->TestAllMethods();
	fac->EvaluateAllMethods();

	fout->Close();
}
