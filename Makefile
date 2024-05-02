CC=arm-none-eabi-gcc
CFLAGS= --static -nostdlib -g
DIS=arm-none-eabi-objdump
CONFIG=est-arm/processor.opt

# benchmark to use
TARGET=insertsort

# default: only estimate benchmark
all: est

# Field needed of the output of simplesim
ins_kye=sim_total_insn|sim_total_loads|sim_total_stores|sim_total_branches|sim_cycle
bpred_key=*\.lookups|*\.updates|*\.addr_hits|*\.misses
cache_key=*\.accesses|*\.hits

# build est
est-arm/est:
	make -C est-arm

# estimate the benchmark
est: est-arm/est benchmark/$(TARGET)
	mkdir -p results/$(TARGET)
	cp benchmark/$(TARGET) benchmark/constriants/$(TARGET).cons results/$(TARGET)
	$(DIS) -d results/$(TARGET)/$(TARGET) > results/$(TARGET)/$(TARGET).dis
	# est-arm/est -run CFG -config $(CONFIG) results/$(TARGET)/$(TARGET)
	est-arm/est -config $(CONFIG) results/$(TARGET)/$(TARGET) > results/$(TARGET)/est_info.txt
	dot -T png results/$(TARGET)/$(TARGET).dot -o results/$(TARGET)/$(TARGET).png
	# lp_solve/lp_solve -rxli lp_solve/xli_CPLEX results/$(TARGET)/$(TARGET).lp > results/$(TARGET)/result.txt

# compile benchmark
benchmark/%: benchmark/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	make -C est-arm clean
	-rm results/$(TARGET)/* results/results_sim/$(TARGET)/* benchmark/$(TARGET)

