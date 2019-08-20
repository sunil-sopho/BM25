#!/bin/sh

cmd=$1
outfile=$2
evalfile=$3
if [ "${evalfile}" = '' ]; then
    evalfile=evalT
fi
if [ "${outfile}" = '' ]; then
    outfile=out
fi

if [ "${cmd}" = 'run' ]; then
    make
    ./bin/bm > $outfile
    ./data/trec_eval/trec_eval -q -m ndcg_cut.50 ./data/qrels.51-100.disk1.disk2.part1 ./$outfile > $evalfile
    grep "all" $evalfile
fi
