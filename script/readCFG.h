#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
using namespace std;

class readCFG{
	public:
	ifstream f_cfg;

	bool evalData;
	bool evalBkg;
	bool evalSig;

	bool useTrainTree;
	int bins;
	float xmin;
	float xmax;

	string trainFile;
	string dataFile;
	string bkgFile;
	string sigFile;


	readCFG();
};

readCFG::readCFG(){

	f_cfg.open("cfg");

	string line;

	while(getline(f_cfg,line)){
		if(line[0]=='#')
			continue;
		if(line.find("EVALDATA")!=string::npos)
			if(line.find("TRUE")!=string::npos)
				evalData=true;
			else
				evalData=false;
		else if(line.find("EVALBKG")!=string::npos)
			if(line.find("TRUE")!=string::npos)
				evalBkg=true;
			else
				evalBkg=false;
		else if(line.find("EVALSIG")!=string::npos)
			if(line.find("TRUE")!=string::npos)
				evalSig=true;
			else
				evalSig=false;
		else if(line.find("USETRAINTREE")!=string::npos)
			if(line.find("TRUE")!=string::npos)
				useTrainTree=true;
			else
				useTrainTree=false;
		else if(line.find("BINS")!=string::npos)
			bins = atoi(line.substr(line.find("=")+1).c_str());
		else if(line.find("XMIN")!=string::npos)
			xmin = atof(line.substr(line.find("=")+1).c_str());
		else if(line.find("XMAX")!=string::npos)
			xmax = atof(line.substr(line.find("=")+1).c_str());
		else if(line.find("TRAINFILE")!=string::npos)
			trainFile = line.substr(line.find("=")+1);
		else if(line.find("SIGFILE")!=string::npos)
			sigFile = line.substr(line.find("=")+1);
		else if(line.find("BKGFILE")!=string::npos)
			bkgFile = line.substr(line.find("=")+1);
		else if(line.find("DATAFILE")!=string::npos)
			dataFile = line.substr(line.find("=")+1);
	}
		
}

