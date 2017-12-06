#!/bin/zsh
for f in test/*; do
	./abe $f < param/a.param
done;
