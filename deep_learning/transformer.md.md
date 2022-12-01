# Transfomrer
[Attention Is All You Need](https://arxiv.org/pdf/1706.03762.pdf)  
[Improving Language Understanding by Generative Pre-Training](https://s3-us-west-2.amazonaws.com/openai-assets/research-covers/language-unsupervised/language_understanding_paper.pdf) GTP
[language-models.pdf](https://d4mucfpksywv.cloudfront.net/better-language-models/language-models.pdf) GPT2
[Language Models are Few-Shot Learners](https://arxiv.org/pdf/2005.14165.pdf) GPT3

## Attention Is All You Need
首次采用了Attention机制，抛弃了RNN和CNN等网络结构
定义了Transformer 网络结构
![[Pasted image 20221130150947.png]]

6 identical Encode layer， 6 identical Decorder layer.

MultiHead Attention Layer
![[Pasted image 20221130151458.png]]
在bert模型中, concat 被省略掉。
![[Pasted image 20221130151828.png]]
根据计算公式Attention Layer
![[Pasted image 20221130152000.png]] 
在bert中对应![[Pasted image 20221130152034.png]]



## Improving Language Understanding by Generative Pre-Training(GPT)
![[Pasted image 20221130153203.png]]