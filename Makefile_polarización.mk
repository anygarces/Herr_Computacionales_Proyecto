phony : test_anim1.gif test_anim2.gif test_animA.gif 

test_anim1.gif : PLOTS1_polarizacion_campo_E.py datos_simulacion1.txt
	py PLOTS1_polarizacion_campo_E.py 

datos_simulacion1.txt : polarizacion_campo_E.cpp 
	g++ polarizacion_campo_E.cpp -o wave1.x
	.\wave1.x

test_anim2.gif : PLOTS2_polarizacion_campo_E.py datos_simulacion2.txt
	py PLOTS2_polarizacion_campo_E.py 

datos_simulacion2.txt : E_wave.cpp 
	g++ E_wave.cpp -o wave2.x
	.\wave2.x

test_animA.gif : PLOTSA.py datos_simulacionA.txt
	py PLOTSA.py 

datos_simulacionA.txt : Advection_E.cpp 
	g++ Advection_E.cpp -o waveA.x
	.\waveA.x

clean :
	del *.txt *.x *.gif *.png
