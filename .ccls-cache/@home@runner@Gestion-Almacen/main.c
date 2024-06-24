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

/*typedef struct{
    char rut[15];
    int deuda;
    int edad;
} Cliente;*/

typedef struct{
    int monto;
    int cantidad;
    Producto producto;
    //Cliente cliente;
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

void formatearCadena(char *cadena){
    for(int i=0;cadena[i]!='\0';++i) {
        if(isalpha(cadena[i])) {
            cadena[i] = toupper(cadena[i]);
        }
    }
}

void leerCodigoBarra() {
    char codigoBarra[100];
    printf("Ingrese el codigo de barra: ");
    scanf("%s", codigoBarra);
    printf("Código de barra: %s\n", codigoBarra);
}

void agregarProductoInventarioUser(Map *inventario){
    char car = 'n';
    do {
        printf("=================================\n");
        printf("       Agregar producto.\n");
        printf("=================================\n");
        int codigo;
        printf("Ingrese el código del producto: ");
        scanf("%d", &codigo);
        MapPair *pair = map_search(inventario, &codigo); // O(1)
        Producto *producto = crearProducto();
        producto->codigo = codigo;
        if(pair){
            List *listaProductos = (List *)pair->value;
            producto = copiarProducto(list_first(listaProductos));
            printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes, &producto->fechaV.year);
            list_pushBack(listaProductos, producto);
        } else {
            List *listaProductos = list_create();
            printf("Ingrese el nombre del producto: ");
            scanf(" %[^\n]1000s", producto->nombre);
            getchar();
            formatearCadena(producto->nombre);
            printf("Ingrese el precio de costo del producto: ");
            scanf("%d", &producto->precioCosto);
            printf("Ingrese el precio de venta al público del producto: ");
            scanf("%d", &producto->precioVenta);
            printf("Ingrese el tipo de producto: ");
            formatearCadena(producto->tipoProducto);
            scanf(" %[^\n]1000s", producto->tipoProducto);
            getchar();
            printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes, &producto->fechaV.year);
            list_pushBack(listaProductos, producto);
            map_insert(inventario, &producto->codigo, listaProductos);
        }
        printf("Desea agregar otro producto al inventario? (s/n): ");
        scanf(" %c", &car);
    } while(car != 'n');
}

void visualizacionInventario(Map *inventario) {
    printf("====================================\n");
    printf("             Inventario\n");
    printf("====================================\n");
    MapPair *pair = map_first(inventario);
    if(!pair) {
        printf("No hay productos guardados en el inventario!\n");
        return;
    }
    while(pair) {
        List *listaProductos = (List *)pair->value;
        Producto *producto = list_first(listaProductos);
        printf("\nLista de productos %s que hay en el inventario: \n", producto->nombre);
        while(producto){
            puts("==================================================================\n");
            printf("Producto %s con fecha de vencimiento el %d/%d/%d\n", producto->nombre, producto->fechaV.dia, producto->fechaV.mes, producto->fechaV.year);
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
            printf("\nSe ha actualizado correctamente, quedan %d unidades del producto %s\n", list_size(listaProductos), nombreProducto);
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
        formatearCadena(producto->nombre);
        producto->precioVenta = atoi(campos[2]);
        map_insert(precioProductos, strdup(producto->nombre), producto);
    }
}

void cargarPedidosCSV(List *pedidos, Map *inventario, Map *productosPrecio) { // El tercer mapa es el que guarda los productos por su nombre (key) y guarda su codigo y precio y nombre
    printf("===============================\n");
    printf("        Cargar pedidos\n");
    printf("===============================\n");
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
        //char *aux = strtok(campos[1], "\n");
        strcpy(pedido->producto.nombre, campos[1]);
        formatearCadena(pedido->producto.nombre);
        MapPair *pair = map_search(productosPrecio, pedido->producto.nombre);
        if(pair) {
            Producto *producto = (Producto *)pair->value;
            pedido->producto.precioVenta = producto->precioVenta;
            pedido->producto.codigo = producto->codigo;
            pedido->monto = pedido->producto.precioVenta * pedido->cantidad;
            list_pushBack(pedidos, pedido);
        } else 
            printf("No se encuentra el producto %s en el inventario, por lo que no se agregará a la lista de pedidos\n", pedido->producto.nombre);
    }
    fclose(archivo);
    Pedido *aux = (Pedido *)list_first(pedidos);
    while(aux){
        printf("==========================================\n");
        printf("Nombre producto: %s\n", aux->producto.nombre);
        printf("Precio: %d\n", aux->producto.precioVenta);
        printf("Cantidad: %d\n", aux->cantidad);
        printf("Tipo: %s\n", aux->producto.tipoProducto);
        actualizarInventario(inventario, aux->producto.codigo, aux->cantidad);
        aux = (Pedido *)list_next(pedidos);
    }
}

