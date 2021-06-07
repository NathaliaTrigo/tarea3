#include "common.h"
#include "operaciones.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(NULL));

	List* routes = createList();
	List* points = createList();

	bool running = true;
	while(running){
		bool result = mostrar_menu(routes, points);
    	if (!result) { running = false; }
		// else { fancy_pause(); }
  	}

	// Liberar lista points
	Point* p = firstList(points);
	while(p)
	{ 
		free(p);
		p = nextList(points); 
	}
	free(points);
	
	// Liberar lista rutas.
	Route* r = firstList(routes);
	while(r)
	{
		free(r->indices);
		free(r);
		r = nextList(routes); 
	}
	free(routes);
	
	printf("Goodbye!\n");
	return (0);
}
