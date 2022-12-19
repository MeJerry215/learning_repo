# GPU Arch

## Arch

| Arch   | Version | Time |
| -----  | ------- | ---- |
| Tesla  | 1.0     | 2008 |
| Fermi  | 2.0     | 2010 |
| Kepler | 3.0     | 2012 |
| Maxcell| 5.0     | 2014 |
| Pascall| 6.0     | 2016 |
| Volta  | 7.0     | 2017 |
| Turning| 7.5     | 2018 |
| Ampere | 8.0     | 2020 |
| Hopper | 9.0     | 2022 |

|    GPU             |  P100        | V100           | A100           | H100           |
|------------------- | ------------- | ------------- |  ------------- |  ------------- |
| Compute Cap        | 6.0           | 7.0           | 8.0            | 9.0            |
| FP64               | 5.3TFLOPS | 7.8TFLOPS | 9.7TFLOPS/19.5TFLOPS(TC) | 30TFLOPS/60TFLOS(TC) |
| FP32               | 10.6TFLOPS    | 15.7TFLOPS    | 19.5TFLOPS     | 60TFLOS        |
| TF32               | NA            | NA            | 156TFLOPS(TC)  | 500TFLOPS      |
| FP16               | 21.2TFLOPS    |               | 78TFLOPS/312TFLOPS(TC) | 120TFLOS/1000TFLOPS |
| BF16               | NA            | NA            | 39TFLOPS/312TFLOPS(TC) | 120TFLOS/1000TFLOPS |
| FP8                | NA            | NA            | NA             | 2000TFLOPS     |
| INT8               | NA            | NA            | 624TFLOPS(TC)  | 2000TFLOPS     |
| INT4               | NA            | NA            | 1248TFLOPS(TC) |
| TensorCore Mixed   | NA            | 125TFOPS      | 
| NvLink             | 160GB/s       | 300GB/s       | 600GB/s        | 900GB/s        |
| Texture Units      | 224           | 320           | 432            | 528            |
| Mem Inferface      | 4096-bit HMB2 | 4096-bit HMB2 | 5120-bit HMB2  | 5120-bit HMB3  |
| SM                 | 56            | 80            | 108            | 132            |  
| TPC                | 28            | 40            | 54             | 66             |
| FP32 Cores/SM      | 64            | 64            | 64             | 128            |
| FP32 Cores/GPU     | 3584          | 5120          | 6912           | 16896          |
| FP64 Cores/SM      | 32            | 32            | 32             | 64             |
| FP64 Cores/GPU     | 1792          | 2560          | 3456           | 8448           |
| TensorCores/SM     | NA            | 8             | 4              | 4              |
| TensorCores/GPU    | NA            | 640           | 432            | 528            |
| Clock Rate         | 1328MHz       |               | 
| Boost Clock Rate   | 1480 MHz      | 1530MHz       | 1410MHz        |
| Memory Clock Rate  | 703MHz DDR    | 877.5MHz DDR  | 1215MHz DDR    |
| Memory Bandwidth   | 720GB/sec     | 900GB/sec     | 1555GB/sec     | 3000GB/sec    |
| Unified Memory     | √             | √             |
| Mem Size           | 16GB          | 16 GB         | 40GB           | 80GB          |
| L1 Cache Size      | 64KB/SM       | Total 128KB with share| 192KB combined | 256KB combined |
| L2 Cache Size      | 4096KB        | 6144KB        | 40MB           | 50MB          |
| Share Mem Size     | 64KB/SM       | up to 96KB/SM | up to 164KB    | up to 228KB   |
| Register File/SM   | 256KB         | 256KB         | 256KB          | 256KB         |
| Register File/GPU  | 14336KB       | 20480KB       | 27648KB        | 33792KB       |
| Threads/Warp       | 32            | 32            | 32             | 32            |
| Warps/SM           | 64            | 64            | 64             | 64            |
| Threads/SM         | 2048          | 2048          | 2048           | 2048          |
| Thread Blocks/SM   | 32            | 32            | 32             | 32            |
| Max Thread Block Size | 1024       | 1024          | 1024           | 1024          |
| TDP                | 300W          | 300W          | 400W           | 700W          |
| Transistors        | 15.3 billion  | 21.1 billion  | 54.2 billion   | 80 billion    |
| Die Size           | 610$mm^2$     | 815 $mm^2$    | 826 $mm^2$     | 814 $mm^2$    |
| Manufacturing      | 16nm FinFet+  | 12nm FFN      | 7-nm N7 FinFet | 4N            |

**实际产品参数低于设计white papaer参数，属于正常阉割**




## Tesla

