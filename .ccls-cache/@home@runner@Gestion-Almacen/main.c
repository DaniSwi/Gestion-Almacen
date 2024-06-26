#include "tdas/extra.h"
#include "tdas/heap.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ELIMINAR ULTIMO CAMBIO ---> git reset --hard HEAD~1 (vengan a la puta
// ayudantia cabrones)

typedef struct {
  int dia;
  int mes;
  int year;
} fechaVencimiento; 

typedef struct {
  int precioCosto;
  int precioVenta;
  int codigo;
  char nombre[10000];
  char tipoProducto[1000];
  fechaVencimiento fechaV;
} Producto;

typedef struct {
  int monto;
  int cantidad;
  Producto producto;
} Pedido;

Pedido *crearPedido() {
  Pedido *pedido = (Pedido *)malloc(sizeof(Pedido));
  return pedido;
}

Producto *crearProducto() {
  Producto *producto = (Producto *)malloc(sizeof(Producto));
  return producto;
}

Producto *copiarProducto(Producto *producto) {
  Producto *new = crearProducto();
  new->precioCosto = producto->precioCosto;
  new->precioVenta = producto->precioVenta;
  new->codigo = producto->codigo;
  strcpy(new->nombre, producto->nombre);
  strcpy(new->tipoProducto, producto->tipoProducto);
  return new;
}

void formatearCadena(char *cadena) {
  for (int i = 0; cadena[i] != '\0'; ++i) {
    if (isalpha(cadena[i])) {
      cadena[i] = toupper(cadena[i]);
    } else if (cadena[i] == '\n') {
      cadena[i] = '\0';
    }
  }
}

void leerCodigoBarra() {
  char codigoBarra[100];
  printf("Ingrese el codigo de barra: ");
  scanf("%s", codigoBarra);
  printf("Código de barra: %s\n", codigoBarra);
}

void agregarProductoInventarioUser(Map *inventario) {
  char car;
  do {
    limpiarPantalla();
    printf("=================================\n");
    printf("       Agregar producto.\n");
    printf("=================================\n");
    int codigo;
    printf("Ingrese el código del producto: ");
    scanf("%d", &codigo);
    MapPair *pair = map_search(inventario, &codigo); // O(1)
    Producto *producto = crearProducto();
    producto->codigo = codigo;
    if (pair) {
      List *listaProductos = (List *)pair->value;
      producto = copiarProducto(list_first(listaProductos));
      printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
      scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes,
            &producto->fechaV.year);
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
      scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes,
            &producto->fechaV.year);
      list_pushBack(listaProductos, producto);
      map_insert(inventario, &producto->codigo, listaProductos);
    }
    printf("Presione cualquier tecla para agregar otro producto, para volver al menú presione n: \n");
    scanf(" %c", &car);
  } while(car != 'n');
}

void visualizacionInventario(Map *inventario) {
  printf("====================================\n");
  printf("             Inventario\n");
  printf("====================================\n");
  MapPair *pair = map_first(inventario);
  if (!pair) {
    printf("No hay productos guardados en el inventario!\n");
    return;
  }
  while (pair) {
    List *listaProductos = (List *)pair->value;
    Producto *producto = list_first(listaProductos);
    printf("\n================================================================="
           "======\n");
    printf("\nLista de productos %s que hay en el inventario: \n",
           producto->nombre);
    while (producto) {
      printf("  * Producto %s con fecha de vencimiento el %d/%d/%d\n",
             producto->nombre, producto->fechaV.dia, producto->fechaV.mes,
             producto->fechaV.year);
      producto = (Producto *)list_next(listaProductos);
    }
    pair = map_next(inventario);
  }
  printf("\n=======================================================================\n");
}

