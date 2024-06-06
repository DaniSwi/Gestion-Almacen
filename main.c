#include <stdio.h>
#include <stdlib.h>

struct Producto {
  int codigo;
  char nombre[1000];
  float precio;
};


int main() {
  struct Producto carne;
  carne.codigo = 123456;
  strcpy(carne.nombre, "carne");
  carne.precio = 1000;
  
  printf("XES BEST GRUPO\n");
  return 0;
}