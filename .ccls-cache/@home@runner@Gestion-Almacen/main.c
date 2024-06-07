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
}fechaVencimiento;

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

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

Pedido *crearPedido(){
  Pedido *pedido = (Pedido *)malloc(sizeof(Pedido));
  return pedido;
}

Producto *crearProducto(){
  Producto *producto = (Producto *)malloc(sizeof(Producto));
  return producto;
}

void gestionarInventario(Map *inventario){
  
}

void actualizarInventario(Map *inventario, char *nombreProducto, int cantidad){
  MapPair *pair = map_search(inventario, nombreProducto);
  if(pair == NULL)
    puts("No se encontró el producto");
  else {
    Producto *producto = (Producto *)pair->value;
    producto->cantidad -= cantidad;
  }
}


void cargarPedidosCSV(List *pedidos, Map *inventario){
  FILE *archivo = fopen("data/pedidos.csv", "r");
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
    printf("Precio: %d, Cantdad: %d, Producto: %s, Tipo de producto: %s\n", aux->productoPedido.precioVenta, aux->productoPedido.cantidad, aux->productoPedido.nombre, aux->productoPedido.tipoProducto);
    actualizarInventario(inventario, aux->productoPedido.nombre, aux->productoPedido.cantidad);
    aux = (Pedido *)list_next(pedidos);
  }
}






int main() {

  int opcion;
  List *pedidos = list_create();
  Map *inventario = map_create(is_equal_int);

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch(opcion){
      case 1:
        cargarPedidosCSV(pedidos);
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      default:
        puts("Opción no válida, intente nuevamente");
    }
    presioneTeclaParaContinuar();
  } while(opcion != 4);
  
  
  return 0;
}