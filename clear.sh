#/bin/bash

doEVAL=false
doHIST=false
doPLOT=false
doFORCE=false

while getopts ":a" opt; do
	case $opt in
		a)
			doEVAL=true
			doHIST=true
			doPLOT=true
			;;
		e)
			doEVAL=true
			;;
		f)
			doFORCE=true
			;;
		h)
			doHIST=true
			;;
		p)
			doPLOT=true
			;;
	esac
done


weightFile=$1
weightPrefix="weights/MVA_"
weightSuffix=".weights.xml"

dirPrefix="./"
suffix="${weightFile/$dirPrefix/}"
suffix="${suffix/$weightPrefix/}"
suffix="${suffix/$weightSuffix/}"
printf "BEGIN TO REMOVE: $suffix \n"


evalFile="./evals/data/evalData_${suffix}.root"
if [ -f $evalFile -a "$doEVAL" = true ];
then
	rm $evalFile
else
	echo "$evalFile does not exist" 
fi

histFile="./hists/hist_${suffix}.root"
if [ -f $histFile -a "$doHIST" = true ];
then 
	rm $histFile
else
	echo "$histFile does not exist"
fi

plotFile="./plots/${suffix}.png"
resFile="./plots/${suffix}_res.png"
if [ -f $plotFile -a "$doPLOT" = true ];
then
	rm $plotFile
else
	echo "$plotFile does not exist"
fi
