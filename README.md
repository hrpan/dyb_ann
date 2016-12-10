# dyb_ann
Artificial neural network for accidental background subtraction


##Evaluating a file

To evaluate a file, use ```apply.C``` with the following format

```root 'apply.C("weightfile","inputfile","TTree")'```

Make sure that your ```TTree``` contains four ```TBranch``` with the following names

- ep (Prompt Energy)
- ed (Delay Energy)
- dt (Capture Time)
- dist (Prompt-Delay Distance)

(Note that the naming has to be exactly the same, otherwise it won't work.)

The output file contains a ```TTree``` named ```MLP``` with ```TBranch``` ```eval_kMLP``` containing the responses from the given weight file.

###An example

First we generate the response file using ```apply.C```

```root 'apply.C("weights/test.weights.xml","ibd.root","IBD")'```

then we load ```ibd.root``` with ```ROOT```

```>root ibd.root```

Finally we add ```eval_kMLP``` as friend for ```IBD```

```root [0] IBD->AddFriend("MLP","ibd_kMLP.root")```

Now you can use ```eval_kMLP``` as if it is in the same ```TTree``` as ```IBD```
