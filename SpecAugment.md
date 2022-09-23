# SpecAugment

[SpecAugment: A Simple Data Augmentation Method for Automatic Speech Recognition](https://arxiv.org/pdf/1904.08779.pdf)

### Abstract

A method of data augmentation for speech recognition for low resource training.

directly apply to the feature inputs of a neural network.

consisits of warping the features masking blocks of frequency channels, and masking blocks of time steps.

achive 6.8% WER on test-other without LM, 5.8% with shallow fusion LM.

![image-20220919171234634](D:\Repo\learning_repo\SpecAugment.assets\image-20220919171234634.png)

form top to bottom, figures depict the log mel sepctrogram of the base input with no augmentation, time warp, frequency masking and time masking applied.