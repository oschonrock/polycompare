
#!/usr/bin/bash

OPTIND=1
rerun=0

while getopts ":r" opt
do
    case "$opt" in
	h|\?)
	    echo " -r to rerun benchmarks "
	    exit 0
	    ;;
	r)  rerun=1
	    ;;
    esac
done

shift $((OPTIND-1))

phases="populate iterate both"

for phase in $phases
do
    csvfile=results/$phase.csv
    if [[ $rerun == 1 ]]
    then
	benchmarks="poly_vec poly_virt poly_var"
	first=true
	for benchmark in $benchmarks
	do
	    if [[ $first == true ]]
	    then
		op="> $csvfile"
		first=false
	    else
		op="| grep -v '^name' >> $csvfile"
	    fi
	    ./build/$benchmark --benchmark_format=csv --benchmark_filter=$phase $op
	done
    fi
    python google_benchmark_plot/plot.py --logx < $csvfile &
done
