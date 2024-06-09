#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include "tdas/heap.h"
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
} Producto;

typedef struct{
    char rut[15];
    int deuda;
    int edad;
} Cliente;

typedef struct{
    int monto;
    Producto productoPedido;
    Cliente cliente;
} Pedido;

Pedido *crearPedido(){
    Pedido *pedido = (Pedido *)malloc(sizeof(Pedido));
    return pedido;
}

Producto *crearProducto(){
    Producto *producto = (Producto *)malloc(sizeof(Producto));
    return producto;
}

void agregarProductoInventarioUser(Map *inventario, char *nombreProducto){
    MapPair *pair = map_search(inventario, nombreProducto);
    Producto *producto;
    if(pair){
        producto = (Producto *)pair->value;
        ++producto->cantidad;
    } else {
        producto = crearProducto();
        strcpy(producto->nombre, nombreProducto);
        printf("Ingrese el precio de costo del producto: ");
        scanf("%d", &producto->precioCosto);
        printf("Ingrese el precio de venta del producto: ");
        scanf("%d", &producto->precioVenta);
        printf("Ingrese el tipo de producto: ");
        scanf(" %[^\n]1000s", producto->tipoProducto);
        producto->cantidad = 1;
    }
    printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes, &producto->fechaV.year);
    map_insert(inventario, producto->nombre, producto);
}

void visualizacionInventario(Map *inventario){
    printf("Inventario:\n");
    MapPair *pair = map_first(inventario);
    while(pair){
        Producto *producto = pair->value;
        puts("=====================");
        printf("Nombre: %s\n", producto->nombre);
        printf("Tipo: %s\n", producto->tipoProducto);
        printf("Cantidad: %d\n", producto->cantidad);
        pair = map_next(inventario);
    }
}

int actualizarInventario(Map *inventario, char *nombreProducto, int cantidad){
    MapPair *pair = map_search(inventario, nombreProducto);
    if(pair == NULL) {
        printf("\nNo se encuentra el producto %s en el inventario del almacén\n", nombreProducto);
      return 0;
    } else {
        Producto *producto = (Producto *)pair->value;
        if(producto->cantidad < cantidad) {
            printf("\nNo se cuenta con la cantidad solicitada del producto %s \n", nombreProducto);
            return 0;
        } else {
            producto->cantidad -= cantidad;
        }
    }
  return 1;
}


void cargarPedidosCSV(List *pedidos, Map *inventario){ //funcionaaaaaaaa
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
        printf("Nombre producto: %s", aux->productoPedido.nombre);
        printf("Precio: %d", aux->productoPedido.precioVenta);
        printf("Cantidad: %d", aux->productoPedido.cantidad);
        printf("Tipo: %s", aux->productoPedido.tipoProducto);
        if(actualizarInventario(inventario, aux->productoPedido.nombre, aux->productoPedido.cantidad)) {
            printf("Se puede entregar el producto con las cantidades indicadas\n");
        } else {
            printf("No se puede entregar el producto con las cantidades indicadas\n");
        }
        aux = (Pedido *)list_next(pedidos);
    }
}

void agregarProductoAuto(Map *inventario, Producto *producto){
    MapPair *pair = map_search(inventario, producto->nombre);
    if(pair){
        Producto *productoInventario = (Producto *)pair->value;
        productoInventario->cantidad += producto->cantidad;
    } else 
        map_insert(inventario, producto->nombre, producto);
}

void cargarCargaDesdeCSV(Map *inventario){
    FILE *archico = fopen("data/carga.csv", "r");
    if(archico == NULL){
        perror("No se pudo abrir el archivo\n");
        return;
    }
    char **campos;
    campos = leer_linea_csv(archico, ',');
    while ((campos = leer_linea_csv(archico, ',')) != NULL){
        Producto *producto = crearProducto();
        strcpy(producto->nombre, campos[0]);
        producto->precioCosto = atoi(campos[1]);
        producto->precioVenta = atoi(campos[2]);
        producto->cantidad = atoi(campos[3]);
        strcpy(producto->tipoProducto, campos[4]);
        producto->fechaV.dia = atoi(campos[5]);
        producto->fechaV.mes = atoi(campos[6]);
        producto->fechaV.year = atoi(campos[7]);
        agregarProductoAuto(inventario, producto);
    }
}

void pagar(Pedido *pedido){
  printf("El cliente debe pagar: %d\n", pedido->monto);
  printf("Ingrese el monto que le pagó el cliente: ");
  int montoPagado;
  scanf("%d", &montoPagado);
  if(montoPagado < pedido->monto){
    printf("El cliente queda con una deuda de $ %d\n", pedido->monto - montoPagado);
    pedido->cliente.deuda = pedido->monto - montoPagado;
  } else {
    int vuelto = montoPagado - pedido->monto;
    printf("El vuelto es de: %d\n", vuelto);
  }
}


//tarjeta / efectivo 1500 -> 2000
//tarjeta: debito o credito (cuasi innecesario)




int main() {

    int opcion;
    List *pedidos = list_create();
    Map *inventario = map_create(is_equal_str);
    char nombreProducto[10000];
    Heap *productosVencimiento = heap_create();
    char car = 'S';

    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                while(car == 'S') {
                    printf("Ingrese el nombre del producto: ");
                    scanf(" %[^\n]10000s", nombreProducto);
                    agregarProductoInventarioUser(inventario, nombreProducto);
                    printf("¿Desea agregar otro producto? (S/N): ");
                    scanf(" %c", &car);
                } 
                printf("\nVolviendo al menú principal\n");
                break;
            case 2:
                cargarPedidosCSV(pedidos, inventario); // Cargar un pedido para un cliente
                break;
            case 3:
                visualizacionInventario(inventario); // Ver inventario
                break;
            case 4:
                cargarCargaDesdeCSV(inventario);
                break;
            case 5:
                //exportarPedidos(pedidos);
                break;
            default:
                puts("Opción no válida, intente nuevamente");
        }
        presioneTeclaParaContinuar();
    } while(opcion != 9);
    
    
    return 0;
}