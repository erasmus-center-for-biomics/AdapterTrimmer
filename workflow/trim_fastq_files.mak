# Global parameters
# =================
#
path_viral := $(shell echo $$PATH_VIRAL)
path_zcat := $(shell echo $$PATH_ZCAT)

# Minor fixed parameters
# ----------------------
minimum_matches = 2
maximum_mismatches = 2
minimum_readlength = 20
adapter_trim_options = --maximum-mismatches $(maximum_mismatches) --minimum-matches $(minimum_matches) --minimum-bases-remaining $(minimum_readlength)

# Target files to create
# ======================
#
inputfiles := $(wildcard *.fastq) $(wildcard *.fastq.gz)
filebase   := $(sort $(foreach entry, $(inputfiles), $(shell echo $(entry) | sed 's/.gz$$//' | sed 's/.fastq$$//')))
trimmed    := $(patsubst %, trimmed/%.fastq, $(filebase))

# Make commands
# =============
#
all: trim

trim: $(trimmed)


# Make recipes
# ============
#

# Trim the sequences
# ------------------
trimmed/%.fastq: %.fastq
	mkdir -p logs
	mkdir -p trimmed
	$(path_viral)/submodules/AdapterTrimmer/bin/adapter_trim \
		-i $*.fastq \
		$(adapter_trim_options) \
		-a AGATCGGAAGAG \
		-a CTGTCTCTTATA \
		-o trimmed/$*.fastq 2>> logs/$*.trim.errors.log >> logs/$*.messages.errors.log

trimmed/%.fastq: %.fastq.gz
	mkdir -p logs
	mkdir -p trimmed
	$(path_zcat)/zcat $*.fastq.gz | $(path_viral)/submodules/AdapterTrimmer/bin/adapter_trim \
		-i - \
		$(adapter_trim_options) \
		-a AGATCGGAAGAG \
		-a CTGTCTCTTATA \
		-o trimmed/$*.fastq 2>> logs/$*.trim.errors.log >> logs/$*.messages.errors.log