void actualizarInventario(Map *inventario, int codigo, int cantidad) {
  MapPair *pair = map_search(inventario, &codigo);
  if (pair == NULL)
    printf("\nNo se encuentra el producto en el inventario del almacén\n");
  else {
    List *listaProductos = (List *)pair->value;
    Producto *producto = (Producto *)list_first(listaProductos);
    int cantidadProductos = list_size(listaProductos);
    char nombreProducto[1000];
    strcpy(nombreProducto, producto->nombre);
    if(cantidadProductos == 0)
      printf("No quedan unidades del producto!\n");
    else if(cantidadProductos < cantidad)
      printf("\nNo se cuenta con la cantidad solicitada del producto %s", nombreProducto);
    else {
      int contadorEliminados = 0;
      while (contadorEliminados < cantidad) {
        list_popBack(listaProductos);
        ++contadorEliminados;
      }
      printf("\nSe ha actualizado correctamente, quedan %d unidades del producto %s\n", list_size(listaProductos), nombreProducto);
    }
  }
}

void cargarProductos(Map *precioProductos) {
  FILE *archivo = fopen("data/productos.csv", "r");
  if (archivo == NULL) {
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ',');
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Producto *producto = crearProducto();
    producto->codigo = atoi(campos[0]);
    strcpy(producto->nombre, campos[1]);
    formatearCadena(producto->nombre);
    producto->precioVenta = atoi(campos[2]);
    strcpy(producto->tipoProducto, campos[3]);
    formatearCadena(producto->tipoProducto);
    map_insert(precioProductos, strdup(producto->nombre), producto);
  }
}

void cargarPedidosCSV(List *pedidos, Map *inventario, Map *productosPrecio) { // El tercer mapa es el que guarda los productos por
                            // su nombre (key) y guarda su codigo y precio y
                            // nombre
  printf("===============================\n");
  printf("        Cargar pedidos\n");
  printf("===============================\n");
  FILE *archivo = fopen("data/pedidos.csv", "r");
  if (archivo == NULL) {
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ',');
  list_clean(pedidos);
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Pedido *pedido = crearPedido();
    pedido->cantidad = atoi(campos[0]);
    strcpy(pedido->producto.nombre, campos[1]);
    formatearCadena(pedido->producto.nombre);
    MapPair *pair = map_search(productosPrecio, pedido->producto.nombre);
    if (pair) {
      Producto *producto = (Producto *)pair->value;
      pedido->producto.precioVenta = producto->precioVenta;
      pedido->producto.codigo = producto->codigo;
      strcpy(pedido->producto.tipoProducto, producto->tipoProducto);
      pedido->monto = pedido->producto.precioVenta * pedido->cantidad;
      list_pushBack(pedidos, pedido);
    }
  }
  fclose(archivo);
  Pedido *aux = (Pedido *)list_first(pedidos);
  while (aux) {
    printf("\n==========================================\n");
    printf("\nNombre producto: %s\n", aux->producto.nombre);
    printf("Precio: %d\n", aux->producto.precioVenta);
    printf("Cantidad: %d\n", aux->cantidad);
    printf("Tipo: %s\n", aux->producto.tipoProducto);
    actualizarInventario(inventario, aux->producto.codigo, aux->cantidad);
    aux = (Pedido *)list_next(pedidos);
  }
  printf("\n==========================================\n");
}

void agregarProductoAuto(Map *inventario, Producto *producto) {
  MapPair *pair = map_search(inventario, &producto->codigo);
  if (pair) {
    List *listaProductos = (List *)pair->value;
    list_pushBack(listaProductos, producto);
  } else {
    List *listaProductos = list_create();
    list_pushBack(listaProductos, producto);
    map_insert(inventario, &producto->codigo, listaProductos);
  }
}

void cargarCargaDesdeCSV(Map *inventario, Map *precioProductos) {
  FILE *archivo = fopen("data/carga.csv", "r");
  if (archivo == NULL) {
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ',');
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
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
    if (pair) {
      Producto *new = (Producto *)pair->value;
      producto->codigo = new->codigo;
      agregarProductoAuto(inventario, producto);
    } else
      printf("\nEl producto %s no existe en los productos que se venden\n",
             producto->nombre);
  }
  fclose(archivo);
  printf("\nSe ha cargado exitosamente!\n");
}

