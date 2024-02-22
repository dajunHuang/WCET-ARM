#!/bin/sh

#dir-arc
#chronos-demo -simplesim-arm
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
#           |-begin.sh

cur_dir=$PWD
est_results_dir=$cur_dir/results/results_est
sim_results_dir=$cur_dir/results/results_sim

#if file exist, remove and recreate
function recreate(){
    if test -f "$1"; then
        rm $1

    fi
    touch $1
}   

function rm_dir_if_exist(){
    if [ -d "$1" ]; then
        rm -rf $1
    fi
}

function collect_est(){
    for file in `ls $1` 
    do
        if [ -d $1"/"$file ] ; then
            collect_est $1"/"$file
        elif [ "$file" = "results.txt" ];then
            res_temp=$1/$file
            ben_name=${1##*/}
            res=$(head $res_temp | sed -n '3p'| awk '{print $5}')
            input_res=${ben_name}' '${res}
            #echo "input_res : $input_res"
            echo $input_res >> $est_results_dir/all_est_results.txt
        fi
    done
}


function collect_sim(){
    for file in `ls $1` 
    do
        if [ -d $1"/"$file ] ; then
            collect_sim $1"/"$file
        elif [ "$file" = "sim.txt" ];then
            res_temp=$1/$file
            ben_name=${1##*/}
            res=$(cat  $res_temp | grep sim_cycle |awk '{print $2}')
            input_res=${ben_name}' '${res}
            echo $input_res
            echo $input_res >> $sim_results_dir/all_sim_results.txt
        fi
    done
}

function padding_res(){
    res=$1
    space_num=$((15 - ${#1}))
    for ((i=1;i<=$space_num;i++))
    do
        res="$res"" "
        #echo $i
    done
    res+='|'
    echo "$res"
}

function collect_results() {
    sim_dir=$est_results_dir/all_est_results.txt
    est_dir=$sim_results_dir/all_sim_results.txt
    report_dir=$cur_dir/results/report.txt

    all_names=$(cat $sim_dir | awk '{print $1}')
    name_array=(${all_names//,/ })
    est_values=$(cat $est_dir | awk '{print $2}')
    est_array=(${est_values//,/ })
    sim_values=$(cat $sim_dir | awk '{print $2}')
    sim_array=(${sim_values//,/ })
    
    split_line=""
    for i in {1..3}
    do
        split_line+='+'
        for j in {1..15}
        do
            split_line+='-'
        done
    done
    split_line+='+'

    recreate $report_dir

    echo $split_line >> $report_dir
    top_rule="|"
    top_rule+=`padding_res "benchmark"`
    top_rule+=`padding_res "simulation"`
    top_rule+=`padding_res "estimation"`
    echo "$top_rule" >> $report_dir
    echo $split_line >> $report_dir

    for ((i=0;i<${#name_array[*]};i++))
    do
        temp_line="|"
        temp_name=`padding_res ${name_array[$i]}`
        temp_est=`padding_res ${est_array[$i]}`
        temp_sim=`padding_res ${sim_array[$i]}`
        temp_line+=${temp_name};
        temp_line+=${temp_est};
        temp_line+=${temp_sim};
        echo "$temp_line" >> $report_dir
        echo "$split_line" >> $report_dir
    done

}

function begin(){
    rm_dir_if_exist results
    #estimate & simulation
    bash estimate.sh 
    bash simulation.sh

    # collect etsimation results
    result_est=$est_results_dir/all_est_results.txt
    recreate $result_est
    collect_est $est_results_dir
    #collect simulation results
    result_sim=$sim_results_dir/all_sim_results.txt
    recreate $result_sim
    collect_sim $sim_results_dir
    collect_results
}

begin
#collect_results
# test="name"
# padding_res $test 
# echo "$test"