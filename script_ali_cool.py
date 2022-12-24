
from distutils.command.config import config
from genericpath import isdir
from posixpath import dirname
import subprocess
import os
from sys import stdout
from sys import argv


def execute_with(simulator, trace, ssd_config, temperature, peCycle, others):
    if others != "" :
        dir_name = './log' + '/PE' +  peCycle + '/' + others + '/' + trace
    else :
        dir_name = './log' + '/PE' +  peCycle + '/' + trace

    if(not os.path.isdir(dir_name)):
        os.makedirs(dir_name)

    subprocess.run([simulator,'./config/' + trace + '.cfg', './simplessd/config/' + ssd_config, dir_name])

if len(argv) < 2:
    exit()


## Usage : python3 script_ali_days.py {trace1} {trace2} ... {coolSize} {peCycle}

len_argv = len(argv)
peCycle = argv[len_argv - 1]
coolSize = argv[len_argv - 2]
for i in range (1, len_argv - 2):
    trace = argv[i]
    print("########################################################################################")
    print("Trace : " + trace + ", temperature : 25, PE : " + peCycle)
    print("########################################################################################")
    stdout.flush()
    ssd_config = "alibaba" + "_pe" + peCycle + "_" + "cool" + coolSize + ".cfg"
    others = "cool" + coolSize
    execute_with("./simplessd-standalone", trace, ssd_config, "25", peCycle, others)
    print("########################################################################################")
    print()