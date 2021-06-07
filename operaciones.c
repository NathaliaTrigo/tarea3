#include "operaciones.h"


Route *createRoute(List *points, char* name)
{
	Route *new_route = malloc(sizeof(*new_route));
	size_t num_points = sizeOfList(points);
	
	new_route->points = points;
	new_route->indices = malloc(num_points * sizeof(*new_route->indices));
	new_route->size = num_points;
	new_route->length = 0.0f;
	strcpy(new_route->nombre, name);

	for(size_t i = 0; i < num_points; ++i) { new_route->indices[i] = -1; }

    return (new_route);
}

void update_route_length(Route* route)
{
	int num_indices = route->size;
	route->length = 0.0f;
	for(int i = 0; i < num_indices - 1; ++i)
	{
		int index_a = route->indices[i];
		int index_b = route->indices[i+1];

		Point* point_a = getList(route->points, index_a);
		Point* point_b = getList(route->points, index_b);
		
		float dist = distance_two_points(point_a, point_b);
		route->length += dist;
	}
}

Route *generate_random_route(List* points, char* name)
{
	Route *new_route = createRoute(points, name);
	size_t num_points = new_route->size;

    // Nones
	for(int i = 0; i < num_points; ++i)
	{
		int test = -1;
		while(true)
		{
			test = rand() % num_points;
			// Check if in use
			bool used = false;
			for(int i = 0; i < num_points; ++i)
			{
				if (new_route->indices[i] == test) { used = true; }
			}

			if (!used) { break; }

		}
		
		new_route->indices[i] = test;
	}

	update_route_length(new_route);

    return (new_route);
}

List *load_from_file(const char *path)
{
	List *points = createList();

    FILE *fptr = fopen(path, "r");
    if (!fptr)
    {
        printf("Error al abrir '%s'\n", path);
        return (points);
    }
    
    int *largo;
	int load_count = 0;

    char line_buffer[4096];
    while (fgets(line_buffer, sizeof(line_buffer), fptr))
    {
        line_buffer[strcspn(line_buffer, "\n")] = 0;
        char *line_ptr = line_buffer;

        // Skip empty lines
        if (!line_ptr || *line_ptr == '\n')
        {
            // printf("Skipping empty line!\n");
            continue;
        }

        char piece_buffer[256];
        Point point;

        // Get X
        line_ptr = trim_whitespace(line_ptr);
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ' ', true) ||
            !string_to_int(piece_buffer, &point.x))
        {
            printf("Failed to convert %s to integer!\n", piece_buffer);
            continue;
        }

        line_ptr = trim_whitespace(line_ptr);
        // Get Y
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ' ', true) ||
            !string_to_int(piece_buffer, &point.y))
        {
            printf("Failed to convert %s to integer!\n", piece_buffer);
            continue;
        }

        // Si tenemos un punto, crear uno nuevo y a la lista.
        Point *new_point = calloc(1, sizeof(*new_point));
        new_point->x = point.x;
        new_point->y = point.y;
        pushBack(points, new_point);
        largo++;
		++load_count;
    }

	fclose(fptr);
	printf("Se cargaron %d entregas\n", load_count);

    return (points);
}

float distance_two_points(Point *a, Point *b)
{
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    float result = sqrtf(dx * dx + dy * dy);
    return (result);
}

int compare_routes(const void* a, const void* b)
{
	Route* valor_a = *(Route**)a;
	Route* valor_b = *(Route**)b;
	return(valor_a->length - valor_b->length);
}

void print_route(Route* route)
{
	printf("%s: <", route->nombre);
	for(int i = 0; i < route->size; ++i)
	{
		if (i < route->size - 1) {
			printf("%d, ", route->indices[i] + 1);
		} else {
			printf("%d", route->indices[i] + 1);
		}
	}
	printf("> distancia total es: %f\n", route->length);
}

Route** obtener_rutas_ordenadas(List* routes)
{
	int num_rutas = sizeOfList(routes);
	Route** resultado = malloc(sizeof(*resultado) * num_rutas);
	
	Route* ruta = firstList(routes); 
	int i = 0;
	while(ruta)
	{
		resultado[i] = ruta;
		ruta = nextList(routes);
		++i;
	}

	qsort(resultado, num_rutas, sizeof(*resultado), compare_routes);

	return(resultado);
}