void agregarProductoAuto(Map *inventario, Producto *producto){
    MapPair *pair = map_search(inventario, &producto->codigo);
    if(pair){
        List *listaProductos = (List *)pair->value;
        list_pushBack(listaProductos, producto);
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
        formatearCadena(producto->nombre);
        producto->precioCosto = atoi(campos[1]);
        producto->precioVenta = atoi(campos[2]);
        strcpy(producto->tipoProducto, campos[3]);
        formatearCadena(producto->tipoProducto);
        producto->fechaV.dia = atoi(campos[4]);
        producto->fechaV.mes = atoi(campos[5]);
        producto->fechaV.year = atoi(campos[6]);
        MapPair *pair = map_search(precioProductos, producto->nombre);
        if(pair){
            Producto *new = (Producto *)pair->value;
            producto->codigo = new->codigo;
            agregarProductoAuto(inventario, producto);
        } else 
            printf("\nEl producto %s no existe en los productos que se venden\n", producto->nombre);
    }
}

/*void pagar(Pedido *pedido){
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
}*/

void mostrarProductos(Map *productos){
    printf("\nA continuación se mostrarán todos los productos que se venden en el almacén\n");
    MapPair *pair = map_first(productos);
    while(pair){
        Producto *producto = (Producto *)pair->value;
        printf("\n==============================\n");
        printf("Código: %d\n", producto->codigo);
        printf("Nombre: %s\n", producto->nombre);
        printf("Precio de costo: %d\n", producto->precioVenta);
        printf("==============================\n");
        pair = map_next(productos);
    }
}

void cuadrarInventario(Map *inventario, Map *productosPrecio) {
    MapPair *pair = map_first(inventario);
    if(!pair) {
        printf("No hay elementos en el inventario!\n");
        return;
    }
    while(pair) {
        List *listaProductos = (List *)pair->value;
        Producto *producto = (Producto *)list_first(listaProductos);
        if(producto == NULL) {
            pair = map_next(inventario);
            continue;
        }
        MapPair *nextPair = map_next(inventario);
        MapPair *par = map_search(productosPrecio, producto->nombre);
        if(par == NULL) {
            printf("El producto %s no existe en los productos que se venden, será eliminado del inventario\n", producto->nombre);
            list_clean(pair->value);
            map_remove(inventario, &producto->codigo);
        }
        pair = nextPair; 
    }
}

void exportarInventario(Map *inventario){
    FILE *archivo = fopen("data/inventarioOutput.csv", "w");
    if(archivo == NULL){
        perror("No se pudo abrir el archivo\n");
        return;
    }
    for(MapPair *pair=map_first(inventario);pair!=NULL;pair=map_next(inventario)){
        List *listProductos = (List *)pair->value;
        Producto *producto = (Producto *)list_first(listProductos);
        while(producto){
            fprintf(archivo, "%d,%s,%d,%d,%d,%d,%d,%s\n",producto->codigo, producto->nombre, producto->precioCosto, producto->precioVenta, producto->fechaV.dia, producto->fechaV.mes, producto->fechaV.year, producto->tipoProducto);
            producto = (Producto *)list_next(listProductos);
        }
    }
    printf("\nSe ha exportado correctamente el inventario al archivo inventarioOutput.csv\n"); 
    fclose(archivo);
}

