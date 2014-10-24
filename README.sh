#!/bin/bash
echo ----------------------------------------------
echo README
echo updated 10/2014 ejo
echo run-psec4 command-line functions and arguments
echo Printed on $(uname -o) system on $(date)
echo ----------------------------------------------
if [ $(uname -o) = 'Cygwin' ]; then
    ./bin/refresh.exe -h
    ./bin/RawRead.exe -h
    ./bin/SetPed.exe -h
    ./bin/TakePed.exe -h
    ./bin/LinScan.exe -h
    ./bin/EnableTrig.exe -h
    ./bin/SetInternalTrig.exe -h
    ./bin/ScopeData.exe -h
    ./bin/LogData.exe -h
else
    ./bin/refresh -h
    ./bin/RawRead -h
    ./bin/SetPed -h
    ./bin/TakePed -h
    ./bin/LinScan -h
    ./bin/EnableTrig -h
    ./bin/SetInternalTrig -h
    ./bin/ScopeData -h
    ./bin/LogData -h     
fi
echo
echo ----------------------------------------------

