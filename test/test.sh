#!/bin/env sh


../bin/adapter_trim \
	-i test_simple.fastq \
	--maximum-mismatches 2 --minimum-matches 2 --minimum-bases-remaining 10 \
	-a GAATTC -a GATC \
	-o simple_base.fastq

../bin/iupac_trim \
	-i test_simple.fastq \
	--maximum-mismatches 2 --minimum-matches 2 --minimum-bases-remaining 20 --first-base 10 \
	-a RAATTY -a GATC \
	-o simple_iupac.fastq

cat simple_iupac.fastq
