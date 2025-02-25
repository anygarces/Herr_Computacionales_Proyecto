phony : test_anim.gif

test_anim.gif : PLOTS_polarizacion_campo_E.py datos_simulacion.txt
	py PLOTS_polarizacion_campo_E.py 

datos_simulacion.txt : polarizacion_campo_E.cpp 
	g++ polarizacion_campo_E.cpp -o wave.x
	.\wave.x

clean :
	del *.txt *.x *.gif *.png