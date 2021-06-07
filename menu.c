
#include "menu.h"

bool menu_salir(List* routes, List* points) { return(false); }

bool mostrar_menu(List* routes, List* points)
{
    // Estas son todas las optiones en el menu principal.
    // cada una tiene una letra que la causa, un mensaje a mostrar
    // y una funcion a llamar en caso de ser seleccionada.
    MenuOption options[] = {
        {"1", "Importar archivo de coordenadas", menu_cargar_archivo},
        {"2", "Distancia entre dos entregas", menu_distancia_dos_entregas},
        {"3", "Tres entregas mas cercanas a un punto", menu_tres_entregas_mas_cercanas},
        {"4", "Crear ruta", menu_crear_ruta},
        {"5", "Generar ruta aleatoria", menu_generar_ruta_aleatoria},
        {"6", "Mejorar ruta", menu_mejorar_ruta},
        {"7", "Mostrar ruta", menu_mostrar_ruta},
        {"8", "Mejor ruta", menu_mejor_ruta},
        {"0", "Salir", menu_salir},
    };

    // Mostrar todas las opciones en pantalla
    printf("+----------+\n");
    printf("| Entregas |\n");
    printf("+----------+\n");
    printf("Ingrese una opcion:\n");

    MenuOption* option;
    int options_len = sizeof(options) / sizeof(options[0]);
    for(int i = 0; i < options_len; ++i)
    {
        option = &options[i];
        printf("%s. %s\n", option->option, option->msg);
    }

    // Obtener el input del usuario.
    char line[LINE_MAX];
    leer_linea("> ", line, sizeof(line), true);

    if (!(*line))
    { 
        printf("Ingrese una opcion\n"); 
        return(true); 
    }

    // Buscamos si la opcion corresponde a alguna de la lista.
    bool found = false;
    for(int i = 0; i < options_len; ++i)
    {
        option = &options[i];
        found = strcmp_insensitive(option->option, line) == 0;
        // printf("%c == %c? %s\n",option->option, selected_option, found ? "Y" : "N");

        if (found) 
        { 
            found = true; 
            break; 
        }
    }

    // De encontrar la funcion, la llamamos y retornamos lo que nos de,
    // sino, asumimos un mal input y preguntamos denuevo.
    if (found) 
    {
        // printf("calling %c's function!\n", option->option);
        return(option->func(routes, points)); 
    }
    else 
    { 
        printf("Opcion '%s' invalida\n", line);
        return(true);
    }
}


bool menu_cargar_archivo(List * routes, List * points)
{
	char respuesta[64];
	leer_linea("Ingrese la ruta al archivo a cargar: ", respuesta, sizeof(respuesta), false);
	List* cargados = load_from_file(respuesta);

	int largo_cargados = sizeOfList(cargados);
	if (largo_cargados > 0)
	{
		Point* punto = firstList(cargados);
		int count = 0;
		while(punto)
		{
			pushBack(points, punto);
			punto = nextList(cargados);
		}
		cleanList(cargados);
	}

	free(cargados);
	return(true);
}

bool menu_distancia_dos_entregas(List * routes, List * points)
{
	if (sizeOfList(points) == 0) { printf("No hay entregas!\n"); return(true); }

	printf("Lista de entregas:\n");
	Point* point = firstList(points);
	int i = 1;
	while(point)
	{
		printf("%d)\t<%d,%d>\n", i++, point->x, point->y);
		point = nextList(points);
	} printf("\n");

	int indice_a, indice_b;
	
	leer_numero("Ingrese el indice de la primera entrega: ", &indice_a, false);
	leer_numero("Ingrese el indice de la segunda entrega: ", &indice_b, false);
	if (indice_a == indice_b) { printf("Los indices son iguales, la distancia es 0\n"); return(true); }
	
	Point* point_a = getList(points, indice_a - 1);
	if (!point_a) { printf("El primer punto no existe\n"); return(true); }
	Point* point_b = getList(points, indice_b - 1);
	if (!point_b) { printf("El segundo punto no existe\n"); return(true); }

	float distance = distance_two_points(point_a, point_b);
	printf("La distancia entre <%d,%d> y <%d,%d> es de %f\n",
		point_a->x, point_a->y, point_b->x, point_b->y, distance);
	return(true);
}

bool menu_tres_entregas_mas_cercanas(List * routes, List * points)
{
	if (sizeOfList(points) == 0) { printf("No hay entregas!\n"); return(true); }

	printf("Ingrese las coordenadas del punto\n");
	int x, y;
	leer_numero("Ingrese x: ", &x, false);
	leer_numero("Ingrese y: ", &y, false);
	nearby_three(points, x, y);
  return(true);
}

bool menu_crear_ruta(List * routes, List * points)
{
	if (sizeOfList(points) == 0) { printf("No hay entregas!\n"); return (true); }
	printf("Ingrese las coordenadas\n");
	int x, y;
	leer_numero("Ingrese x: ", &x, false);
	leer_numero("Ingrese y: ", &y, false);
  
	
	crear_ruta(routes, points, x, y);
	return(true);
}

bool menu_generar_ruta_aleatoria(List * routes, List * points)
{
	if (sizeOfList(points) == 0) { printf("No hay entregas!\n"); return(true); }
	char nombre[STR_ITEM_MAX];
	leer_linea("Ingrese el nombre para la nueva ruta aleatoria: ", nombre, sizeof(nombre), false);
	Route* new_route = generate_random_route(points, nombre);
	pushBack(routes, new_route);
	return(true);
}

bool menu_mejorar_ruta(List * routes, List * points)
{
	if (sizeOfList(routes) == 0) { printf("No hay rutas!\n"); return(true); }

	int i = 0;
	Route* ruta = firstList(routes);
	while(ruta)
	{
		printf("%d)\t", ++i);
		print_route(ruta);
		printf("\n");
		ruta = nextList(routes);
	}

	int indice_ruta;
	leer_numero("Ingrese el numero de la ruta que desea optimizar: ", &indice_ruta, false);
	optimize_route( routes, indice_ruta);
	return(true);
}

bool menu_mostrar_ruta(List * routes, List * points)
{
	if (sizeOfList(routes) == 0) { printf("No hay rutas!\n"); return(true); }

	mostrar_rutas(routes);
	return(true);
}

bool menu_mejor_ruta(List * routes, List * points)
{
	if (sizeOfList(points) == 0) { printf("No hay entregas!\n"); return (true); }
	
	printf("Ingrese las coordenadas\n");
	int x, y;
	leer_numero("Ingrese x: ", &x, false);
	leer_numero("Ingrese y: ", &y, false);
	printf("Espere mientras se calcula la ruta optima...\n");
	Route* nueva_ruta = mejor_ruta(routes, points, x, y);
	pushBack(routes, nueva_ruta);

	printf("La ruta que minimiza la distancia a recorrer es: ");
	print_route(nueva_ruta);
	printf("\n");

	return(true);
}
