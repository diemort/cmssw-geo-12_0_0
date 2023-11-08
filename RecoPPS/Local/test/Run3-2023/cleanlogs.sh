#!/bin/bash

rm -rf $( basename ${1} .dag ).dag.*
rm -rf logs/$( basename ${1} .dag )*