[P100 Pascal White Paper](https://images.nvidia.com/content/pdf/tesla/whitepaper/pascal-architecture-whitepaper.pdf)

[V100 Volta White Paper](https://www.nvidia.com/content/dam/en-zz/Solutions/Data-Center/tesla-product-literature/volta-architecture-whitepaper.pdf)

### P100

15.3 billion transistor, 16nm [FinFET](#finfet)

6 GPCs(Graphics Processing Clusters, each including 10 SMs)
30 TPCS(Texture Processing Clusters, each including 2 SMs)
8 512-bit memory controller(4096 bits)
60 SMs(Stream Multiprocessors)
Each SM has 64 Cuda Cores, 4 texture units. Total 3840 cuda core and 240 texture units. Partitions 2 processing blocks(each has 32 FP32 cuda cores). Each block  has 1 instruction buffer, 1 warp scheduler, 2 dispatch units. each warp scheduler 2 warp instruction per clock.


features: 
- [NVLink](#nvlink)
- [hbm2](#hbm)
- [Unified Memory](#unmem)

GPU-to-GPU data transfer, 160GB/s bidirectional bandwidth—5x(5 x PCIE Gen3 x 16)
![[cuda/gpu_arch/Pasted image 20220814012306.png]]

#### Hardware Arch
![[cuda/gpu_arch/Pasted image 20220814013555.png]]
![[cuda/gpu_arch/Pasted image 20220814015744.png]]

### V100

21.1 billion transistors, 12nm [FinFET](#finfet)

6 GPCs(Graphics Processing Clusters, each including 14 SMs)
42 TPCS(Texture Processing Clusters, each including 2 SMs)
8 512-bit memory controller(4096 bits)
84 SMs(Stream Multiprocessors)
Total 672 Tensor Cores, 336 texture units. 6144KB L2 cache.
Partitions 4 processing blocks(each has 32 FP32 cuda cores, 8 FP64 cores, 16 INT32 cores, 2 mixed-precision tensor cores). Each block has 1 L0 instruction cache, 1 warp scheduler, 1 dispatch units.each warp scheduler 1 warp instruction per clock.

Each Tensor Core 64 FP FMA per clock. 4x4 matrices $D = A * B + C$, A and B are FP16, C and D may be FP16 or FP32
![[cuda/gpu_arch/Pasted image 20220814125743.png]]


features:
- [2rd Generation NVLink](#nvlink)
- [hbm2](#hbm)
- [MPS](#mps)
- [Maximum Performance and Maximum Efficiency Modes]()
- [Tensor Cores](#tensor_core)

#### Hardware Arch
![[cuda/gpu_arch/Pasted image 20220814123032.png]]
![[cuda/gpu_arch/Pasted image 20220814124759.png]]

## Fermi
[Fermi Whitepaper](https://www.nvidia.com/content/PDF/fermi_white_papers/NVIDIA_Fermi_Compute_Architecture_Whitepaper.pdf)


## Kepler


## MaxWell


## Turing


## Ampere
[Ampere whitepaer](https://images.nvidia.com/aem-dam/en-zz/Solutions/data-center/nvidia-ampere-architecture-whitepaper.pdf)

54.2 billion transistors, 7nm N7

features:
- [TF32](#tf32)
- [BF16](#bf16)
- [Sparsity](#sparsity)
- [MIG](#mig)
- [3rd Generation NVLink](#nvlink)
- Asynchronous Copy
- [AMP](#amp)
- [NVJPG](#nvjpg)

8 GPCs(Graphics Processing Clusters, each including 16 SMs)
64 TPCS(Texture Processing Clusters, each including 2 SMs)
12 512-bit memory controller(6144 bits)
128 SMs(Stream Multiprocessors)
64 FP32 CUDA Cores/SM, 8192 FP32 CUDA Cores per full GPU
4 Third-generation Tensor Cores/SM, 512 Third-generation Tensor Cores per full GPU 
<!-- Total 672 Tensor Cores, 336 texture units. 6144KB L2 cache. -->
#### Hardware Arch
![[cuda/gpu_arch/Pasted image 20220814153645.png]]
![[cuda/gpu_arch/Pasted image 20220814154148.png]]
![[cuda/gpu_arch/Pasted image 20220814154253.png]]

![[cuda/gpu_arch/Pasted image 20220814161256.png]]
## Hopper
[Hopper whitepaer](https://resources.nvidia.com/en-us-tensor-core)

80 billion transistors, 7nm N4

8 GPCs(Graphics Processing Clusters, each including 18 SMs)
72 TPCS(Texture Processing Clusters, each including 2 SMs)
12 512-bit memory controller(6144 bits)
144 SMs(Stream Multiprocessors)
128 FP32 CUDA Cores per SM, 18432 FP32 CUDA Cores per full GPU
4 Fourth-Generation Tensor Cores per SM, 576 per full GPU
60 MB L2 Cache

features:
- [TMA](#tma)
- [HMB](#hnm)
- Thread Block Clusters. Thread Block Runs on isngle SM, Thread Block Clusters runs on groups SMs.![[cuda/gpu_arch/Pasted image 20220814165856.png]]
- Distributed Shared Memory(DSMEM)

#### Hardware Arch
![[cuda/gpu_arch/Pasted image 20220814164413.png]]
![[cuda/gpu_arch/Pasted image 20220814165040.png]]

![[cuda/gpu_arch/Pasted image 20220814170616.png]]
## Refence
**<span id="nvlink">NVLink</span>**

**<span id="hbn">HBM2</span>**

**<span id="unmem">Unified Memory</span>**


**<span id="finfet">FinFET</span>**


**<span id="gpc">GPC</span>**

**<span id="sm">SM</span>**

**<span id="tpc">TPC</span>**

**<span id="tpc">MPS</span>**

**<span id="tpc">Tensor Cores</span>**

**<span id="tf32">TF32</span>**

**<span id="tf32">Sparsity</span>**

**<span id="mig">MIG</span>**

**<span id="amp">Automatic Mixed Precision</span>**

**<span id="nvjpg">NVJPG</span>**

**<span id="tma">Tensor Memory Accelerator</span>**