
Adapter trimmer
===============

A simple trimmer that removes sequences from the 3-prime end of Illumina reads. A simple Make 
workflow has been added to use this tool on multiple FastQ files at once in the workflow 
directory.    

After building the software with the provided Makefile, 2 programs should be created in the 
bin directory. These 2 binaries are adapter_trim and iupac_trim. The adapter_trim application 
removes standard sequences from the reads. The iupac_trim program understands the complete IUPAC 
nucleotide codes. 

The adapter trimmer uses a simple match/mismatch scoring algorithm. Reads are trimmed if more than 
`$match` bases match the provided sequence and no more than `$mismatch` mismatches occurred. Multiple 
sequences can be processed at once. No specific paired-end functionality is available in this tool, but 
the first and second reads can be processed independantly.   

To do
-----

* Implement tests software test. Currently, only a test script with simple test data is present.  

Installation instruction
------------------------

Prerequisites:
* BOOST libraries
* BOOST development libraries

```{r}
cd /path/to/AdapterTrimmer/
make ;
```
