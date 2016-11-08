float ep,ed,dt,dist;

void eval(TString weightFile){


	TMVA::Reader *reader = new TMVA::Reader();
	reader->AddVariable("ep",&ep);
	reader->AddVariable("ed",&ed);
	reader->AddVariable("dt",&dt);
	reader->AddVariable("dist",&dist);

	reader->BookMVA("kMLP",weightFile.Data());

	weightFile.ReplaceAll("./","");
	weightFile.ReplaceAll("weights/MVA_","");
	weightFile.ReplaceAll(".weights.xml","");


	//TFile *fin = new TFile("../../data/ibd_ct_EH1.root","READ");	
	//TFile *fin = new TFile("../../data/ibd_ct_EH1.root","READ");	
	TChain *chainData = new TChain("IBD");
	//chainData->Add("../../data_ibd_ct/2*.root");
	chainData->Add("../../data_ibd_ct/*.root");
	TFile *fSig = new TFile("../../data/mcFormat.root","READ");
	TChain *chainBkg = new TChain("acc");
	chainBkg->Add("../acc/data_pair_sig/*");	

	//TTree *tr_Data = (TTree*) fin->Get("IBD");
	TTree *tr_Data = (TTree*) chainData;
	setAddress(tr_Data);

	TTree *tr_Sig = (TTree*) fSig->Get("tr");
	setAddress(tr_Sig);

	TTree *tr_Bkg = (TTree*) chainBkg;
	setAddress(tr_Bkg);

	TString evalData = TString::Format("./evals/data/evalData_%s.root",weightFile.Data());

	fillTree(reader,tr_Data,evalData);


	TString evalSig = TString::Format("./evals/sig/evalSig_%s.root",weightFile.Data());

	fillTree(reader,tr_Sig,evalSig);

	TString evalBkg = TString::Format("./evals/bkg/evalBkg_%s.root",weightFile.Data());

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
