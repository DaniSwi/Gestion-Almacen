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

void agregarProductoInventarioUser(Map *inventario){
    char car = 'n';
    do {
        int codigo;
        printf("Ingrese el código del producto: ");
        scanf("%d", &codigo);
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
            printf("Ingrese el nombre del producto: ");
            scanf(" %[^\n]1000s", producto->nombre);
            printf("Ingrese el precio de costo del producto: ");
            scanf("%d", &producto->precioCosto);
            printf("Ingrese el precio de venta al público del producto: ");
            scanf("%d", &producto->precioVenta);
            printf("Ingrese el tipo de producto: ");
            scanf(" %[^\n]1000s", producto->tipoProducto);
            printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes, &producto->fechaV.year);
            list_pushBack(listaProductos, producto);
            map_insert(inventario, &producto->codigo, listaProductos);
        }
        printf("Desea agregar otro producto al inventario? (s/n): ");
        char car;
        scanf(" %c", &car);
    } while(car != 'n');
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

void actualizarInventario(Map *inventario, int codigo, int cantidad){
    MapPair *pair = map_search(inventario, &codigo);
    if(pair == NULL) 
        printf("\nNo se encuentra el producto en el inventario del almacén\n");
    else {
        List *listaProductos = (List *)pair->value;
        int cantidadProductos = list_size(listaProductos);
        char nombreProducto[1000];
        strcpy(nombreProducto, pair->key);
        if(cantidadProductos < cantidad) 
            printf("\nNo se cuenta con la cantidad solicitada del producto %s \n", nombreProducto);
        else {
            int contadorEliminados = 0;
            while(contadorEliminados < cantidad)
                list_popBack(listaProductos);
            printf("Se ha actualizado correctamente, quedan %d unidades del producto %s\n", list_size(listaProductos), nombreProducto);
        }
    }
}

void cargarProductos(Map *precioProductos){
    FILE *archivo = fopen("data/productos.csv", "r");
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

void cargarPedidosCSV(List *pedidos, Map *inventario, Map *productosPrecio){ // El tercer mapa es el que guarda los productos por su nombre (key) y guarda su codigo y precio y nombre
    FILE *archivo = fopen("data/pedidos.csv", "r");
    if(archivo == NULL){
        perror("No se pudo abrir el archivo\n");
        return;
    }
    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Pedido *pedido = crearPedido();
        pedido->cantidad = atoi(campos[0]);
        strcpy(pedido->productoPedido.nombre, campos[1]);
        MapPair *pair = map_search(productosPrecio, pedido->productoPedido.nombre);
        if(pair){
            Producto *producto = (Producto *)pair->value;
            pedido->productoPedido.precioVenta = producto->precioVenta;
            pedido->productoPedido.codigo = producto->codigo;
            list_pushBack(pedidos, pedido);
        } else 
            printf("No se encuentra el producto %s en el inventario, por lo que no se agregrá a la lista de pedidos\n", pedido->productoPedido.nombre);
    }
    fclose(archivo);
    Pedido *aux = (Pedido *)list_first(pedidos);
    while(aux){
        printf("Nombre producto: %s", aux->productoPedido.nombre);
        printf("Precio: %d", aux->productoPedido.precioVenta);
        printf("Cantidad: %d", aux->cantidad);
        printf("Tipo: %s", aux->productoPedido.tipoProducto);
        actualizarInventario(inventario, aux->productoPedido.codigo, aux->cantidad);
        aux = (Pedido *)list_next(pedidos);
    }
}

void agregarProductoAuto(Map *inventario, Producto *producto){
    MapPair *pair = map_search(inventario, &producto->codigo);
    if(pair){
        List *listaProductos = (List *)pair->value;
        list_pushBack(listaProductos, producto);
        map_insert(inventario, &producto->codigo, listaProductos);
    } else {
        List *listaProductos = list_create();
        list_pushBack(listaProductos, producto);
        map_insert(inventario, &producto->codigo, listaProductos);
    }
}

void cargarCargaDesdeCSV(Map *inventario, Map *precioProductos){
    FILE *archivo = fopen("data/carga.csv", "r");
    if(archivo == NULL){
        perror("No se pudo abrir el archivo\n");
        return;
    }
    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while((campos = leer_linea_csv(archivo, ',')) != NULL){
        Producto *producto = crearProducto();
        strcpy(producto->nombre, campos[0]);
        producto->precioCosto = atoi(campos[1]);
        producto->precioVenta = atoi(campos[2]);
        strcpy(producto->tipoProducto, campos[3]);
        producto->fechaV.dia = atoi(campos[4]);
        producto->fechaV.mes = atoi(campos[5]);
        producto->fechaV.year = atoi(campos[6]);
        MapPair *pair = map_search(precioProductos, &producto->nombre);
        if(pair){
            Producto *new = (Producto *)pair->value;
            producto->codigo = new->codigo;
            agregarProductoAuto(inventario, producto);
        } else 
            printf("El producto %s no existe en los productos que se venden\n", producto->nombre);
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

void mostrarProductos(Map *productos){
    MapPair *pair = map_first(productos);
    while(pair){
        Producto *producto = (Producto *)pair->value;
        printf("Código: %d\n", producto->codigo);
        printf("Nombre: %s\n", producto->nombre);
        printf("Precio de costo: %d\n", producto->precioVenta);
        pair = map_next(productos);
    }
}

void cuadrarInventario(Map *inventario, Map *productosPrecio){
    MapPair *pair = map_first(inventario);
    while(pair){
        List *listaProductos = (List *)pair->value;
        Producto *producto = list_first(listaProductos);
        MapPair *par = map_search(productosPrecio, &producto->nombre);
        if(par==NULL){
            printf("El producto %s no existe en los productos que se venden, será eliminado\n", producto->nombre);
            map_remove(inventario, listaProductos);
        }
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

    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                agregarProductoInventarioUser(inventario);
                break;
            case 2:
                cargarPedidosCSV(pedidos, inventario, precioProductos); // Cargar un pedido para un cliente
                break;
            case 3:
                visualizacionInventario(inventario); // Ver inventario
                break;
            case 4:
                cargarCargaDesdeCSV(inventario, precioProductos);
                break;
            case 5:
                //exportarPedidos(pedidos);
                break;
            case 6:
                mostrarProductos(precioProductos);
                break;
            default:
                puts("Opción no válida, intente nuevamente");
        }
        presioneTeclaParaContinuar();
    } while(opcion != 9);
    
    map_clean(inventario);
    map_clean(precioProductos);
    
    return 0;
}