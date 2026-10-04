OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17 
CXXFILES = projet.cc message.cc simulation.cc shape.cc lifeform.cc gui.cc graphic.cc
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
OFILES = projet.o message.o simulation.o shape.o lifeform.o gui.o graphic.o

all: $(OUT)

gui.o: gui.cc gui.h graphic_gui.h graphic.h simulation.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
graphic.o: graphic.cc graphic_gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

projet.o: projet.cc gui.h simulation.cc
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
simulation.o: simulation.cc message.h lifeform.h shape.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
shape.o: shape.cc shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
lifeform.o: lifeform.cc message.h shape.h lifeform.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "Cleaning compilation files"
	@rm *.o $(OUT) *.cc~ *.h~

