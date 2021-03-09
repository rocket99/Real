#!/bin/bash

glslangValidator -G basic.frag -o basic_frag.spv
glslangValidator -G basic.vert -o basic_vert.spv

glslangValidator -G basic2D.vert -o basic2D_vert.spv
glslangValidator -G basic2D.frag -o basic2D_frag.spv

glslangValidator -G basic_compute.comp  -o basic_compute.spv 
