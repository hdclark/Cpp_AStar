#!/bin/bash


echo "g++ $@  -std=c++11 Utilities.cc astartest_2d_4.cc -o AStarTest_2D_4"
g++  $@  -std=c++11 Utilities.cc astartest_2d_4.cc -o AStarTest_2D_4


echo "g++ $@  -std=c++11 Utilities.cc astartest_2d_3.cc -o AStarTest_2D_3"
g++  $@  -std=c++11 Utilities.cc astartest_2d_3.cc -o AStarTest_2D_3


echo "g++ $@  -std=c++11 Utilities.cc astartest_2d_2.cc -o AStarTest_2D_2"
g++  $@  -std=c++11 Utilities.cc astartest_2d_2.cc -o AStarTest_2D_2


echo "g++ $@  -std=c++11 Utilities.cc astartest_2d.cc -o AStarTest_2D"
g++  $@  -std=c++11 Utilities.cc astartest_2d.cc -o AStarTest_2D


echo "g++ $@  -std=c++11 Utilities.cc astartest_3d.cc -o AStarTest_3D"
g++  $@  -std=c++11 Utilities.cc astartest_3d.cc -o AStarTest_3D
