#!/bin/bash

doEVAL=false
doHIST=false
doPLOT=false
doFORCE=false

while getopts ":aefhp" opt; do
	case $opt in
		a)
			doEVAL=true
			doHIST=true
			doPLOT=true
			echo "Doing all"
			;;
		e)
			doEVAL=true
			;;
		f)
			doFORCE=true
			echo "Forcing RECREATE" 
			;;
		h)
			doHIST=true
			;;
		p)
			doPLOT=true
			;;
	esac
done

shift $((OPTIND-1))

weightFile=$1
weightPrefix="weights/MVA_"
weightSuffix=".weights.xml"

dirPrefix="./"
suffix="${weightFile/$dirPrefix/}"
suffix="${suffix/$weightPrefix/}"
suffix="${suffix/$weightSuffix/}"
printf "BEGIN TO ANALYZE: $suffix \n"

if [ "$doEVAL" = true ];
then
	evalFile="./evals/data/evalData_${suffix}.root"
	if [ -f $evalFile ];
	then
		if [ "$doFORCE" = true ];
		then
			echo "Evaluating $evalFile"
			root -l -b -q 'script/eval.C("'$weightFile'")'
		else
			echo "$evalFile already exists"
		fi
	else
		echo "Evaluating $evalFile" 
		root -l -b -q 'script/eval.C("'$weightFile'")'
	fi
fi

if [ "$doHIST" = true ];
then 
	histFile="./hists/hist_${suffix}.root"
	if [ -f $histFile ];
	then 
		if [ "$doFORCE" = true ];
		then 
			echo "Histogramming $histFile"
			root -l -b -q 'script/hist.C("'$weightFile'")'
		else		
			echo "$histFile already exists"
		fi
	else
		echo "Histogramming $histFile"
		root -l -b -q 'script/hist.C("'$weightFile'")'
	fi
fi

if [ "$doPLOT" = true ];
then
	plotFile="./plots/${suffix}.png"
	if [ -f $plotFile ];
	then
		if [ "$doFORCE" = true ];
		then
			echo "Fitting $plotFile"
			root -l -b -q 'script/fit.C("'$weightFile'")'
		else
			echo "$plotFile already exists"
		fi
	else
		echo "Fitting $plotFile"
		root -l -b -q 'script/fit.C("'$weightFile'")'
	fi
fi
