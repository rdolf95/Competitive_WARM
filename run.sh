#!/bin/bash

#trace_names=("alibaba_30" "alibaba_130" "alibaba_150" "alibaba_230" "alibaba_401")
#trace_names=("alibaba_12" "alibaba_100" "alibaba_130" "alibaba_507" "alibaba_538" "alibaba_730" "alibaba_731" "alibaba_743" "alibaba_792" "alibaba_124" "alibaba_806")
#trace_names=("alibaba_507" "alibaba_792")
#trace_names=("alibaba_12" "alibaba_100" "alibaba_731")
trace_names=("alibaba_727")


for (( i = 0 ; i < ${#trace_names[@]} ; i++ )) ; do
    echo  >$(pwd)/log/stdout/"${trace_names[$i]}_3000.txt"
    echo  >$(pwd)/log/stderr/"${trace_names[$i]}_3000.txt"
done

#for (( i = 0 ; i < ${#trace_names[@]} ; i++ )) ; do
#    echo  >$(pwd)/log/stdout/"${trace_names[$i]}_5000.txt"
#    echo  >$(pwd)/log/stderr/"${trace_names[$i]}_5000.txt"
#done

for (( i = 0 ; i < ${#trace_names[@]} ; i++ )) ; do
    python3 script_ali.py ${trace_names[$i]} 3000 >>$(pwd)/log/stdout/"${trace_names[$i]}_3000.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_3000.txt" &
    #python3 script_ali.py ${trace_names[$i]} 5000 >>$(pwd)/log/stdout/"${trace_names[$i]}_5000.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_5000.txt" &
done


#alibaba_430