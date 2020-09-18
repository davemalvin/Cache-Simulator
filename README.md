# Cache-Simulator
**Course Name:** Informatics 2C - Introduction to Computer Systems<br />
**Programming Language:** C<br />
**Objective:** To implement a direct-mapped cache to the memory hierarchy and the datapath and control signals of a MIPS processor.<br />
**Results:**  
Task 1 MIPS processor:<br />
     J_test.txt            : 5 out of 5 checks --> [7.00/7]<br /> 
     LW_test.txt           : 8 out of 8 checks --> [7.00/7]<br />
     SW_test.txt           : 7 out of 7 checks --> [7.00/7] \ 
     BQE_test.txt          : 5 out of 5 checks --> [7.00/7] \ 
     SLT_test.txt          : 7 out of 7 checks --> [7.00/7] \  
     ADDI_test.txt         : 7 out of 7 checks --> [7.00/7] \  
     pipe_complex_test.txt : 675 out of 675 checks --> [8.00/8] \  
 
Cache tests (with cache size 128B) \ 
     cache_LW_test.txt     : <not checking stores>  <LW hits:  incorrect>  <tag_bits: correct>  [1.00/7] \  
     cache_SW_test.txt     : segmentation or assertion --> [0.00/7] \  
     cache_complex_test.txt: <SW hits:  incorrect>  <LW hits:  incorrect>  <tag_bits: correct>  [1.00/7] \  
 
Cache tests (with cache size 256B) \ 
     cache_LW_test.txt     : <not checking stores>  <LW hits:    correct>  <tag_bits: correct>  [7.00/7] \  
     cache_SW_test.txt     : <SW hits:    correct>  <LW hits:    correct>  <tag_bits: correct>  [7.00/7] \  
     cache_complex_test.txt: <SW hits:  incorrect>  <LW hits:  incorrect>  <tag_bits: correct>  [1.00/7] \  
 
Dynamic memory management \ 
     Proper use of malloc  : [8.00/8] \  
 
Final cw2 mark: [75.0/100] 
