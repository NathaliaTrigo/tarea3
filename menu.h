#pragma once

#include "common.h"
#include "parse.h"
#include "read.h"
#include "strcmp_insensitive.h"

#include "operaciones.h"

// MenuFunc es un puntero a una funcion de tipo bool name(void)
typedef bool MenuFunc(List*, List*);

// MenuOption es una 
// "tupla" de (letra, mensaje a mostrar, ptr a funcion)
typedef struct MenuOption 
{ 
    const char* option;
    const char* msg; 
    MenuFunc* func;
} MenuOption;

bool mostrar_menu(List* routes, List* points);
bool menu_cargar_archivo(List* routes, List* points);
bool menu_distancia_dos_entregas(List* routes, List* points);
bool menu_tres_entregas_mas_cercanas(List* routes, List* points);
bool menu_crear_ruta(List* routes, List* points);
bool menu_generar_ruta_aleatoria(List* routes, List* points);
bool menu_mejorar_ruta(List* routes, List* points);
bool menu_mostrar_ruta(List* routes, List* points);
bool menu_mejor_ruta(List* routes, List* points);
bool menu_salir(List* routes, List* points);