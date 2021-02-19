#!/bin/bash

cp -rf shader/ ./build/ 

cd ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

make 