void mostrarProductos(Map *productos) {
  printf("================================================\n");
  printf("           Productos a la venta\n");
  printf("================================================\n\n");
  MapPair *pair = map_first(productos);
  while (pair) {
    Producto *producto = (Producto *)pair->value;
    printf("================================================\n\n");
    printf("Código: %d\n", producto->codigo);
    printf("Nombre: %s\n", producto->nombre);
    printf("Precio de costo: %d\n\n", producto->precioVenta);
    pair = map_next(productos);
  }
  printf("================================================\n");
}

void cuadrarInventario(Map *inventario, Map *productosPrecio) {
  int bool = 0;
  printf("================================================\n");
  printf("              Cuadrar Inventario\n");
  printf("================================================\n\n");
  MapPair *pair = map_first(inventario);
  if (!pair) {
    printf("No hay elementos en el inventario!\n");
    return;
  }
  while (pair) {
    List *listaProductos = (List *)pair->value;
    Producto *producto = (Producto *)list_first(listaProductos);
    if (producto == NULL) {
      pair = map_next(inventario);
      continue;
    }
    MapPair *nextPair = map_next(inventario);
    MapPair *par = map_search(productosPrecio, producto->nombre);
    if (par == NULL) {
      printf("El producto %s no existe en los productos que se venden, será "
             "eliminado del inventario\n",
             producto->nombre);
      list_clean(pair->value);
      map_remove(inventario, &producto->codigo);
      ++bool;
    }
    pair = nextPair;
  }
  if(!bool)
    printf("      El inventario estaba cuadrado!\n");
  printf("\n================================================\n\n");
}

void exportarInventario(Map *inventario) {
  printf("==========================================\n");
  printf("       Exportacion del inventario\n");
  printf("==========================================\n");
  FILE *archivo = fopen("data/inventarioOutput.csv", "w");
  if (archivo == NULL) {
    perror("No se pudo abrir el archivo\n");
    return;
  }
  for (MapPair *pair = map_first(inventario); pair != NULL;
       pair = map_next(inventario)) {
    List *listProductos = (List *)pair->value;
    Producto *producto = (Producto *)list_first(listProductos);
    while (producto) {
      fprintf(archivo, "%d,%s,%d,%d,%d,%d,%d,%s\n", producto->codigo,
              producto->nombre, producto->precioCosto, producto->precioVenta,
              producto->fechaV.dia, producto->fechaV.mes, producto->fechaV.year,
              producto->tipoProducto);
      producto = (Producto *)list_next(listProductos);
    }
  }
  printf("\nSe ha exportado correctamente el inventario al archivo "
         "inventarioOutput.csv\n");
  printf("\n==========================================\n\n");
  fclose(archivo);
}

void buscarProductoPorCodigo(Map *inventario) {
  int codigo;

  printf("=========================================\n");
  printf("    Busqueda de producto por codigo\n");
  printf("=========================================\n");
  printf("\nIngrese el código del producto : ");
  scanf("%d", &codigo);
  printf("\n=========================================\n");
  MapPair *pair = map_search(inventario, &codigo);
  if (pair) {
    List *listaProductos = (List *)pair->value;
    Producto *producto = (Producto *)list_first(listaProductos);
    printf("\nEl producto %s existe en el inventario\n", producto->nombre);
    printf("Precio: %i\n", producto->precioVenta);

  } else
    printf("El producto con código %d no existe en el inventario\n", codigo);
  printf("\n=========================================\n");
}