void mostrar_rutas(List* routes)
{
	Route** para_mostrar = obtener_rutas_ordenadas(routes);

	//printf("done\n");
	//Mostramos los puntos en orden
	for(int i = 0; i < sizeOfList(routes); ++i)
	{
		print_route(para_mostrar[i]);
		printf("\n");
	}

	free(para_mostrar);
}

typedef struct dist_with_id
{
	float dist;
	int id;
} dist_with_id;

int compare_dist_with_id(const void* a, const void* b)
{
	dist_with_id* dist_a = (dist_with_id*)a;
	dist_with_id* dist_b = (dist_with_id*)b;
	return(dist_a->dist - dist_b->dist);
}

void nearby_three(List* points, int x, int y)
{
	Point p = {x, y};
	int num_points = sizeOfList(points);
	dist_with_id* dists = malloc(num_points * sizeof(*dists));
	
	Point* current = firstList(points);
	for(int i = 0; i < num_points; ++i)
	{
		dists[i] = (dist_with_id){ distance_two_points(&p, current), i };
		current = nextList(points);
	}

	qsort(dists, num_points, sizeof(*dists), compare_dist_with_id);

	printf("Los 3 mas cercanos al punto <%d, %d> son:\n", x, y);
	for(int i = 0; i < 3; ++i)
	{
		Point* point = getList(points, dists[i].id);
		printf("<%d, %d, %f>\n", point->x, point->y, dists[i].dist);
	}

	free(dists);
}

void optimize_route(List *routes, int num_from_zero){
	int num = num_from_zero - 1;
	Route* ruta = getList(routes, num);
	printf("Usted escogio la ruta %d: ", num_from_zero);
	print_route(ruta);
  printf("\n");

	char respuesta[STR_ITEM_MAX];
	leer_linea("Desea optimizar de forma automatica? SI/NO: ", respuesta, sizeof(respuesta), false);
	
	bool automatic;
	if (strcmp_insensitive(respuesta, "si") == 0)
	{
		automatic = true;
	}
	else
	{
		automatic = false;
	}

	int indice_a, indice_b;
	size_t ruta_size = ruta->size;
	// Obtener los dos indices a intercambiar.
	if (automatic)
	{
		indice_a = rand() % ruta_size;
		indice_b = rand() % ruta_size;
		while(indice_a == indice_b) { indice_b = rand() % ruta_size; }
	}
	else
	{
non_automatic_get_indice_a:
		leer_numero("Ingrese el primer indice: ", &indice_a, false);
		if (indice_a == 0) { printf("Los indices parten desde el uno.\n"); goto non_automatic_get_indice_a; }

non_automatic_get_indice_b:
    	leer_numero("Ingrese el segundo indice: ", &indice_b, false);
		if (indice_b == 0) { printf("Los indices parten desde el uno.\n"); goto non_automatic_get_indice_b; }
		while(indice_a == indice_b) { 
			printf("Los indices deben ser diferentes!\n");
			leer_numero("Ingrese el segundo indice: ", &indice_b, false); 
			if (indice_b == 0) { printf("Los indices parten desde el uno.\n"); goto non_automatic_get_indice_b; }
		}
		--indice_a; --indice_b;
	}

	Point* punto_a = getList(ruta->points, indice_a);
	Point* punto_b = getList(ruta->points, indice_b);

	printf("Se intercambiaran los indices %d y %d, que corresponden a los puntos <%d,%d> y <%d,%d> respectivamente\n",
		indice_a + 1, indice_b + 1, punto_a->x, punto_a->y, punto_b->x, punto_b->y);

	int temp = ruta->indices[indice_a];
	ruta->indices[indice_a] = ruta->indices[indice_b];
	ruta->indices[indice_b] = temp;

	float old_length = ruta->length;
	update_route_length(ruta);

	if (old_length < ruta->length)
	{
		printf("La ruta anterior era mejor, con una distancia de %f, mientras que la nueva tiene una de %f, volviendo atras.\n",
		old_length, ruta->length);
		int temp = ruta->indices[indice_a];
		ruta->indices[indice_a] = ruta->indices[indice_b];
		ruta->indices[indice_b] = temp;
		ruta->length = old_length;
	}
	else
	{
		printf("La nueva ruta es mejor, pasando de una distancia de %f a una de %f, conservandola.\n",
			old_length, ruta->length);
	}
}

