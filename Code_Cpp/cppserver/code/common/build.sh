#!/bin/bash 

rm -rf _
mkdir _
cd  _ 

cmake ../
make -j8
