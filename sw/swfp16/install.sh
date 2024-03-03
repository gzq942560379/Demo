#!/bin/bash


install_prefix=/home/export/online1/mdt00/shisuan/jiaweile/guozhuoqiang/DeepFlame/software/swfp16

source_dir=/home/export/base/shisuan/jiaweile/online/guozhuoqiang/Demo/sw/swfp16

mkdir -p $install_prefix

mkdir -p $install_prefix/include
mkdir -p $install_prefix/lib

cp $source_dir/swfp16.h $install_prefix/include
cp $source_dir/libswfp16.a $install_prefix/lib


