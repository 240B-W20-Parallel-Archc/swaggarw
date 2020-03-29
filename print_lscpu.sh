#/bin/bash

d1size=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL1_DCACHE_SIZE | cut -d ' ' -f 2`
d1assoc=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL1_DCACHE_ASSOC | cut -d ' ' -f 2`
d1line=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL1_DCACHE_LINESIZE | cut -d ' ' -f 2`

d2size=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL2_CACHE_SIZE | cut -d ' ' -f 2`
d2assoc=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL2_CACHE_ASSOC | cut -d ' ' -f 2`
d2line=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL2_CACHE_LINESIZE | cut -d ' ' -f 2`

d3size=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL3_CACHE_SIZE | cut -d ' ' -f 2`
d3assoc=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL3_CACHE_ASSOC | cut -d ' ' -f 2`
d3line=`getconf -a | grep CACHE | tr -s ' ' | grep LEVEL3_CACHE_LINESIZE | cut -d ' ' -f 2`

cpus=`lscpu | grep "Core(s) per socket" | tr -s ' ' | cut -d ' ' -f 4`
threads=`lscpu | grep "CPU(s):" | tr -s ' ' | cut -d ' ' -f 2`

echo ""
echo ""

echo "Maximum Possible Threads: ${threads}"
echo "CPUS: ${cpus}"

echo ""
echo ""

echo -e "Level 1 Cache:"
echo "Size: ${d1size}"
echo "Associativity: ${d1assoc}"
echo "Line Size: ${d1line}"
echo ""
echo ""

echo "Level 2 Cache:"
echo "Size: ${d2size}"
echo "Associativity: ${d2assoc}"
echo "Line Size: ${d2line}"
echo ""
echo ""

echo "Level 3 Cache:"
echo "Size: ${d3size}"
echo "Associativity: ${d3assoc}"
echo "Line Size: ${d3line}"
echo ""
echo ""