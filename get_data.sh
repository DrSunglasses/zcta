#!/bin/sh
mkdir -p data
cd data
wget -nd -i ../data_file_list.txt
for zipfile in *.zip; do
	unzip $zipfile;
	rm $zipfile
done;