Route* crear_ruta(List* routes, List* points, int x, int y)
{
	Point p = {x, y};
	int num_points = sizeOfList(points);
	dist_with_id* dists = malloc(num_points * sizeof(*dists));
	bool* in_use = calloc(num_points, sizeof(bool));
	
	Point* current = firstList(points);
	for(int i = 0; i < num_points; ++i)
	{
		dists[i] = (dist_with_id){ distance_two_points(&p, current), i };
		current = nextList(points);
	}

	qsort(dists, num_points, sizeof(*dists), compare_dist_with_id);


	Route* new_route = malloc(sizeof(*new_route));
	new_route->size = num_points;
	new_route->indices = calloc(num_points, sizeof(*new_route->indices));
	new_route->length = 0.0f;
	new_route->points = points;

	int count = 0;
	while(count < num_points)
	{
		Point* current = firstList(points);
		int i = 0;
		while(current)
		{
			// Nos saltamos los que ya fueron elegidos previamente...
			if (in_use[i]) { current = nextList(points); ++i; continue; }

			printf("%d)\t<%d, %d>\n", i + 1, current->x, current->y);
			++i;
			current = nextList(points);
		} printf("\n");

		// Obtener del usuario el indice a agregar.
		int indice;

add_points_to_route:
		leer_numero("Ingrese el indice a agregar a la ruta: ", &indice, false);
		if (indice == 0 || indice > num_points + 1) { 
			printf("Ingrese un indice en el rango [%d-%d]\n", 1, num_points + 1);
			goto add_points_to_route;
		}

		// Comprobar que no este repetido
		if (in_use[indice-1]) {
			printf("El indice %d ya esta en uso, elija otro.\n", indice);
			goto add_points_to_route;
		}
		
		in_use[indice-1] = true;
		new_route->indices[count] = indice-1;
		++count;
	}
	update_route_length(new_route);

	Point* last_p = getList(points, new_route->indices[num_points - 1]);
	float dist_last_and_point = distance_two_points(&p, last_p);
	new_route->length += dist_last_and_point;

	char nombre[STR_ITEM_MAX];
	leer_linea("Ingrese el nombre de la nueva ruta: ", nombre, sizeof(nombre), false);
	strcpy(new_route->nombre, nombre);

	free(in_use);
	free(dists);

	pushBack(routes, new_route);
	return(new_route);
}

void mejor_ruta_do(Route* route, int* intentos)
{
	int indice_a, indice_b;
	int ruta_size = route->size;
	
	for(int i = 0; i < 500000; ++i)
	{
		// Mejorar ruta
		indice_a = rand() % ruta_size;
		indice_b = rand() % ruta_size;
		while(indice_a == indice_b) { indice_b = rand() % ruta_size; }

		Point* punto_a = getList(route->points, indice_a);
		Point* punto_b = getList(route->points, indice_b);

		int temp = route->indices[indice_a];
		route->indices[indice_a] = route->indices[indice_b];
		route->indices[indice_b] = temp;

		float old_length = route->length;
		update_route_length(route);

		// printf("Despues de cambiar dio un largo de: %f\n", route->length);

		if (old_length < route->length)
		{
			//printf("La ruta anterior era mejor, con una distancia de %f, mientras que la nueva tiene una de %f, volviendo atras.\n",
			//  old_length, ruta->length);
			int temp = route->indices[indice_a];
			route->indices[indice_a] = route->indices[indice_b];
			route->indices[indice_b] = temp;
			route->length = old_length;
		}

		(*intentos)++;
		if (i % 100 == 0){ printf("Iteracion de optimizacion %d...\r", i);}
	}
	printf("Listo.                                        \n");
}

Route* mejor_ruta(List* routes, List* points, int x, int y)
{
	Route* result = generate_random_route(points, "ruta optima");
	float old_dist = 0;
	float dist_inicial = result->length;
	int maximo_intentos = 1;
	int intentos = 0;
	while (result->length >= old_dist && maximo_intentos > 0) { 
		if (old_dist != 0) { printf("Tratando de mejorar la ruta, el largo actual es de %f\n", old_dist); }
		old_dist = result->length;
		mejor_ruta_do(result, &intentos); 
		--maximo_intentos;
	}

	float dist_final = result->length;
	printf("Despues de %d iteraciones, la distancia bajo de %f a %f\n", intentos, dist_inicial, dist_final);
	return(result);
}
