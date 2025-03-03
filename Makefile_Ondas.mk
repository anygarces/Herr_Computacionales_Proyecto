phony : wave_simulation.gif Advection_simulation.gif

wave_simulation.gif : graf_onda.py wave.x 
	py graf_onda.py

wave.x : onda.cpp 
	g++ onda.cpp -o wave.x
	.\wave.x

Advection_simulation.gif : graf_advection.py advection.x 
	py graf_advection.py

advection.x : advection.cpp 
	g++ advection.cpp -o advection.x
	.\advection.x


