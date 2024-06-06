#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ELIMINAR ULTIMO CAMBIO ---> git reset --hard HEAD~1 (vengan a la puta ayudantia cabrones)

struct Producto {
  int codigo;
  char nombre[1000];
  float precio;
  //Fecha de vencimiento:
  int dia;
  int mes;
  int year;
};

int main() {
  struct Producto carne; 
  carne.dia = 12;
  carne.mes = 4;
  carne.year = 2025;
  carne.codigo = 12345678;
  carne.precio = 10000; //Precio por kilo?!?!?!?
  
  printf("XES BEST GRUPO\n");
  return 0;
}