#!/bin/bash

for i in *.JPG; do echo ${i%.JPG}; convert $i ${i%.JPG}.ppm; done
for i in *.jpg; do echo ${i%.jpg}; convert $i ${i%.jpg}.ppm; done
