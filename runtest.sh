#!/bin/zsh
for f in datatest/*; do
	./abe $f < param/a.param
done;
