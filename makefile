all : hell.l hell.y
	clear
	flex -i hell.l
	bison hell.y
	gcc hell.tab.c -o analisador -lm
	./analisador
