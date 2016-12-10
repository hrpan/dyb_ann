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
TCut bkgEval = "eval_kMLP<0.2";
TCut sigEval = "eval_kMLP>0.9";

void plot(TString weightFile){

	readCFG cfg;

	weightFile.ReplaceAll("./","");
	weightFile.ReplaceAll("weights/MVA_T","T");
	weightFile.ReplaceAll(".weights.xml","");

	TChain *chainData = new TChain("IBD");
	TChain *chainSig = new TChain("tr");
	TChain *chainBkg = new TChain("acc");

	chainData->Add(cfg.dataFile.c_str());
	chainSig->Add(cfg.sigFile.c_str());
	chainBkg->Add(cfg.bkgFile.c_str());

	TString s_evalData = TString::Format("./evals/data/evalData_%s.root",weightFile.Data());
	TString s_evalSig = TString::Format("./evals/sig/evalSig_%s.root",weightFile.Data());
	TString s_evalBkg = TString::Format("./evals/bkg/evalBkg_%s.root",weightFile.Data());

	TFile *evalData = new TFile(s_evalData,"READ");
	TFile *evalSig = new TFile(s_evalSig,"READ");
	TFile *evalBkg = new TFile(s_evalBkg,"READ");

	TTree *tr_Data = (TTree*) evalData->Get("MLP");
	TTree *tr_Sig = (TTree*) evalSig->Get("MLP");
	TTree *tr_Bkg = (TTree*) evalBkg->Get("MLP");


	tr_Data->AddFriend(chainData);
	tr_Sig->AddFriend(chainSig);
	tr_Bkg->AddFriend(chainBkg);

	TCut sigCut = epCut&&edCut&&dtCut&&distCut;
	TCut bkgCut = epCut&&edCut&&dtCut&&distCut;

	int bins = 1000;
	
	drawNS("ep",bins,0,12,tr_Data,tr_Bkg,bkgEval,bkgEval&&bkgCut,"bkg");
	drawNS("ed",bins,1,12,tr_Data,tr_Bkg,bkgEval,bkgEval&&bkgCut,"bkg");
	drawNS("dt",bins,0,400,tr_Data,tr_Bkg,bkgEval,bkgEval&&bkgCut,"bkg");
	drawNS("dist",bins,0,2500,tr_Data,tr_Bkg,bkgEval,bkgEval&&bkgCut,"bkg");
	drawNS("ep",bins,0,12,tr_Data,tr_Sig,sigEval,sigEval&&bkgCut,"sig");
	drawNS("ed",bins,1,12,tr_Data,tr_Sig,sigEval,sigEval&&bkgCut,"sig");
	drawNS("dt",bins,0,400,tr_Data,tr_Sig,sigEval,sigEval&&bkgCut,"sig");
	drawNS("dist",bins,0,2500,tr_Data,tr_Sig,sigEval,sigEval&&bkgCut,"sig");

}

void drawNS(TString var,int bins,int xmin,int xmax,TTree *tr1,TTree *tr2,TCut cut1,TCut cut2,TString suffix){
	TString str_h1 = TString::Format("%s>>h1(%d,%d,%d)",var.Data(),bins,xmin,xmax);
	TString str_h2 = TString::Format("%s>>h2(%d,%d,%d)",var.Data(),bins,xmin,xmax);

	tr1->Draw(str_h1,cut1);
	tr2->Draw(str_h2,cut2);
	h2->SetLineColor(kRed);
	h1->DrawNormalized();
	h2->DrawNormalized("same");
	gPad->SetLogy();	
	TString ofile = TString::Format("plots/%s_%s.png",var.Data(),suffix.Data());
	c1->SaveAs(ofile);
}
