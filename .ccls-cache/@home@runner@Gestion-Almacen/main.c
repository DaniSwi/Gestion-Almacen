#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

//ELIMINAR ULTIMO CAMBIO ---> git reset --hard HEAD~1 (vengan a la puta ayudantia cabrones)

typedef struct{
  int dia;
  int mes;
  int year;
} fechaVencimiento;

typedef struct{
  int precioCosto;
  int precioVenta;
  int cantidad;
  char nombre[10000];
  char tipoProducto[1000];
  fechaVencimiento fechaV;
}Producto;

typedef struct{
  char nombre[10000];
  int deuda;
  int edad;
}Cliente;

typedef struct{
  int monto;
  Producto productoPedido;
  Cliente cliente;
}Pedido;

Pedido *crearPedido(){
  Pedido *pedido = (Pedido *)malloc(sizeof(Pedido));
  return pedido;
}


void cargarPedidos(List *pedidos){
  FILE *archivo = fopen("pedidos.csv", "r");
  if(archivo == NULL){
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ',');
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Pedido *pedido = crearPedido();
    pedido->productoPedido.precioVenta = atoi(campos[0]); 
    pedido->productoPedido.cantidad = atoi(campos[1]);
    strcpy(pedido->productoPedido.nombre, campos[2]);
    strcpy(pedido->productoPedido.tipoProducto, campos[3]);
    list_pushBack(pedidos, pedido);
  }
  fclose(archivo);
  Pedido *aux = (Pedido *)list_first(pedidos);
  while(aux){
    printf("Precio: %d, Cantdad: %d, Producto: %s, Tipo de producto: %s\n", aux->monto, aux->productoPedido.cantidad, aux->productoPedido.nombre, aux->productoPedido.tipoProducto);
    aux = (Pedido *)list_next(pedidos);
  }
}









int main() {

  char opcion;
  List *pedidos = list_create();

  
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);
    switch(opcion){
      case '1':
        cargarPedidos(pedidos);
        break;
    }
    
  } while(opcion != 4);
  
  
  return 0;
}