#include "tdas/extra.h"
#include "tdas/heap.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// aqui tenemos es struct de fechaVenciento con dia, mes y año.
typedef struct {
  int dia;
  int mes;
  int year;
} fechaVencimiento;

// aqui tenemos el tipo de dato size_t con un "cambio de nombre"
typedef size_t st;

// aqui tenemos el strcut de producto que ultiliza el struct de tipoVencimiento
typedef struct {
  int precioCosto;         // Costo del producto
  int precioVenta;         // Precio a la venta
  st codigo;               // Codigo del producto
  char nombre[10000];      // Nombre del prodcuto
  char tipoProducto[1000]; // Tipo de producto
  fechaVencimiento fechaV; // fecha de vencimiento
} Producto;

// struct tipo pedido que contiene el monto y cantidad de productos
typedef struct {
  int monto;
  int cantidad;
  Producto producto;
} Pedido;

// Funcion crear pedido
Pedido *crearPedido() {
  Pedido *pedido = (Pedido *)malloc(
      sizeof(Pedido)); // reserva memoria para crear un pedido vacio
  return pedido;       // retorna
}

// funcion crear producto
Producto *crearProducto() {
  Producto *producto = (Producto *)malloc(
      sizeof(Producto)); // Crea un producto vacío reservando memoria
  return producto;       // retorna
}

// Funcion copiar para producto
Producto *copiarProducto(Producto *producto) {// la funcion recibe un producto 
  Producto *new = crearProducto(); // creo un nuevo producto que sera la copia
                                   // con la fucion crarProducto
  new->precioCosto =
      producto->precioCosto; // le asigno el precio costo de mi otro producto
  new->precioVenta = producto->precioVenta; // le asigno el precio venta
  new->codigo = producto->codigo;           // le asigno el codigo
  strcpy(new->nombre, producto->nombre);    // le copio el nombre
  strcpy(new->tipoProducto, producto->tipoProducto); // le copio el tipo
  return new; // retorna el nuevo producto copiado
}

// funcion formatear cadena
void formatearCadena(char *cadena) {// la funcion recibe una cadena
  for (int i = 0; cadena[i] != '\0';
       ++i) { // recorro la cadena hasta el ultimo caracter
    if (isalpha(
            cadena[i])) { // si el caracter es alfabetico lo vuelvo mayuscula
      cadena[i] = toupper(cadena[i]);
    } else if (cadena[i] == '\n') { // si el caracter es un salto de linea lo
                                    // volvemos el caracter nulo
      cadena[i] = '\0';
    }
  }
}

// funcion para agregar productos al inventario
void agregarProductoInventarioUser(Map *inventario) {// la funcion recibe el mapa del inventario
  char car; // Creo una variable caracter para el menú
  do {      // hago un ciclo do/while para ir mostrando el menu y leyendo datos
    limpiarPantalla(); // se limpia la pantalla
    printf("=================================\n");
    printf("       Agregar producto.\n");
    printf("=================================\n"); // se crea el menu
    st codigo;    // variable para guardar el codigo del producto
    int cantidad; // variable para guardar la cantidad del producto
    printf("Ingrese el código del producto: ");
    scanf("%zu", &codigo); // se ingresa el producto con un mensaje
    printf("Ingrese la cantidad de productos a agregar: ");
    scanf("%d", &cantidad); // se le ingresa la cantidad con un mensaje
    MapPair *pair = map_search(
        inventario,
        &codigo); // se crea un puntero pair para buscar denptro del mapa
    Producto *producto = crearProducto(); // creamos un nuevo producto
    producto->codigo = codigo;            // Le asignamos el codigo al producto
    if (pair) { // Si el producto ya existe en el inventario
      List *listaProductos = (List *)pair->value;
      producto = copiarProducto(list_first(
          listaProductos)); // se copia el producto en la lista de productos
      printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
      scanf("%d/%d/%d", &producto->fechaV.dia,
            &producto->fechaV.mes, // se ingresa la fecha de vencimiento
            &producto->fechaV.year);
      for (int i = 0; i < cantidad; ++i) // recorremos la lista
        list_pushBack(listaProductos,
                      producto); // lo incertamos al final de la lista
    } else {                     // Si el producto no existe en el inventario
      List *listaProductos = list_create(); // se crea una lista de productos
      printf("Ingrese el nombre del producto: ");
      scanf(" %[^\n]1000s", producto->nombre); // se ingresa el nombre
      getchar();
      formatearCadena(producto->nombre); // lo transformamos todo a mayusculas
      printf("Ingrese el precio de costo del producto: ");
      scanf("%d", &producto->precioCosto); // ingresamos el costo
      printf("Ingrese el precio de venta al público del producto: ");
      scanf("%d", &producto->precioVenta); // ingresamos el precio venta
      printf("Ingrese el tipo de producto: ");
      scanf(" %[^\n]1000s",
            producto->tipoProducto); // ingresamos el tipo de producto
      getchar();
      formatearCadena(producto->tipoProducto); // lo transformamos a mayusculas
      printf("Ingrese la fecha de vencimiento del producto (dd/mm/yyyy): ");
      scanf("%d/%d/%d", &producto->fechaV.dia, &producto->fechaV.mes,
            &producto->fechaV.year);     // ingresamos la fecha de vencimiento
      for (int i = 0; i < cantidad; ++i) // recorremos la lista
        list_pushBack(listaProductos,
                      producto); // incertamos al final de la lista
      map_insert(inventario, &producto->codigo,
                 listaProductos); // incertamos el producto en el mapa
    }
    printf("Presione cualquier tecla para agregar otro producto, para volver "
           "al menú presione n: \n");
    scanf(" %c", &car); // aqui se ingresa un caracter para ver si se sigue
                        // ingresando productos
  } while (car != 'n'); // mientras el caracter no se n, se repite la funcion.
}