void buscarProductoPorCodigo(Map *inventario){
    int codigo;
  
    printf("=========================================\n");
    printf("  Busqueda de producto por codigo\n");
    printf("=========================================\n");
    printf("\nIngrese el código del producto : ");
    scanf("%d", &codigo);
    
    MapPair *pair = map_search(inventario, &codigo);
    if(pair){
        List *listaProductos = (List *)pair->value;
        Producto *producto = (Producto *)list_first(listaProductos);
        printf("\nEl producto %s existe en el inventario\n", producto->nombre);
    } else 
        printf("El producto con código %d no existe en el inventario\n", codigo);
}

void actualizarProductosPorTipo(Map *productosPorTipo, Map *inventario){ 
    MapPair *pair = map_first(inventario);
    while(pair){
        List *listaProductos = (List *)pair->value;
        Producto *producto = (Producto *)list_first(listaProductos);
        MapPair *par = map_search(productosPorTipo, producto->tipoProducto);
        if(!par) {
            List *listaProductosTipo = list_create();
            list_pushBack(listaProductosTipo, producto);
            map_insert(productosPorTipo, strdup(producto->tipoProducto), listaProductosTipo);
        }
        pair = map_next(inventario);
    }
}

void buscarProductosPorTipo(Map *productosPorTipo) {
    char tipo[1000];
    printf("\nIngrese el tipo de producto a buscar: ");
    scanf(" %[^\n]1000s", tipo);
    formatearCadena(tipo);
    MapPair *pair = map_search(productosPorTipo, tipo);
    if(pair){
        List *listaProductos = (List *)pair->value;
        Producto *producto = (Producto *)list_first(listaProductos);
        printf("\nProductos de tipo %s:\n", tipo);
        while(producto){
            printf("\n==============================\n");
            printf("Nombre: %s\n", producto->nombre);
            printf("Precio de venta: %d\n", producto->precioVenta);
            printf("================================\n");
            producto = (Producto *)list_next(listaProductos);
        }
    } else {
        printf("\nNo se encuentran productos del tipo %s\n", tipo);
    }
}

void mostrarPedido(List *pedidos)
{
    limpiarPantalla();
    printf("Producto(s) en el pedido:\n\n");
    int sumaPrecio = 0;
    Pedido *aux = list_first(pedidos);
    while(aux) {
        printf(" - %s (%d)\n", aux->producto.nombre, aux->producto.precioVenta);
        sumaPrecio += aux->producto.precioVenta;
        aux = list_next(pedidos);
    }
    printf("\nCosto total: %d$\n", sumaPrecio);
}

void agregarProductoPedido(void) //List *pedidos, Map *inventario
{
    
}

void eliminarProductoPedido(void) //List *pedidos, Map *inventario
{
    
}

void modificarPedido(List *pedidos, Map *inventario, unsigned short ctrl)
{
    char op;
    do {
        limpiarPantalla();
        printf("=====================================\n");
        printf("   Menú de modificación del pedido   \n");
        printf("=====================================\n");
        if(ctrl == 0) {
            printf("1) Agregar producto al pedido\n");
            printf("0) Volver al menú de gestión de pedido\n");
        } else {
            printf("1) Agregar producto al pedido\n");
            printf("2) Eliminar producto del pedido\n");
            printf("0) Volver al menú de gestión de pedido\n");
        }
        scanf(" %c", &op);
        if(op == '1') {
            agregarProductoPedido();
        } else if(ctrl == 1 && op == '2') {
            eliminarProductoPedido();
        } else if(op == '0') {
            printf("Volviendo al menú de gestión de pedido\n");
        } else {
            printf("Opción no valida, intente de nuevo.\n");
        }
        presioneTeclaParaContinuar();
    } while(op != '0'); 
    
    
} 

void procesarPago(List *pedidos, Map *inventario)
{
    printf("3\n");
}

void mostrarMenuPedidoVacio() 
{
  limpiarPantalla();
  printf("Actualmente no hay datos de pedido\n");
  printf("Se pueden agregar productos manualmente o utilizando la 2da función del menú principal\n");
  printf("1) Agregar productos manualmente\n");
  printf("0) Volver al menú principal\n");
}

