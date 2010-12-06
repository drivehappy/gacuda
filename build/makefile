INC=/usr/local/cuda/include
LIB=/usr/local/cuda/lib64
OUT=../out/gaCudaHook.so
OUT_MAKE=../out/makeHook.so
OUT_MAIN=../out/gaCudaMain

all: gaCudaHook gaCudaMain

gaCudaHook:
	g++ ../src/gaCudaHook.cpp -I$(INC) -L$(SRC) -lcudart -O2 -o $(OUT) -fPIC -shared

gaCudaMain:
	g++ ../src/gaCudaMain.cpp -I$(INC) -L$(SRC) -O2 -o $(OUT_MAIN)

makeHook:
	g++ ../src/makeHook.cpp -O2 -o $(OUT_MAKE) -ldl -fPIC -shared

clean:
	rm *.so
