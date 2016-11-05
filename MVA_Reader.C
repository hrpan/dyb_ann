void MVA_Reader(){
	TMVA::Reader *reader = new TMVA::Reader();
	float ep,ed,dt,dist;
	int det,site;
	reader->AddVariable("ep",&ep);
	reader->AddVariable("ed",&ed);
	reader->AddVariable("dt",&dt);
	reader->AddVariable("dist",&dist);

	reader->BookMVA("kMLP","weights/MVA_TMVA_MLP.weights.xml");

	TFile *fibd = new TFile("../../data/ibd_ct_EH1.root","READ");
	
	TTree *tr = (TTree*) fibd->Get("IBD");
	tr->SetBranchAddress("ep",&ep);
	tr->SetBranchAddress("ed",&ed);
	tr->SetBranchAddress("dt",&dt);
	tr->SetBranchAddress("dist",&dist);
	tr->SetBranchAddress("det",&det);
	tr->SetBranchAddress("site",&site);

	TFile *fout = new TFile("out.root","RECREATE");
	TTree *tr_out = new TTree("MLP","MLP");

	float eval_kMLP;
	tr_out->Branch("eval_kMLP",&eval_kMLP);	

	int nevts = tr->GetEntries();
	for(int i=0;i<nevts;++i){
		if(i%(nevts/100)==0) cout << "Progress: " << i*100/nevts << "%" << endl;
		tr->GetEntry(i);
		eval_kMLP = reader->EvaluateMVA("kMLP");
		tr_out->Fill();	
	}
	fout->Write();
	fout->Close();

}