// funcion para visualizar el inventario
void visualizacionInventario(Map *inventario) {// la funcion recibe el mapa del inventario
  printf("====================================\n");
  printf("             Inventario\n");
  printf("====================================\n"); // muestra el muenu del
                                                    // inventario
  MapPair *pair = map_first(
      inventario); // accedemos al primer elemento del mapa con productos
  if (!pair) {     // si el mapa esta vacio muestra el mensaje
    printf("No hay productos guardados en el inventario!\n");
    return; // retornamos
  }
  while (pair) { // mientras exista productos dentro del mapa
    List *listaProductos =
        (List *)
            pair->value; // accedemos a la lista de productos dentro del mapa
    Producto *producto =
        list_first(listaProductos); // accedemos al primer elemento
    printf("\n================================================================="
           "======\n");
    printf("\nLista de productos %s que hay en el inventario: \n",
           producto->nombre); // mostramos el nombre por pantalla
    while (producto) {        // mientras el producto en el mapa exista...
      printf("  * Producto %s con fecha de vencimiento el %d/%d/%d\n",
             producto->nombre, producto->fechaV.dia, producto->fechaV.mes,
             producto->fechaV.year); // mostramos la cantidad de prodcutos con
                                     // su fecha de vencimiento
      producto = (Producto *)list_next(
          listaProductos); // pasamos al siguiente elemnto de la lista
    }
    pair = map_next(inventario); // pasamos al siguiente elmento del mapa
  }
  printf("\n==================================================================="
         "====\n");
}

// funcion para actualizar el inventario
void actualizarInventario(Map *inventario, st codigo,
                          int cantidad) { //la funcion recibe el mapa del inventario, el codigo del producto y la cantidad
  MapPair *pair = map_search(inventario, &codigo);// se busca el punteor pair en el mapa
  if (pair == NULL)// si pair no existe o es igual a NULL se imprime el mensaje
    printf("\nNo se encuentra el producto en el inventario del almacén\n");
  else {// si pari existe
    List *listaProductos = (List *)pair->value;// se accede a la lista de productos
    Producto *producto = (Producto *)list_first(listaProductos);// se busca el primer elemento de la lista de productos
    int cantidadProductos = list_size(listaProductos);// la cantidad de productos es igual a el tamaño de la lista
    char nombreProducto[1000];// creamos una variable para el nombre
    strcpy(nombreProducto, producto->nombre);// la copiamos con el nombre del producto 
    if (cantidadProductos == 0)// si la cantidad de productos es 0
      printf("No quedan unidades del producto!\n");// no hay productos en el inventario
    else if (cantidadProductos < cantidad)// si la cantidad de productos de la lista es mayor a la cantidad de productos
      printf("\nNo se cuenta con la cantidad solicitada del producto %s",
             nombreProducto); // Daniel tienes que arreglar esto !!!!
    else {// en cualquier otro caso 
      int contadorEliminados = 0;// creamos una variable de para contar los eliminados
      while (contadorEliminados < cantidad) {// mientras los eliminado sean menor a la cantidad
        list_popBack(listaProductos);// elimina desde atras de la lista los productos
        ++contadorEliminados;// suma 1 a la cantidad de prductos eliminados
      }
      printf("\nSe ha actualizado correctamente, quedan %d unidades del "
             "producto %s\n",
             list_size(listaProductos), nombreProducto);// se muestra el mensaje de cuantos productos se han eliminado y cuantos quedan
    }
  }
}

