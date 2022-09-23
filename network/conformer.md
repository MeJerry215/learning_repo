# Conformer

[Conformer: Convolution-augmented Transformer for Speech Recognition](https://arxiv.org/pdf/2005.08100.pdf)





### Abstract

WER achieves 2.1%/4.3% without LM, 1.9%/3.9% with LM

convolution focus on local information, transformer focus on global information, combine convolution and transformer

introduce macaron-FFN, transform-xl



CTC:  Connectionist Temporal Classification
ASR: Audio Speech Recognition
WER: Word Error Rate
FFN: Feed Forward Network



### Network structure

![image-20220919164049969](D:\Repo\learning_repo\network\conformer.assets\image-20220919164049969.png)

3 model 10M(2.7%/6.3%), 30M, and 118M(1.9%/3.9% with LM)



convolution subsampling layer + conformer blocks * n
conformer blocks: feed-forward, self-attention, convlution, feed-forward.

#### MHSA(residual block):

![image-20220919164944544](D:\Repo\learning_repo\network\conformer.assets\image-20220919164944544.png)multi-headed self-atternion(MHSA) employs transformer-XL's relative sinusoidal positional encoding scheme
relative sinusoidal positional encoding scheme focous on different input lenght and resulting encodier is more robust on the variabance of the utterane length

#### Convolution Module(residual block):

![image-20220919164959470](D:\Repo\learning_repo\network\conformer.assets\image-20220919164959470.png)layer norm + pointwise conv + glu + 1d depthwise conv + batch norm + swish + point wise conv + dropout

#### Feed forward moduel(FFN)(residual block): 

![image-20220919165010283](D:\Repo\learning_repo\network\conformer.assets\image-20220919165010283.png)layernorm +  linear layer + swish actionvation + dropout  + linear layerout + dropout

Conformer Block:
learned from macaron-net, FFN + MHSA + Conv + FFN
![image-20220919165032526](D:\Repo\learning_repo\network\conformer.assets\image-20220919165032526.png)


Datasets: