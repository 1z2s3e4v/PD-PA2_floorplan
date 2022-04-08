CC=g++
LDFLAGS=-std=c++11 -O3 -lm
SOURCES=src/main.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=fp
INCLUDES=src/fp.h

a=0.5

all: $(SOURCES) bin/$(EXECUTABLE)

bin/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@
# ami33.block  ami33.nets  ami49.block  ami49.nets  apte.block  apte.nets  hp.block  hp.nets  xerox.block  xerox.nets
t0:
	./bin/$(EXECUTABLE) $(a) input_pa2/ami33.block input_pa2/ami33.nets output_pa2/ami33.rpt
t1:
	./bin/$(EXECUTABLE) $(a) input_pa2/ami49.block input_pa2/ami49.nets output_pa2/ami49.rpt
t2:
	./bin/$(EXECUTABLE) $(a) input_pa2/apte.block input_pa2/apte.nets output_pa2/apte.rpt
t3:
	./bin/$(EXECUTABLE) $(a) input_pa2/hp.block input_pa2/hp.nets output_pa2/hp.rpt
t4:
	./bin/$(EXECUTABLE) $(a) input_pa2/xerox.block input_pa2/xerox.nets output_pa2/xerox.rpt

v0:
	./checker/checker input_pa2/ami33.block input_pa2/ami33.nets output_pa2/ami33.rpt $(a)
v1:
	./checker/checker input_pa2/ami49.block input_pa2/ami49.nets output_pa2/ami49.rpt $(a)
v2:
	./checker/checker input_pa2/apte.block input_pa2/apte.nets output_pa2/apte.rpt $(a)
v3:
	./checker/checker input_pa2/hp.block input_pa2/hp.nets output_pa2/hp.rpt $(a)
v4:
	./checker/checker input_pa2/xerox.block input_pa2/xerox.nets output_pa2/xerox.rpt $(a)
clean:
	rm -rf *.o bin/$(EXECUTABLE)