// funcion para cargar productos del archivo
void cargarProductos(Map *precioProductos) {// la funcion recibe el mapa con el precio de los productos
  FILE *archivo = fopen("data/productos.csv", "r");// se abre el archivo que vamos a ocupar
  if (archivo == NULL) {// si el archivo es igual a NULL, es porque no se abrio el archivo 
    perror("No se pudo abrir el archivo\n");
    return;// retornamos
  }
  char **campos;// creamos la variable campos que sera cada columna del archivo
  campos = leer_linea_csv(archivo, ',');// se lee el archivo con la funcion y separado con una coma
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {// mientras cada columna sea distinta de NULL
    Producto *producto = crearProducto();// se crea un nuevo producto 
    producto->codigo = atoi(campos[0]);// se le asigna el codigo que aparezca en el archivo
    strcpy(producto->nombre, campos[1]);// se copia el nombre que hay en el archivo
    formatearCadena(producto->nombre);// se transforma a mayuscula
    producto->precioVenta = atoi(campos[2]);// copiamos el precio de venta
    strcpy(producto->tipoProducto, campos[3]);// se copia el el tipo de producto
    formatearCadena(producto->tipoProducto);// se transforma a mayuscula
    map_insert(precioProductos, strdup(producto->nombre), producto);// insertamos los datos en el mapa
  }
}

