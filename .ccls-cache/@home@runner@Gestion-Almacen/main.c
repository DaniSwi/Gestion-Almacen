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
    int codigo;
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
    int cantidad;
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

Producto *copiarProducto(Producto *producto){
    Producto *new = crearProducto();
    new->precioCosto = producto->precioCosto;
    new->precioVenta = producto->precioVenta;
    strcpy(new->nombre, producto->nombre);
    strcpy(new->tipoProducto, producto->tipoProducto);
    return new;
}

void agregarProductoInventarioUser(Map *inventario, int codigo){
    MapPair *pair = map_search(inventario, &codigo);
    Producto *producto = crearProducto();
    producto->codigo = codigo;
    if(pair){
        List *listaProductos = (List *)pair->value;
        producto = copiarProducto(list_first(listaProductos));
        printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes, &producto->fechaV.year);
        list_pushBack(listaProductos, producto);
        map_insert(inventario, &producto->codigo, listaProductos);
    } else {
        List *listaProductos = list_create();
        printf("Ingrese el nombre del producto: \n");
        scanf(" %[^\n]1000s", producto->nombre);
        printf("Ingrese el precio de costo del producto: ");
        scanf("%d", &producto->precioCosto);
        printf("Ingrese el precio de venta al público del producto: ");
        scanf("%d", &producto->precioVenta);
        printf("Ingrese el tipo de producto: ");
        scanf(" %[^\n]1000s", producto->tipoProducto);
        list_pushBack(listaProductos, producto);
        map_insert(inventario, &producto->codigo, listaProductos);
    }
}

void visualizacionInventario(Map *inventario) {
    MapPair *pair = map_first(inventario);
    if(!pair) {
        printf("No hay productos guardados en el inventario!\n");
        return;
    }
    printf("Inventario:\n");
    while(pair) {
        List *listaProductos = (List *)pair->value;
        Producto *producto = list_first(listaProductos);
        printf("Lista de producto %s que hay en el inventario: \n", producto->nombre);
        while(producto){
            puts("=====================");
            printf("Producto %s con fecha de vencimiento el %d/%d/%d\n", producto->nombre, producto->fechaV.dia, producto->fechaV.mes, producto->fechaV.year);
            puts("=====================");
            producto = (Producto *)list_next(listaProductos);
        }
        pair = map_next(inventario);
    }
}

int actualizarInventario(Map *inventario, int codigo, int cantidad){
    MapPair *pair = map_search(inventario, &codigo);
    if(pair == NULL) {
        printf("\nNo se encuentra el producto en el inventario del almacén\n");
        return 0;
    } else {
        List *listaProductos = (List *)pair->value;
        int cantidadProductos = list_size(listaProductos);
        char nombreProducto[1000];
        strcpy(nombreProducto, pair->key);
        if(cantidadProductos < cantidad) {
            printf("\nNo se cuenta con la cantidad solicitada del producto %s \n", nombreProducto);
            return 0;
        } else {
            int contadorEliminados = 0;
            while(contadorEliminados < cantidad)
                list_popBack(listaProductos);
            printf("Quedan %d unidades del producto %s\n", list_size(listaProductos), nombreProducto);
        }
    }
  return 1;
}

void cargarProductos(Map *precioProductos){
    FILE *archivo = fopen("productos.csv", "r");
    if(archivo == NULL){
        perror("No se pudo abrir el archivo\n");
        return;
    }
    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while((campos = leer_linea_csv(archivo, ',')) != NULL){
        Producto *producto = crearProducto();
        producto->codigo = atoi(campos[0]);
        strcpy(producto->nombre, campos[1]);
        producto->precioVenta = atoi(campos[2]);
        map_insert(precioProductos, &producto->nombre, producto);
    }
}

void cargarPedidosCSV(List *pedidos, Map *inventario, Map *productosPorNombre){ //funcionaaaaaaaa
    FILE *archivo = fopen("data/pedidos.csv", "r");
    if(archivo == NULL){
        perror("No se pudo abrir el archivo\n");
        return;
    }
    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Pedido *pedido = crearPedido();
        pedido->productoPedido.precioVenta = atoi(campos[0]); //Disclaimer, el cliente no sabe cuanto vale el producto, entonces hay que modificar esto :d
        pedido->cantidad = atoi(campos[1]);
        strcpy(pedido->productoPedido.nombre, campos[2]);
        strcpy(pedido->productoPedido.tipoProducto, campos[3]);
        list_pushBack(pedidos, pedido);
        MapPair *pair = map_search(productosPorNombre, pedido->productoPedido.nombre);
    }
    fclose(archivo);
    Pedido *aux = (Pedido *)list_first(pedidos);
    while(aux){
        printf("Nombre producto: %s", aux->productoPedido.nombre);
        printf("Precio: %d", aux->productoPedido.precioVenta);
        printf("Cantidad: %d", aux->cantidad);
        printf("Tipo: %s", aux->productoPedido.tipoProducto);
        actualizarInventario(inventario, , int cantidad)
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
    Map *inventario = map_create(is_equal_int);
    Map *precioProductos = map_create(is_equal_str);
    cargarProductos(precioProductos);
    int codigoProducto;
    Heap *productosVencimiento = heap_create();
    char car = 'S';

    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                while(car == 'S') {
                    printf("Ingrese el código del producto: ");
                    scanf("%d", &codigoProducto);
                    agregarProductoInventarioUser(inventario, codigoProducto);
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