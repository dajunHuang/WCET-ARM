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

cur_dir=$PWD
path=$1
gcc=arm-linux-gcc
objdump=arm-linux-objdump
lp_dir=$cur_dir/lp_solve/lp_solve
xli_dir=$cur_dir/lp_solve/xli_CPLEX
est=$cur_dir/est-arm/est
configDir=$cur_dir/est-arm/processor.opt


function mkdir_if_not_exist(){
    my_dir=$1
    if [ ! -d "$my_dir" ]; then
            #echo "创建文件夹 $1"
            mkdir -p $my_dir
    fi
}

function estimate(){

    est_obj=$1

    #$estDir -run CFG $est_obj
    echo "$est -config $configDir $est_obj";
    $est -config $configDir $est_obj > est_info.txt
}


function lp_solve(){
    echo "$lp_dir -rxli ./xli_CPLEX $1"
    $lp_dir -rxli ${xli_dir} $1 > results.txt
}


function compile_and_estimate(){
    src=$1 # $1 is the source file need to be compiled
    obj=${src%%.*} 

    echo "$gcc  -g -O2  $src -o  $obj "
    $gcc  -g -O2  $src -o  $obj 

    #echo "mkdir_if_not_exist ../results/results_sim/$obj"
    mkdir_if_not_exist ../results/results_est/$obj
    mv $obj ../results/results_est/$obj/
    #echo "mv $obj ../results/results_sim/$obj/"
    cd ../results/results_est/$obj/
    #echo $PWD
    $dis ${obj} 
    cp $cur_dir/benchmark/constriants/${obj}.cons ./
    estimate ${obj} 
    lp_solve ${obj}.lp
    cd -
    #echo $PWD                        
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
            compile_and_estimate $file
            #simulation ${file%%.*} 
        fi
    done
    cd ..
}

begin