void MVA(){
	TFile *fout = new TFile("MVA.root","RECREATE");
	
	TMVA::Factory *fac = new TMVA::Factory("MVA",fout);
	
	TFile *fmc = new TFile("../../data/mcFormat.root","READ");
	TFile *facc = new TFile("../acc/data_pair/21221.root","READ");

	TTree *sigTree = (TTree*) fmc->Get("tr");
	TTree *bkgTree = (TTree*) facc->Get("acc");

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
	TCut distCut = "dist>0&&dist<5000";
	TCut preCut = epCut&&edCut&&dtCut&&distCut;

	int nTrainS = 1000000;
	int nTrainB = 1000000;
	int nTestS = 100000;
	int nTestB = 100000;

	TString trainOpts = TString::Format("nTrain_Signal=%d:nTrain_Background=%d",nTrainS,nTrainB);
	TString testOpts = TString::Format("nTest_Signal=%d:nTest_Background=%d",nTestS,nTestB);
	TString prepOpts = trainOpts+":"+testOpts;

	fac->PrepareTrainingAndTestTree(preCut,prepOpts);

	
	fac->BookMethod(TMVA::Types::kBDT,"BDT");
	fac->BookMethod(TMVA::Types::kSVM,"SVM");
	fac->BookMethod(TMVA::Types::kMLP,"TMVA_MLP","HiddenLayers=N+1,N+1:VarTransform=Norm");
	//fac->BookMethod(TMVA::Types::kTMlpANN,"ROOT_MLP","!V:HiddenLayers=N+1,N+1");

	fac->TrainAllMethods();
	fac->TestAllMethods();
	fac->EvaluateAllMethods();

	fout->Close();
}
