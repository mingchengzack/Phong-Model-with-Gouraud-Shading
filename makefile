project3: main.o object.o triangle.o line.o point.o slider.o utility.o
	g++ -g -std=c++11 -Wall -o project3 main.o line.o point.o utility.o object.o triangle.o slider.o -lglut -lGL

main.o: main.cpp utility.h object.h slider.h
	g++ -g -std=c++11 -Wall -c -o main.o main.cpp -lglut -lGL

object.o: object.cpp object.h point.h triangle.h
	g++ -g -std=c++11 -Wall -c -o object.o object.cpp -lglut -lGL

triangle.o: triangle.cpp utility.h point.h line.h
	g++ -g -std=c++11 -Wall -c -o triangle.o triangle.cpp -lglut -lGL

line.o: line.cpp line.h point.h utility.h
	g++ -g -std=c++11 -Wall -c -o line.o line.cpp -lglut -lglut -lGL

point.o: point.cpp point.h
	g++ -g -std=c++11 -Wall -c -o point.o point.cpp -lglut -lGL

slider.o: slider.cpp slider.h utility.h
	g++ -g -std=c++11 -Wall -c -o slider.o slider.cpp -lglut -lGL

utility.o: utility.cpp utility.h
	g++ -g -std=c++11 -Wall -c -o utility.o utility.cpp -lglut -lGL

clean:
	rm -f *.o project3
