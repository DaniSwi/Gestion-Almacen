#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

//ELIMINAR ULTIMO CAMBIO ---> git reset --hard HEAD~1 (vengan a la puta ayudantia cabrones)

typedef struct {
  int dia;
  int mes;
  int year;
} fechaVencimiento;

typedef struct {
  int precioCosto;
  int precioVenta;
  int cantidad;
  char nombre[10000];
  char tipoProducto[1000];
  fechaVencimiento fechaV;
} Producto;

typedef struct {
  char nombre[10000];
  int deuda;
  int edad;
} Cliente;

typedef struct {
  int monto;
  Producto productoPedido;
  Cliente cliente;
} Pedido;

Pedido *crearPedido() {
  Pedido *pedido = (Pedido *)malloc(sizeof(Pedido));
  return pedido;
}


void cargarPedidos(Map *pedidos){
  FILE *archivo = fopen("pedidos.csv", "r");
  if(archivo == NULL){
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ',');
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Pedido *pedido = crearPedido();
    pedido->monto = atoi(campos[0]);
    pedido->productoPedido.precioVenta = atoi(campos[1]);
  }
  
}









int main() {
  
  
  
  
  return 0;
}