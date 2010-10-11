all: install

gaCudaHook:
	g++ ../src/gaCudaHook.cpp -I/usr/local/cuda/include -L/usr/local/cuda/lib -lcuda -lcudart -lcublas -O2 -o gaCudaHook.so -fPIC -shared

install: gaCudaHook
	cp gaCudaHook.so /home/mharmer/dev/cs791v/lab1/out

clean:
	rm *.so
