#!/bin/sh

#dir-arc
#chrnos-demo -simplesim-arm
#           |-est
#           |-benchmark
#           |-results  -report.txt
#                      |           
#                      |-results_est - test1
#                                    |-test2
#                                    |-...
#                      | results_sim - test1
#                                    |-...
#           |-lp_solve
#           |-simulation.sh
#           |-estimate.sh

path=$1

cc=arm-linux-gcc
# cc=~/chronos-demo/est/bin/sslittle-na-sstrix-gcc
sim=../simplesim-arm/sim-outorder
# sim=../simplesim-3.0/sim-outorder

function remkdir()
{
  if [ -e ${1} ] ; then
    rm -rf ${1}
  fi
  
  mkdir ${1}
}

function mkdir_if_not_exist(){
    my_dir=$1
    if [ ! -d "$my_dir" ]; then
            #echo "创建文件夹 $1"
            mkdir -p $my_dir
    fi
}

#build test benchmarks to ARM ELF binary excutable file.
function build_and_simulation(){
    src=$1 # $1 is the source file need to be compiled
    flag=-g 
    obj=${src%%.*}  
    #compile
    $cc $flag $src -static -o $obj
    
    #move to objective directory
    #echo "mkdir_if_not_exist ../results/results_sim/$obj"
    mkdir_if_not_exist ../results/results_sim/$obj
    #echo "mv $obj ../results/results_sim/$obj/"
    simulation ${obj} 
    mv $obj ../results/results_sim/$obj/
}

function simulation(){
    src=$1
    #obj=${src%%.*}
    processor_config=./simplesim-arm/tests/highend.cfg
    ins_kye='sim_total_insn|sim_total_loads|sim_total_stores|sim_total_branches|sim_cycle'
    bpred_key='*\.lookups|*\.updates|*\.addr_hits|*\.misses'
    cache_key='*\.accesses|*\.hits' 

    #echo "simulation $PWD"
    #echo "$sim $src"
    #$sim -config $processor_config $obj >& tempfile
    echo "$sim $src ..."
    $sim $src >& tempfile
    cat tempfile | grep -E "$ins_kye|$bpred_key|$cache_key" > sim.txt
    mv sim.txt ../results/results_sim/${obj}/sim.txt
    cat tempfile | grep  'sim_cycle' | awk '{print $2}' >> \
        ../results/results_sim/${obj}/sim.txt
    rm tempfile
    #echo "done"
}

function begin(){
    #compile and move to objective directory
    if [ $# -eq 0 ]; then
        path="./benchmark"
    fi

    cd $path
    for file in `ls`
    do
        if [ "${file#*.}" = "c" ];then
            #echo "$file"
            build_and_simulation $file
            #simulation ${file%%.*} 
        fi
    done
    cd ..
}

begin