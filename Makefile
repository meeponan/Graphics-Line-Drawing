CC = gcc
NAME = liu_xiuwen
LIBS = -lglut32 -lglu32 -lopengl32
CFLAGS = -O3

LIBS =  -L/usr/X11R6/lib/ -O2 -lglut -lGLU -lGL -lXmu -lXt -lSM -lICE -lXext -lX11 -lXi -lXext -lX11 -lm

lab1: lab1.o SSD_util.o
	$(CC) -o lab1 lab1.o SSD_util.o $(LIBS)
lab1_extra: lab1_extra.o SSD_util.o
	$(CC) -o lab1_extra lab1_extra.o SSD_util.o $(LIBS)
.c.o: 
	$(CC)  $(CFLAGS) -c  $(COPT) $<
tar:
	tar cvfz lab1_$(NAME).tar.gz *.c *.h 
	ls -l lab1_$(NAME).tar.gz
run:
	./lab1 lab1_dino.ssd
	./lab1 lab1_hex.ssd
	./lab1 lab1_tri.ssd
	./lab1 lab1_pie_chart.ssd

clean:
	rm  *.o