void actualizarProductosPorTipo(Map *productosPorTipo, Map *productos) {
  MapPair *pair = map_first(productos);
  while (pair) {
    Producto *producto = (Producto *)pair->value;
    formatearCadena(producto->tipoProducto);
    MapPair *par = map_search(productosPorTipo, producto->tipoProducto);
    if (par)
      list_pushBack(par->value, producto);
    else {
      List *listaProductos = list_create();
      list_pushBack(listaProductos, producto);
      map_insert(productosPorTipo, producto->tipoProducto, listaProductos);
    }
    pair = map_next(productos);
  }
}

void buscarProductosPorTipo(Map *productosPorTipo) {
  char tipo[40];
  printf("============================================\n");
  printf("     Busqueda de producto por tipo\n");
  printf("============================================\n");
  printf("\nIngrese el tipo de producto a buscar: ");
  scanf(" %[^\n]40s", tipo);
  formatearCadena(tipo);
  MapPair *pair = map_search(productosPorTipo, tipo);
  if (pair) {
    List *listaProductos = (List *)pair->value;
    Producto *producto = (Producto *)list_first(listaProductos);
    printf("\nProductos de tipo %s:\n", tipo);
    while (producto) {
      printf("============================================\n");
      printf("Nombre: %s\n", producto->nombre);
      printf("Precio de venta: %d\n", producto->precioVenta);
      producto = (Producto *)list_next(listaProductos);
    }
  } else {
    printf("\nNo se encuentran productos del tipo %s...\n", tipo);
  }
  printf("\n============================================\n");
}

void mostrarPedido(List *pedidos, int size) {
  limpiarPantalla();
  printf("====================================\n");
  printf("             Pedido.\n");
  printf("====================================\n");
  if(size == 0) {
    printf("\nNo hay productos en el pedido\n");
  } else {
    printf("\nProducto(s) en el pedido:\n\n");
    int sumaPrecio = 0;
    Pedido *aux = list_first(pedidos);
    while (aux) {
      printf(" - %s (%d$) x %d\n", aux->producto.nombre, aux->producto.precioVenta, aux->cantidad);
      sumaPrecio += (aux->producto.precioVenta * aux->cantidad);
      aux = list_next(pedidos);
    }
    printf("\nCosto total: %d$\n", sumaPrecio);
  }
  printf("\n====================================\n\n");
}

Pedido *buscarEnPedido(List *pedidos, int codigo)
{
  Pedido *aux = list_first(pedidos);
  while(aux) {
    if(aux->producto.codigo == codigo) {
      return aux;
    }
    aux = list_next(pedidos);
  }
  return NULL;
}

