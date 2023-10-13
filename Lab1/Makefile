run: build
	./main_crcw; ./main_crew

build: crcw crew

crcw:
	g++ crcw.cpp sequential_multiplier.cpp -o main_crcw -lpthread

crew:
	g++ crew.cpp sequential_multiplier.cpp -o main_crew -lpthread

clean:
	rm main_crcw main_crew
