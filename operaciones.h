#pragma once


#include "common.h"
#include <math.h>

#include "list.h"
#include "read.h"
#include "parse.h"

#include "strcmp_insensitive.h"

typedef struct Point
{
    int x, y;
} Point;

typedef struct Route
{
  List *points;
  int *indices;
  size_t size;
  float length;
  char nombre[30];
} Route;

List *load_from_file(const char *path);

Route *createRoute(List *points, char* name);
void update_route_length(Route* route);
void mostrar_rutas(List* routes);
Route *generate_random_route(List *points, char* name);
int compare_routes(const void* a, const void* b);
void print_route(Route* route);
float distance_two_points(Point *a, Point *b);
void nearby_three(List* points, int x, int y);
void optimize_route(List *routes, int num);
Route* mejor_ruta(List* routes, List* points, int x, int y);
Route* crear_ruta(List* routes, List* points, int x, int y);