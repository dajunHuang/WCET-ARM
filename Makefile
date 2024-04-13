CC=arm-none-eabi-gcc
CFLAGS= --static -nostdlib -g
DIS=arm-none-eabi-objdump
CONFIG=est-arm/processor.opt

# benchmark to use
TARGET=insertsort

# default: only estimate benchmark
all: est

# build simplesim
simplesim-arm/sim-outorder:
	make -C simplesim-arm

# Field needed of the output of simplesim
ins_kye=sim_total_insn|sim_total_loads|sim_total_stores|sim_total_branches|sim_cycle
bpred_key=*\.lookups|*\.updates|*\.addr_hits|*\.misses
cache_key=*\.accesses|*\.hits

# simulate benchmark
sim: simplesim-arm/sim-outorder benchmark/$(TARGET)
	cp benchmark/$(TARGET) results/results_sim/$(TARGET)
	-simplesim-arm/sim-outorder -config $(CONFIG) results/results_sim/$(TARGET)/$(TARGET) >& results/results_sim/$(TARGET)/sim_result.txt
	cat results/results_sim/$(TARGET)/sim_result.txt | grep -E "$(ins_kye)|$(bpred_key)|$(cache_key)" > results/results_sim/$(TARGET)/short_result.txt

# build est
est-arm/est:
	make -C est-arm

# estimate the benchmark
est: est-arm/est benchmark/$(TARGET)
	cp benchmark/$(TARGET) results/results_est/$(TARGET)
	cp benchmark/constriants/$(TARGET).cons results/results_est/$(TARGET)
	-$(DIS) -d results/results_est/$(TARGET)/$(TARGET) > results/results_est/$(TARGET)/$(TARGET).dis
	est-arm/est -run CFG -config $(CONFIG) results/results_est/$(TARGET)/$(TARGET)
	# est-arm/est -config $(CONFIG) results/results_est/$(TARGET)/$(TARGET) > results/results_est/$(TARGET)/est_info.txt
	# lp_solve/lp_solve -rxli lp_solve/xli_CPLEX results/results_est/$(TARGET)/$(TARGET).lp > results/results_est/$(TARGET)/result.txt

# compile benchmark
benchmark/%: benchmark/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	make -C est-arm clean
	-rm -r results benchmark/$(TARGET)
	mkdir -p results/results_est results/results_sim 
	mkdir -p results/results_est/$(TARGET)
	mkdir -p results/results_sim/$(TARGET)

