# CEPARCO-MP1
**Group members**
	- Manaois, Raidon
	- Son, Ysabelle

**Project specifications**
	<br>- Task: Create a kernel that performs element-wise maximum with index tracking in different versions (C, x86-64, SIMD XMM, and SIMD YMM) and perform comparative analysis. 
  <br>- Input: Scalar integer $n$ (vector length); single-precision float vectors $A$ and $B$. 
  <br>- Process: For each index $i$, $c[i] = \max(A[i], B[i])$ and $idx[i] = 0$ if $a[i] \geq b[i]$ else $1$. 
  <br>- Output: Single-precision vector $C$ for maximums and 32-bit vector $idx$ for tracking. <br><br>
  **Project specifications**
  <br>- Gemini: Used for generating the Markdown formatting and layout of the ReadMe based on project specifications.

## i) Program output screenshots
  # **C Baseline**
### **Vector Size $n = 2^{20}$**
<p align="center">
  <img src="Images/C tests/20 (ysa).jpg" alt="C Kernel Output 2^20" width="48%"/>
  &nbsp;
  <img src="Images/C tests/20 (rayds).jpg" alt="Assembly Kernels Output 2^20" width="48%"/>
</p>

### **Vector Size $n = 2^{26}$**
<p align="center">
  <img src="Images/C tests/26 (ysa).jpg" alt="C Kernel Output 2^20" width="48%"/>
  &nbsp;
  <img src="Images/C tests/26 (rayds).jpg" alt="Assembly Kernels Output 2^20" width="48%"/>
</p>

### **Vector Size $n = 2^{29}$**
<p align="center">
  <img src="Images/C tests/29 (ysa).jpg" alt="C Kernel Output 2^20" width="48%"/>
  &nbsp;
  <img src="Images/C tests/29 (rayds).jpg" alt="Assembly Kernels Output 2^20" width="48%"/>
</p>

# **Multi-Kernel Correctness Check**
### **Vector Size $n = 2^{20}$**
<p align="center">
  <img src="Images/C tests/20 86xy first.png" alt="C Kernel Output 2^20" width="48%"/>
  &nbsp;
  <img src="Images/C tests/20 86xy last.png" alt="Assembly Kernels Output 2^20" width="48%"/>
<p align="center">
<em>Verification of the first five indices showing consistent C and idx values across all kernel versions. Verification of the last five indices, ensuring the kernels handle the end of the vector correctly.</em>
</p>

### **Vector Size $n = 2^{26}$**
<p align="center">
  <img src="Images/C tests/26 86xy first.png" alt="C Kernel Output 2^20" width="48%"/>
  &nbsp;
  <img src="Images/C tests/26 86xy last.png" alt="Assembly Kernels Output 2^20" width="48%"/>
</p>

### **Vector Size $n = 2^{29}$**
<p align="center">
  <img src="Images/C tests/29 86xy first.png" alt="C Kernel Output 2^20" width="48%"/>
  &nbsp;
  <img src="Images/C tests/29 86xy last.png" alt="Assembly Kernels Output 2^20" width="48%"/>
</p>

## ii) Comparative execution-time table

| Kernel / Variant | $n=2^{20}$ | $n=2^{26}$ | $n=2^{29}$ | $n=2^{29}$Performance Note
|---|---:|---:|---:|---:|
C Version1 | 0.003767 s | 0.188167 s | 0.986300 s | Baseline implementation
x86-64 (Scalar) | 0.000867 s | 0.103533 s | 0.795233 s | Slightly faster than C
SIMD XMM | 0.000133 s | 0.045333 s | 0.366900 s | Processes 4 elements/instr
SIMD YMM | 0.000200 s | 0.049300 s | 0.357733 s | Processes 8 elements/instr

### **Analysis of Results**
The SIMD implementations (XMM and YMM) show a significant performance lead over the scalar C and x86-64 versions. 
- **Scalar vs. SIMD**: SIMD versions reduce the number of loop iterations needed by processing 4 (XMM) or 8 (YMM) single-precision elements simultaneously. 
- **XMM vs. YMM**: While YMM should theoretically be twice as fast as XMM, the execution times for $n=2^{29}$ are very close (0.3669s vs 0.3577s). This suggests that as vector size increases, the bottleneck shifts from computational throughput to memory bandwidth.

---

## iii) Boundary Handling Check
To handle vector sizes that are not perfect multiples of 4 or 8, the program implements a **scalar remainder loop**. 

1. The main loop processes elements using SIMD instructions in chunks.
2. Once fewer than 4 (XMM) or 8 (YMM) elements remain, the kernel exits the SIMD loop.
3. A scalar loop processes the remaining "boundary" elements one by one to ensure all indices are computed correctly.

---

## iv) Discussion

### **Problems Encountered and Solutions**
- **Memory Limitations**: During testing, we found that $n=2^{30}$ was not possible on the test machine due to memory constraints.
- **Solution**: We reduced the largest vector size to $n=2^{29}$ as permitted by the project specifications to maintain system stability.
- **Correctness Discrepancies**: Initially, index tracking values were inconsistent between C and SIMD versions.
- **Solution**: Verified the comparison logic ($a[i] \geq b[i]$) to ensure the SIMD masks correctly assigned 0 or 1 to match the C baseline.

### **Unique Methodology**
- **Deterministic Initialization**: We used a specific formula ($sin$ for vector A and $cos$ for vector B) for data initialization. This provided a deterministic and documented way to verify correctness across all kernel versions.

### **AHA Moments**
- Realizing that while YMM registers are wider, the actual speedup over XMM is limited by how fast the CPU can fetch data from memory.
- Seeing the effectiveness of the scalar remainder loop in handling arbitrary vector sizes, which prevented memory access errors on non-aligned boundaries.