void cargarPedidosCSV(List *pedidos, Map *inventario,
                      Map *productosPrecio) { // Carga los pedidos del archivo
  // El tercer mapa es el que guarda los productos por
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

void agregarProductoAuto(
    Map *inventario, Producto *producto) { // Agrega un producto al inventario
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

void cargarCargaDesdeCSV(
    Map *inventario, Map *precioProductos) { // Carga los productos del archivo
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

void mostrarProductos(Map *productos) { // Muestra los productos
  printf("================================================\n");
  printf("           Productos a la venta\n");
  printf("================================================\n\n");
  MapPair *pair = map_first(productos);
  while (pair) {
    Producto *producto = (Producto *)pair->value;
    printf("================================================\n\n");
    printf("Código: %zu\n", producto->codigo);
    printf("Nombre: %s\n", producto->nombre);
    printf("Precio de costo: %d\n\n", producto->precioVenta);
    pair = map_next(productos);
  }
  printf("================================================\n");
}

void cuadrarInventario(Map *inventario,
                       Map *productosPrecio) { // Cuadra el inventario
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
  if (!bool)
    printf("      El inventario estaba cuadrado!\n");
  printf("\n================================================\n\n");
}

void exportarInventario(Map *inventario) { // Exporta el inventario
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
      fprintf(archivo, "%zu,%s,%d,%d,%d,%d,%d,%s\n", producto->codigo,
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

void buscarProductoPorCodigo(
    Map *inventario) { // Busca un producto por su codigo
  st codigo;

  printf("=========================================\n");
  printf("    Busqueda de producto por codigo\n");
  printf("=========================================\n");
  printf("\nIngrese el código del producto : ");
  scanf("%zu", &codigo);
  printf("\n=========================================\n");
  MapPair *pair = map_search(inventario, &codigo);
  if (pair) {
    List *listaProductos = (List *)pair->value;
    Producto *producto = (Producto *)list_first(listaProductos);
    printf("\nEl producto %s existe en el inventario\n", producto->nombre);
    printf("Precio: %i\n", producto->precioVenta);

  } else
    printf("El producto con código %zu no existe en el inventario\n", codigo);
  printf("\n=========================================\n");
}

void actualizarProductosPorTipo(
    Map *productosPorTipo, Map *productos) { // Actualiza los productos por tipo
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

void buscarProductosPorTipo(
  Map *productosPorTipo) { // Busca los productos por tipo
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

void headerModificacionPedido(void) // Imprime el encabezado de la modificacion de pedido
{
  limpiarPantalla();
  printf("=============================================\n");
  printf("       Menú de modificación del pedido    \n");
  printf("=============================================\n");
}

void mostrarPedido(List *pedidos) { // Muestra el pedido
  int size = list_size(pedidos); // Tamaño del pedido
  limpiarPantalla();
  printf("====================================\n");
  printf("             Pedido.\n");
  printf("====================================\n");
  if (size == 0) {
    printf("\nNo hay productos en el pedido\n");
  } else {
    printf("\nProducto(s) en el pedido:\n\n");
    int sumaPrecio = 0; // Variable que guarda el precio total
    int cont = 1; // Contador de productos
    Pedido *aux = list_first(pedidos); // Primer elemento del pedido
    while (aux) { // Se imprimen los productos del pedido, junto a su precio de venta y cantidad
      printf(" %d %s (%d$) x %d\n", cont, aux->producto.nombre,
             aux->producto.precioVenta, aux->cantidad);
      cont++; // Aumentar contador
      sumaPrecio += aux->monto; // Sumar al precio total
      aux = list_next(pedidos); // Mover puntero del pedido al siguiente producto
    }
    printf("\nCosto total: %d$\n", sumaPrecio); // Imprime el precio total del pedido completo
  }
  printf("\n====================================\n\n");
}

Pedido *buscarEnPedido(List *pedidos, 
                       st codigo) //  Busca en el pedido un producto por su codigo
{
  Pedido *aux = list_first(pedidos); // Puntero al primer producto del pedido
  while (aux) {
    if (aux->producto.codigo == codigo) {
      return aux; // Se retorna el producto del pedido si se encuentra dentro de la lista
    }
    aux = list_next(pedidos);
  }
  return NULL; // Si no se encuentra el producto, retorna NULL
}

void agregarCodigo(List *pedidos,
    Map *inventario) // Agrega un producto del pedido por su codigo
{
  st codigo; // Variable que guarda el codigo de producto
  printf("\nIngresar codigo de producto: ");
  scanf("%zu", &codigo); // Se lee el codigo de producto

  printf("\n\n==============================================\n");
  MapPair *pair = map_search(inventario, &codigo); // Se busca en el inventario utilizando el codigo
  if (!pair) // Si no esta el producto en el inventario
    printf("No hay registro de este producto en el inventario\n");
  else { // Si esta el producto en el inventario
    int sizeInvenProdu = list_size(pair->value); // Tamaño/cantidad del producto en el inventario
    Pedido *aux = buscarEnPedido(pedidos, codigo); // Se busca el producto en la lista de pedido
    if (aux) // Si el producto está en la lista
      printf("\nEl producto: %s\n", aux->producto.nombre);
    else // Si el producto no está en la lista
      printf("\nNo se ha ingresado este producto a la lista\n");
    int cant; // Cantidad del producto que se va a añadir
    int cantTotal; // Cantidad del producto total
    printf("Ingrese la cantidad de productos que se van a añadir: ");
    scanf("%d", &cant); // Se lee la cantidad de producto a añadir
    if (aux) // Si el producto está en la lista, se suman las cantidades
      cantTotal = aux->cantidad + cant;
    else // Si el producto está en la lista, la cantidad total es igual a la deseada
      cantTotal = cant;
    if (sizeInvenProdu < cantTotal) { // Si la cantidad deseada es mayor que lo que hay disponible
      printf("La cantidad deseada es mayor que la cantidad en inventario\n");
      printf("Se descartará todo el producto del inventario (%d)\n", sizeInvenProdu);
      cantTotal = sizeInvenProdu; // Se arregla la cantidad para que sea igual a la disponible
    }
    if (aux)
      aux->cantidad = cantTotal; // Se ajusta la cantidad dentro del pedido
    else {
      Pedido *new = crearPedido(); // Se agrega un nuevo producto junto a sus datos a la lista de pedido
      Producto *auxProd = list_first(pair->value);
      new->cantidad = cantTotal;
      new->monto = auxProd->precioVenta *cantTotal;
      new->producto = *auxProd;
      list_pushBack(pedidos, new);
    }
  }
  presioneTeclaParaContinuar();
  return;
}

void agregarManual(List *pedidos) // Agrega un producto manualmente al pedido
{
  int cantidad = 0;
  char nombre[1000];
  int monto = 0; // Definición de variables más header de modificación
  int total = 0;
  headerModificacionPedido();

  Pedido *ped = crearPedido(); // Se crea el pedido junto a su información
  strcpy(ped->producto.nombre, nombre);
  ped->cantidad = cantidad;
  ped->monto = monto;

  list_pushBack(pedidos, ped); // Se agregan el producto a la lista y se leen los datos
  printf("Ingrese el nombre del producto: ");
  scanf(" %[^\n]s", ped->producto.nombre);
  getchar();
  formatearCadena(ped->producto.nombre);
  printf("Ingrese la cantidad del producto: ");
  scanf("%d", &ped->cantidad);
  printf("Ingrese el precio del producto: ");
  scanf("%d", &ped->producto.precioVenta);
  ped->monto = ped->producto.precioVenta * ped->cantidad;
}

void agregarProductoPedido(List *pedidos,
                           Map *inventario) // Agrega un producto al pedido
{
  int op; // Variable de opción
  do {
    headerModificacionPedido(); // Imprimir header de modificación y opciones
    printf("1) Agregar con codigo de producto\n");
    printf("2) Agregar producto manualmente\n");
    printf("0) Volver\n");
    printf("Ingrese una opción: ");

    scanf("%d", &op); // Leer opción
    switch (op) {
    case 1:
      agregarCodigo(pedidos, inventario);
      break;
    case 2:
      agregarManual(pedidos);
      break;
    case 0:
      break;
    default:
      printf("Opción no valida, intente de nuevo.\n");
      presioneTeclaParaContinuar();
    }
  } while (op != 0);
  printf("==============================================\n\n");
}

void eliminarProductoPedido(List *pedidos,
                            Map *inventario) // Elimina un producto del pedido
{
  int op;
  int size = list_size(pedidos);
  do {
    mostrarPedido(pedidos);
    printf("Ingrese número de producto (0 para cancelar): \n");
    scanf("%d", &op);
    if (op == 0)
      return;
    else if (op > size || op < 0) {
      printf("Opción no valida, intente de nuevo.\n");
      presioneTeclaParaContinuar();
    }
  } while (op > size || op < 0);
  int cont = 1;
  Pedido *aux = list_first(pedidos);
  while (cont != op) {
    aux = list_next(pedidos);
    cont++;
  }
  if (cont == op) {
    int cant;
    printf("Ingrese la cantidad de producto que desea eliminar: ");
    scanf("%d", &cant);
    if (cant >= aux->cantidad)
      list_popCurrent(pedidos);
    else {
      aux->cantidad -= cant;
      aux->monto = aux->producto.precioVenta * aux->cantidad;
    }
  }
}

void modificarPedido(List *pedidos, Map *inventario) // Modifica el pedido
{
  int op;
  do {
    int size = list_size(pedidos);
    headerModificacionPedido();
    printf("1) Agregar producto del pedido\n");
    if (size > 0)
      printf("2) Eliminar producto del pedido\n");
    printf("0) Volver al menú de gestión de pedido\n");
    printf("Ingresar opción: ");
    scanf("%d", &op);
    if (op == 1)
      agregarProductoPedido(pedidos, inventario);
    else if (size > 0 && op == 2)
      eliminarProductoPedido(pedidos, inventario);
    else if (op == 0)
      printf("\nVolviendo al menú de gestión de pedido\n");
    else
      printf("Opción no valida, intente de nuevo.\n");
    presioneTeclaParaContinuar();
  } while (op != 0);
}

void pagoEfectivo(List *pedidos) // Realiza el pago en efectivo
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
    printf("Monto a pagar: %d$\n", sumaPrecio);
    printf("Cantidad de dinero pagada: ");
    scanf("%d", &pago);
    if (pago < sumaPrecio) {
      printf("Faltan %d$\n", sumaPrecio - pago);
      presioneTeclaParaContinuar();
    } else if (pago == sumaPrecio)
      printf("Pago exacto, no necesita vuelto\n");
    else
      printf("El vuelto es %d$\n", pago - sumaPrecio);
  } while (pago < sumaPrecio);
}

void eliminarPedido(List *pedidos, Map *inventario) {
  Pedido *aux = list_first(pedidos);
  while (aux) {
    MapPair *pair = map_search(inventario, &aux->producto.codigo);
    if (pair) {
      if (list_size(pair->value) == aux->cantidad)
        map_remove(inventario, &aux->producto.codigo);
      while (aux->cantidad > 0) {
        list_popBack(pair->value);
        aux->cantidad--;
      }
    }
    aux = list_next(pedidos);
    list_popBack(pedidos);
  }
}

void procesarPago(List *pedidos, Map *inventario) // Procesa el pago
{
  int op;
  do {
    mostrarPedido(pedidos);
    printf("1) Confirmar pedido\n");
    printf("0) Cancelar\n");
    printf("Ingrese una opción: ");
    scanf("%d", &op);
    switch (op) {
    case 1: // Confirmar pedido
      printf("Forma de pago\n");
      printf("1) Pago con tarjeta\n");
      printf("2) Pago en efectivo\n");
      printf("0) Cancelar Compra\n");
      printf("Ingrese método de pago: ");
      int mp;
      scanf("%d", &mp);
      switch (mp) {
      case 1: // Pago con tarjeta
        printf("Pago exitoso!\n");
        printf("Gracias por su compra!\n");
        break;
      case 2: // Pago en efectivo
        pagoEfectivo(pedidos);
        printf("Gracias por su compra!\n");
        break;
      case 0: // Cancelar compra
        printf("Compra cancelada\n");
        break;
      default:
        printf("Opción no valida, intente de nuevo.\n");
      }
      if (mp >= 1) {
        eliminarPedido(pedidos, inventario);
        op = 0;
      }
      break;
    case 0:
      printf("Volviendo al menú de gestión de pedido\n");
      break;
    }
    presioneTeclaParaContinuar();

  } while (op != 0);
}

void mostrarMenuPedidoVacio() // Muestra el menu de pedido vacio
{
  limpiarPantalla();
  printf("=======================================\n");
  printf("             Menú pedido\n");
  printf("=======================================\n");
  printf("Actualmente no hay datos de pedido\n");
  printf("Se pueden agregar productos manualmente o utilizando la 2da función "
         "del menú principal\n");
  printf("1) Modificar pedido actual\n");
  printf("0) Volver al menú principal\n");
  printf("Ingrese una opción: ");
}

void mostrarMenuPedido() // Muestra el menu de pedido
{
  limpiarPantalla();
  printf("=======================================\n");
  printf("             Menú pedido\n");
  printf("=======================================\n");
  printf("Actualmente hay datos de un pedido en el sistema\n");
  printf("1) Ver datos del pedido actual\n");
  printf("2) Modificar pedido actual\n");
  printf("3) Procesar compra\n");
  printf("0) Volver al menú principal\n");
  printf("Ingrese una opción: ");
}

void procesarVenta(List *pedidos, Map *inventario) // Procesa la venta
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
        modificarPedido(pedidos, inventario);
        break;
      case '0':
        printf("\nVolviendo al menú principal\n");
        break;
      default:
        printf("Opción no valida, intente de nuevo.\n");
        presioneTeclaParaContinuar();
      }
    } else {
      mostrarMenuPedido();
      scanf(" %c", &op);
      switch (op) {
      case '1': // Ver datos del pedido actual
        mostrarPedido(pedidos);
        presioneTeclaParaContinuar();
        break;
      case '2': // Modificar datos del pedido actual
        modificarPedido(pedidos, inventario);
        break;
      case '3': // Procesar compra
        procesarPago(pedidos, inventario);
        break;
      case '0': // Volver al menú principal
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

void cargarInventario(Map *inventario) { // Carga el inventario
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

int main(int argc, char *argv[]) { // Funcion principal

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
      agregarProductoInventarioUser(
          inventario); // Agregar producto al inventario
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
      cargarCargaDesdeCSV(
          inventario, precioProductos); // Cargar inventario desde un archivo
      break;
    case 5:
      limpiarPantalla();
      exportarInventario(inventario); // Exportar inventario
      break;
    case 6:
      limpiarPantalla();
      mostrarProductos(precioProductos); // Ver productos
      break;
    case 7:
      limpiarPantalla();
      cuadrarInventario(inventario, precioProductos); // Cuadrar inventario
      break;
    case 8:
      limpiarPantalla();
      buscarProductoPorCodigo(inventario); // Buscar producto por codigo
      break;
    case 9:
      limpiarPantalla();
      // test(productosPorTipo);
      buscarProductosPorTipo(productosPorTipo); // Buscar productos por tipo
      break;
    case 10:
      procesarVenta(pedidos, inventario); // Procesar venta
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

  map_clean(inventario); // Liberar memoria
  free(inventario);
  map_clean(precioProductos);
  free(precioProductos);
  map_clean(productosPorTipo);
  free(productosPorTipo);
  list_clean(pedidos);
  free(pedidos);

  return 0;
}