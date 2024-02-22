#!/bin/bash
compute_ilp(){
    if [ "${1: -1}" = "/" ];then
        path="${1}" 
    else
        path="${1}""/"
    fi
    # create result directory
    if [ ! -d "${path}"result ];then
        mkdir result
    else
        echo "result already exist"
    fi

    for file in `ls ${path}`
    do
        if [ "${file#*.}" = "lp" ];then
            #echo "./lp_solve -rxli ./xli_CPLEX "${path}""${file}" > "${file%%.*}".res"
            ./lp_solve -rxli ./xli_CPLEX "${path}""${file}" > "result/${file%%.*}".res
            echo ""${path}""${file}" finished... "
        fi
    done
    echo "all finished!"
}

compute_ilp $1