void agregarProductoPedido(List *pedidos, Map *inventario, int size)
{
  printf("==============================================\n");
  printf("               Gestionar Pedido               \n");
  printf("==============================================\n");
  int op;
  do {
    mostrarPedido(pedidos, size);
    int codigo;
    printf("\nIngresar codigo de producto: ");
    scanf("%d", &codigo);

    printf("\n\n==============================================\n");
    
    Pedido *aux = buscarEnPedido(pedidos, codigo);
    if(aux) {
      printf("\nEl producto: %s con codigo: %i\n", aux->producto.nombre, aux->producto.codigo);
      printf("1) Agregar producto\n");
      printf("2) Eliminar producto\n");
      printf("0) Cancelar\n");
      printf("Ingresar opción: ");
      scanf("%d", &op);
    } else {
      printf("\nNo se ha ingresado este producto a la lista\n");
      printf("1) Agregar producto\n");
      printf("0) Cancelar\n");
      printf("Ingresar opción: ");
      scanf("%d", &op);
    } // Se asume que el producto está en el inventario

    printf("\n==============================================\n");
    
    if(op == 0) return;
    int cant;
    int cantTotal;
    if(op == 1) {
      MapPair *pair = map_search(inventario, &codigo);
      if(!pair) {
        printf("No hay registro de este producto en el inventario\n");
      } else {
        int sizeInvenProdu = list_size(pair->value);
        printf("Ingrese la cantidad de productos que se van a añadir: ");
        scanf("%d", &cant);
        if(!aux) cantTotal = cant;
        else cantTotal = aux->cantidad + cant;
        if(sizeInvenProdu < cantTotal) {
          printf("La cantidad deseada es mayor que la cantidad en inventario\n");
          printf("Se descartarán todos los productos en inventario (%d)\n", sizeInvenProdu);
          cantTotal = sizeInvenProdu;
        }
        if(aux) {
          aux->cantidad = cantTotal;
        } else {
          Pedido *new = crearPedido();
          Producto *auxProd = list_first(pair->value);
          new->cantidad = cantTotal;
          new->monto = auxProd->precioVenta * cantTotal;
          new->producto = *auxProd;
          list_pushBack(pedidos, new);
        }
      }
    } else if(aux && op == 2) {
      printf("Ingrese la cantidad de productos que se van a eliminar: ");
      scanf("%d", &cant);
      cantTotal = aux->cantidad - cant;
      if(cantTotal > 0) {
        aux->cantidad = cantTotal;
      } else {
        if(cantTotal == 0) printf("Se eliminará el producto del pedido\n");
        else printf("Se quieren eliminar más productos de los que hay en el inventario, se eliminan todos\n");
        list_popCurrent(pedidos);
      }
    } else {
      printf("Opción no valida, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();
  } while(op != 0);
  printf("==============================================\n\n");
}

void modificarPedido(List *pedidos, Map *inventario, int size)
{
  char op;
  do {
    limpiarPantalla();
    printf("=============================================\n");
    printf("       Menú de modificación del pedido    \n");
    printf("=============================================\n");
    printf("1) Agregar/Eliminar producto del pedido\n");
    printf("2) Volver al menú de gestión de pedido\n");
    printf("Ingresar opción: ");
    scanf(" %c", &op);
    switch(op) {
      case '1':
        agregarProductoPedido(pedidos, inventario, size);
        break;
      case '2':
        printf("\nVolviendo al menú de gestión de pedido\n");
        break;
      default:
        printf("Opción no valida, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();
  } while (op != '2');
}

void pagoEfectivo(List *pedidos)
{
  int sumaPrecio = 0;
  Pedido *aux = list_first(pedidos);
  while (aux) {
    sumaPrecio += (aux->producto.precioVenta * aux->cantidad);
    aux = list_next(pedidos);
  }
  int pago;
  do {
    limpiarPantalla();
    printf("Monto a pagar: %d\n", sumaPrecio);
    printf("Cantidad de dinero pagada: ");
    scanf("%d", &pago);
    if(pago < sumaPrecio) printf("Faltan %d$\n", sumaPrecio - pago);
    else if(pago == sumaPrecio) printf("Pago exacto, no necesita vuelto\n");
    else printf("El vuelto es %d$\n", pago - sumaPrecio);
  } while(pago < sumaPrecio);
}

void procesarPago(List *pedidos, Map *inventario, int size) 
{ 
  int op;
  do {
    mostrarPedido(pedidos, size);
    printf("1) Confirmar pedido\n");
    printf("2) Cancelar\n");
    printf("Ingrese una opción: ");
    scanf("%d", &op);
    switch(op) {
      case 1:
        printf("Forma de pago\n");
        printf("1) Pago con tarjeta\n");
        printf("2) Pago en efectivo\n");
        printf("3) Cancelar Compra\n");
        printf("Ingrese método de pago: ");
        int mp;
        scanf("%d", &mp);
        switch(mp) {
          case 1:
            printf("Pago exitoso!\n");
            presioneTeclaParaContinuar();
            return;
          case 2:
            pagoEfectivo(pedidos);
            presioneTeclaParaContinuar();
            return;
          case 3:
            printf("Compra cancelada\n");
            break;
          default:
            printf("Opción no valida, intente de nuevo.\n");
        }
        break;
      case 2:
        printf("Volviendo al menú de gestión de pedido\n");
        break;
    }
    presioneTeclaParaContinuar();
  } while(op != 2);
}

void mostrarMenuPedidoVacio() 
{
  limpiarPantalla();
  printf("=======================================\n");
  printf("             Menú pedido\n");
  printf("=======================================\n");
  printf("Actualmente no hay datos de pedido\n");
  printf("Se pueden agregar productos manualmente o utilizando la 2da función "
         "del menú principal\n");
  printf("1) Agregar producto con codigo de barras\n");
  printf("2) Agregar productos manualmente");
  printf("0) Volver al menú principal\n");
  printf("Ingrese una opción: ");
}

void mostrarMenuPedido() 
{
  limpiarPantalla();
  printf("=======================================\n");
  printf("             Menú pedido\n");
  printf("=======================================\n");
  printf("Actualmente hay datos de un pedido en el sistema\n");
  printf("1) Ver datos del pedido actual\n");
  printf("2) Modificar datos del pedido actual\n");
  printf("3) Procesar compra\n");
  printf("0) Volver al menú principal\n");
  printf("Ingrese una opción: ");
}
// Pedido *pedido (producto, cantidad, monto)
void agregarManual(List *pedidos)
{
  int cantidad = 0;
  char nombre[1000];
  int monto = 0;
  
  Pedido *ped = crearPedido();
  strcpy(ped->producto.nombre, nombre);
  ped->cantidad = cantidad;
  ped->monto = monto;

}

void procesarVenta(List *pedidos, Map *inventario) // Hermano dejate de wear
{
  int pedidoSize;
  char op;
  do {
    pedidoSize = list_size(pedidos);
    if (pedidoSize == 0) {
      mostrarMenuPedidoVacio();
      scanf(" %c", &op);
      switch (op) {
      case '1':
        modificarPedido(pedidos, inventario, pedidoSize);
        break;
      case '2':
        agregarManual(pedidos);
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
      switch (op) {
      case '1':
        mostrarPedido(pedidos, pedidoSize);
        presioneTeclaParaContinuar();
        break;
      case '2':
        modificarPedido(pedidos, inventario, pedidoSize);
        break;
      case '3':
        procesarPago(pedidos, inventario, pedidoSize);
        break;
      case '0':
        printf("Volviendo al menú principal\n");
        break;
      default:
        printf("Opción no valida, intente de nuevo.\n");
        presioneTeclaParaContinuar();
      }
    }
  } while (op != '0');
  return;
}

void cargarInventario(Map *inventario) {
  FILE *archivo = fopen("data/inventario.csv", "r");
  if (archivo == NULL) {
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ',');
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Producto *producto = crearProducto();
    producto->codigo = atoi(campos[0]);
    strcpy(producto->nombre, campos[1]);
    formatearCadena(producto->nombre);
    producto->precioVenta = atoi(campos[2]);
    producto->fechaV.dia = atoi(campos[3]);
    producto->fechaV.mes = atoi(campos[4]);
    producto->fechaV.year = atoi(campos[5]);
    MapPair *pair = map_search(inventario, &producto->codigo);
    if (pair) {
      List *listaProductos = (List *)pair->value;
      list_pushBack(listaProductos, producto);
    } else {
      List *listaProductos = list_create();
      list_pushBack(listaProductos, producto);
      map_insert(inventario, &producto->codigo,
                 listaProductos); // recordar que la key del mapa del inventario
                                  // es el codigo del producto
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
  actualizarProductosPorTipo(productosPorTipo, precioProductos);

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);
    switch (opcion) {
    case 1:
      limpiarPantalla();
      agregarProductoInventarioUser(inventario);
      break;
    case 2:
      limpiarPantalla();
      cargarPedidosCSV(pedidos, inventario,
                       precioProductos); // Cargar un pedido para un cliente
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
      // test(productosPorTipo);
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
  } while (opcion != 11);
  printf("Programa terminado correctamente :]");

  map_clean(inventario);
  map_clean(precioProductos);
  map_clean(productosPorTipo);
  list_clean(pedidos);

  return 0;
}