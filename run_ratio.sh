#!/bin/bash
echo 'alibaba_30 alibaba_130 alibaba_150 alibaba_230 alibaba_401 3000'

trace_names=("alibaba_30" "alibaba_130" "alibaba_150" "alibaba_230" "alibaba_401")
#trace_names=("alibaba_330" "alibaba_430")

for (( i = 0 ; i < ${#trace_names[@]} ; i++ )) ; do
    #echo  >$(pwd)/log/stdout/"${trace_names[$i]}_3000.txt"
    echo  >$(pwd)/log/stdout/"${trace_names[$i]}_3000_0.05.txt"
    echo  >$(pwd)/log/stdout/"${trace_names[$i]}_3000_0.08.txt"
    #echo  >$(pwd)/log/stdout/"${trace_names[$i]}_3000_22days.txt"
    #echo  >$(pwd)/log/stderr/"${trace_names[$i]}_3000.txt"
done

for (( i = 0 ; i < ${#trace_names[@]} ; i++ )) ; do
    #echo  >$(pwd)/log/stdout/"${trace_names[$i]}_5000.txt"
    #echo  >$(pwd)/log/stderr/"${trace_names[$i]}_5000.txt"
    echo  >$(pwd)/log/stderr/"${trace_names[$i]}_3000_0.05.txt"
    echo  >$(pwd)/log/stderr/"${trace_names[$i]}_3000_0.08.txt"
done

for (( i = 0 ; i < ${#trace_names[@]} ; i++ )) ; do
    #python3 script_ali.py ${trace_names[$i]} 3000 >>$(pwd)/log/stdout/"${trace_names[$i]}_3000.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_3000.txt" &
    #python3 script_ali.py ${trace_names[$i]} 5000 >>$(pwd)/log/stdout/"${trace_names[$i]}_5000.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_5000.txt" &
    #python3 script_ali_days.py ${trace_names[$i]} 22 3000 >>$(pwd)/log/stdout/"${trace_names[$i]}_3000_22days.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_3000_22days.txt" &
    python3 script_ali_ratio.py ${trace_names[$i]} 0.05 3000 >>$(pwd)/log/stdout/"${trace_names[$i]}_3000_0.05.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_3000_0.05.txt" &
    python3 script_ali_ratio.py ${trace_names[$i]} 0.08 3000 >>$(pwd)/log/stdout/"${trace_names[$i]}_3000_0.08.txt" 2>>$(pwd)/log/stderr/"${trace_names[$i]}_3000_0.08.txt" &
done


#alibaba_430