void mostrarMenuPedido()
{
  limpiarPantalla();
  printf("Actualmente hay datos de un pedido en el sistema\n");
  printf("1) Ver datos del pedido actual\n");
  printf("2) Modificar datos del pedido actual\n");
  printf("3) Procesar compra\n");
  printf("0) Volver al menú principal\n");
}

void procesarVenta(List *pedidos, Map *inventario) //Hermano dejate de wear
{
    int pedidoSize;
    char op;
    do {
        pedidoSize = list_size(pedidos);
        if(pedidoSize == 0) {
            mostrarMenuPedidoVacio();
            scanf(" %c", &op);
            switch(op) {
                case '1':
                    modificarPedido(pedidos, inventario, 0);
                    break;
                case '0':
                    printf("Volviendo al menú principal\n");
                    break;
                default:
                    printf("Opción no valida, intente de nuevo.\n");
                    presioneTeclaParaContinuar();
            }
        } else {
            mostrarMenuPedido();
            scanf(" %c", &op);
            switch(op) {
                case '1':
                    mostrarPedido(pedidos);
                    presioneTeclaParaContinuar();
                    break;
                case '2':
                    modificarPedido(pedidos, inventario, 1);
                    break;
                case '3':
                    procesarPago(pedidos, inventario);
                    return;
                case '0':
                    printf("Volviendo al menú principal\n"); 
                    break; 
                default: 
                    printf("Opción no valida, intente de nuevo.\n");
                    presioneTeclaParaContinuar();
            }
        }
    } while(op != '0');
    return;
}

void cargarInventario(Map *inventario){
    FILE *archivo = fopen("data/inventario.csv", "r");
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
        formatearCadena(producto->nombre);
        producto->precioVenta = atoi(campos[2]);
        producto->fechaV.dia = atoi(campos[3]);
        producto->fechaV.mes = atoi(campos[4]);
        producto->fechaV.year = atoi(campos[5]);
        MapPair *pair = map_search(inventario, &producto->codigo);
        if(pair){
            List *listaProductos = (List *)pair->value;
            list_pushBack(listaProductos, producto);
        } else {
            List *listaProductos = list_create();
            list_pushBack(listaProductos, producto);
            map_insert(inventario, &producto->codigo, listaProductos); //recordar que la key del mapa del inventario es el codigo del producto
        } 
    }
}

int main(int argc, char *argv[]) {

    int opcion;
    List *pedidos = list_create();
    Map *inventario = map_create(is_equal_int);
    Map *precioProductos = map_create(is_equal_str);
    Map *productosPorTipo = map_create(is_equal_str);
    cargarProductos(precioProductos);
    cargarInventario(inventario);
    
    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        switch(opcion) {
            case 1:
                limpiarPantalla();
                agregarProductoInventarioUser(inventario);
                break;
            case 2:
                limpiarPantalla();
                cargarPedidosCSV(pedidos, inventario, precioProductos); // Cargar un pedido para un cliente
                break;
            case 3:
                limpiarPantalla();
                visualizacionInventario(inventario); // Ver inventario
                break;
            case 4:
                limpiarPantalla();
                cargarCargaDesdeCSV(inventario, precioProductos);
                break;
            case 5:
                limpiarPantalla();
                exportarInventario(inventario);
                break;
            case 6:
                limpiarPantalla();
                mostrarProductos(precioProductos);
                break;
            case 7:
                limpiarPantalla();
                cuadrarInventario(inventario, precioProductos);
                break;
            case 8:
                limpiarPantalla();
                buscarProductoPorCodigo(inventario);
                break;
            case 9:
                limpiarPantalla();
                actualizarProductosPorTipo(productosPorTipo, inventario);
                buscarProductosPorTipo(productosPorTipo);
                break;
            case 10:
                procesarVenta(pedidos, inventario);
                break;
            case 11:
                printf("Saliendo del menú principal. . .\n");
                break;
            default:
                puts("Opción no válida, intente nuevamente");
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();
    } while(opcion != 11);
    printf("Programa terminado correctamente :]");
    map_clean(inventario);
    map_clean(precioProductos);
    map_clean(productosPorTipo);
    list_clean(pedidos);
    
    return 0;
}