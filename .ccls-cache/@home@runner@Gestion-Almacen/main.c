#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// aqui tenemos es struct de fechaVenciento con dia, mes y año.
typedef struct {
  int dia; // dia de vencimiento
  int mes; // mes de vencimiento
  int year; // año de vencimiento
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
  int monto; // monto del pedido
  int cantidad; // cantidad de productos
  Producto producto; // producto
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
    printf("Ingrese cualquier caracter para agregar otro producto, para volver "
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

//funcion que actualiza la lista de pedidos
void actualizarPedidos(Map *inventario, List *pedidos) {
  Pedido *pedido = list_first(pedidos);
  while(pedido){
    MapPair *pair = map_search(inventario, &pedido->producto.codigo);
    if(pair){
      List *listaProductos = (List *)pair->value;
      int cantidadProductos = list_size(listaProductos);
      if(cantidadProductos < pedido->cantidad)
        pedido->cantidad = cantidadProductos;
    }
    pedido = (Pedido *)list_next(pedidos);
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
  FILE *archivo = fopen("data/pedidos.csv", "r"); // abrimos el archivo
  if (archivo == NULL) { // si el archivo es igual a NULL, es porque no se abrio el archivo
    perror("No se pudo abrir el archivo\n"); // imprimimos el mensaje de error
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ','); // leemos el archivo con la funcion y separado con una coma
  list_clean(pedidos); // limpiamos la lista de pedidos
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) { // mientras cada columna sea distinta de NULL
    Pedido *pedido = crearPedido(); // creamos un nuevo pedido
    pedido->cantidad = atoi(campos[0]); // se le asigna la cantidad que aparezca en el archivo
    strcpy(pedido->producto.nombre, campos[1]); // se copia el nombre que hay en el archivo
    formatearCadena(pedido->producto.nombre); // se transforma a mayuscula
    MapPair *pair = map_search(productosPrecio, pedido->producto.nombre); // se busca el puntero pair en el mapa
    if (pair) { // si pair existe
      Producto *producto = (Producto *)pair->value; // se accede a la lista de productos
      pedido->producto.precioVenta = producto->precioVenta; // se le asigna el precio de venta
      pedido->producto.codigo = producto->codigo; // se le asigna el codigo
      strcpy(pedido->producto.tipoProducto, producto->tipoProducto); // se copia el el tipo de producto
      pedido->monto = pedido->producto.precioVenta * pedido->cantidad; // se calcula el monto
      list_pushBack(pedidos, pedido); // se agrega el pedido a la lista de pedidos
    }
  }
  fclose(archivo); // se cierra el archivo
  actualizarPedidos(inventario, pedidos);
  Pedido *aux = (Pedido *)list_first(pedidos); // se crea un puntero auxiliar para recorrer la lista de pedidos
  while (aux) { // mientras el puntero auxiliar exista
    printf("\n==========================================\n");
    printf("\nNombre producto: %s\n", aux->producto.nombre); // se imprime el nombre del producto
    printf("Precio: %d\n", aux->producto.precioVenta); // se imprime el precio del producto
    printf("Cantidad: %d\n", aux->cantidad); // se imprime la cantidad del producto
    printf("Tipo: %s\n", aux->producto.tipoProducto); // se imprime el tipo del producto//
    aux = (Pedido *)list_next(pedidos); // se pasa al siguiente pedido
  }
  printf("\n==========================================\n");
}

void agregarProductoAuto(
    Map *inventario, Producto *producto) { // Agrega un producto al inventario
  MapPair *pair = map_search(inventario, &producto->codigo); // Busca el producto en el inventario
  if (pair) { // Si el producto ya existe en el inventario
    List *listaProductos = (List *)pair->value; // Obtiene la lista de productos asociada al producto
    list_pushBack(listaProductos, producto); // Agrega el producto a la lista de productos
  } else { // Si el producto no existe en el inventario
    List *listaProductos = list_create(); // Crea una nueva lista de productos
    list_pushBack(listaProductos, producto); // Agrega el producto a la lista de productos
    map_insert(inventario, &producto->codigo, listaProductos); // Inserta el producto en el inventario
  }
}

void cargarCargaDesdeCSV(
    Map *inventario, Map *precioProductos) { // Carga los productos del archivo
  FILE *archivo = fopen("data/carga.csv", "r");  // abrimos el archivo
  if (archivo == NULL) { // si el archivo es igual a NULL, es porque no se abrio el archivo
    perror("No se pudo abrir el archivo\n"); // imprimimos el mensaje de error
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ','); // leemos el archivo con la funcion y separado con una coma
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) { // mientras cada columna sea distinta de NULL
    Producto *producto = crearProducto(); // creamos un nuevo producto
    strcpy(producto->nombre, campos[0]); // se copia el nombre que hay en el archivo
    formatearCadena(producto->nombre); // se transforma a mayuscula
    producto->precioCosto = atoi(campos[1]); // copiamos el precio de costo
    producto->precioVenta = atoi(campos[2]); // copiamos el precio de venta
    strcpy(producto->tipoProducto, campos[3]); // se copia el el tipo de producto
    formatearCadena(producto->tipoProducto); // se transforma a mayuscula
    producto->fechaV.dia = atoi(campos[4]); // copiamos el dia de vencimiento 
    producto->fechaV.mes = atoi(campos[5]); // copiamos el mes de vencimiento
    producto->fechaV.year = atoi(campos[6]); // copiamos el año de vencimiento
    MapPair *pair = map_search(precioProductos, producto->nombre); // se busca el puntero pair en el mapa
    if (pair) { // si pair existe
      Producto *new = (Producto *)pair->value; // se accede a la lista de productos
      producto->codigo = new->codigo; // se le asigna el codigo
      agregarProductoAuto(inventario, producto); // se agrega el producto al inventario
    } else { // si pair no existe
      printf("\nEl producto %s no existe en los productos que se venden\n", 
             producto->nombre); // se imprime el mensaje de error
    } 
  }
  fclose(archivo); // se cierra el archivo
  printf("\nSe ha cargado exitosamente!\n");
}

void mostrarProductos(Map *productos) { // Muestra los productos
  printf("================================================\n");
  printf("           Productos a la venta\n");
  printf("================================================\n\n");
  MapPair *pair = map_first(productos); // Obtiene el primer par de clave-valor del mapa
  while (pair) { // Mientras haya pares en el mapa
    Producto *producto = (Producto *)pair->value; // Obtiene el valor asociado al par
    printf("================================================\n\n"); 
    printf("Código: %zu\n", producto->codigo); // Imprime el código del producto
    printf("Nombre: %s\n", producto->nombre); // Imprime el nombre del producto
    printf("Precio de costo: %d\n\n", producto->precioVenta); // Imprime el precio de costo del producto
    pair = map_next(productos); // Obtiene el siguiente par de clave-valor del mapa
  }
  printf("================================================\n");
}

void cuadrarInventario(Map *inventario,
                       Map *productosPrecio) { // Cuadra el inventario
  int ctrl = 0;
  printf("================================================\n");
  printf("              Cuadrar Inventario\n");
  printf("================================================\n\n");
  MapPair *pair = map_first(inventario); // Obtiene el primer par de clave-valor del mapa
  if (!pair) { // Si el mapa está vacío
    printf("No hay elementos en el inventario!\n");
    return;
  }
  while (pair) { // Mientras haya pares en el mapa
    List *listaProductos = (List *)pair->value; // Obtiene la lista de productos asociada al producto
    Producto *producto = (Producto *)list_first(listaProductos); // Obtiene el primer producto de la lista
    if (producto == NULL) { // Si la lista está vacía
      pair = map_next(inventario); // Obtiene el siguiente par de clave-valor del mapa
      continue; 
    }
    MapPair *nextPair = map_next(inventario); // Obtiene el siguiente par de clave-valor del mapa
    MapPair *par = map_search(productosPrecio, producto->nombre); // Busca el producto en el mapa
    if (par == NULL) {
      printf("El producto %s no existe en los productos que se venden, será " 
             "eliminado del inventario\n",
             producto->nombre); // Imprime el mensaje de error
      list_clean(pair->value); // Limpia la lista de productos asociada al producto
      map_remove(inventario, &producto->codigo); // Elimina el producto del mapa
      ctrl++; // Suma 1 a la variable bool
    }
    pair = nextPair;
  }
  if (!ctrl) // Si no se eliminó ningún producto
    printf("      El inventario estaba cuadrado!\n");
  printf("\n================================================\n\n");
}

void exportarInventario(Map *inventario) { // Exporta el inventario
  printf("==========================================\n");
  printf("       Exportacion del inventario\n");
  printf("==========================================\n");
  FILE *archivo = fopen("data/inventarioOutput.csv", "w"); // abrimos el archivo
  if (archivo == NULL) { // si el archivo es igual a NULL, es porque no se abrio el archivo
    perror("No se pudo abrir el archivo\n"); // imprimimos el mensaje de error
    return;
  }
  for (MapPair *pair = map_first(inventario); pair != NULL; 
       pair = map_next(inventario)) { // recorre el mapa
    List *listProductos = (List *)pair->value; // Obtiene la lista de productos asociada al producto
    Producto *producto = (Producto *)list_first(listProductos); // Obtiene el primer producto de la lista
    while (producto) { // Mientras haya productos en la lista
      fprintf(archivo, "%zu,%s,%d,%d,%d,%d,%d,%s\n", producto->codigo,
              producto->nombre, producto->precioCosto, producto->precioVenta,
              producto->fechaV.dia, producto->fechaV.mes, producto->fechaV.year,
              producto->tipoProducto); // Escribe los datos del producto en el archivo
      producto = (Producto *)list_next(listProductos); // Obtiene el siguiente producto de la lista
    }
  }
  printf("\nSe ha exportado correctamente el inventario al archivo "
         "inventarioOutput.csv\n"); 
  printf("\n==========================================\n\n");
  fclose(archivo); // se cierra el archivo
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
  MapPair *pair = map_search(inventario, &codigo); // Busca el producto en el inventario
  if (pair) { // Si el producto existe en el inventario
    List *listaProductos = (List *)pair->value; // Obtiene la lista de productos asociada al producto
    Producto *producto = (Producto *)list_first(listaProductos); // Obtiene el primer producto de la lista
    printf("\nEl producto %s existe en el inventario\n", producto->nombre); // Imprime el nombre del producto
    printf("Precio: %i\n", producto->precioVenta); // Imprime el precio del producto

  } else // Si el producto no existe en el inventario
    printf("El producto con código %zu no existe en el inventario\n", codigo); 
  printf("\n=========================================\n");// Imprime el mensaje de error
}

void actualizarProductosPorTipo(
    Map *productosPorTipo, Map *productos) { // Actualiza los productos por tipo
  MapPair *pair = map_first(productos); // Obtiene el primer par de clave-valor del mapa
  while (pair) { // Mientras haya pares en el mapa
    Producto *producto = (Producto *)pair->value; // Obtiene el valor asociado al par
    formatearCadena(producto->tipoProducto); // Se transforma a mayuscula
    MapPair *par = map_search(productosPorTipo, producto->tipoProducto); // Busca el producto en el mapa
    if (par)  // Si el producto existe en el mapa
      list_pushBack(par->value, producto); // Agrega el producto a la lista de productos 
    else { // Si el producto no existe en el mapa
      List *listaProductos = list_create(); // Crea una nueva lista de productos
      list_pushBack(listaProductos, producto); // Agrega el producto a la lista de productos
      map_insert(productosPorTipo, producto->tipoProducto, listaProductos); // Inserta el producto en el mapa
    }
    pair = map_next(productos); // Obtiene el siguiente par de clave-valor del mapa
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
  formatearCadena(tipo); // Transforma la cadena a mayusculas
  MapPair *pair = map_search(productosPorTipo, tipo); // Busca el tipo en el mapa
  if (pair) { // Si se encuentra el tipo
    List *listaProductos = (List *)pair->value; // Obtiene la lista de productos
    Producto *producto = (Producto *)list_first(listaProductos); // Obtiene el primer producto de la lista
    printf("\nProductos de tipo %s:\n", tipo); // Imprime el tipo de producto
    while (producto) { // Mientras haya productos
      printf("============================================\n");
      printf("Nombre: %s\n", producto->nombre);
      printf("Precio de venta: %d\n", producto->precioVenta);
      producto = (Producto *)list_next(listaProductos); // Obtiene el siguiente producto de la lista
    }
  } else { // Si no se encuentra el tipo
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
      new->monto = auxProd->precioVenta * cantTotal;
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
    case 1: // Se llama a la función que corresponda
      agregarCodigo(pedidos, inventario); // Agregar por codigo
      break;
    case 2:
      agregarManual(pedidos); // Agregar manualmente
      break;
    case 0:
      break;
    default:
      printf("Opción no valida, intente de nuevo.\n");
      presioneTeclaParaContinuar();
    }
  } while (op != 0); // Se repite hasta que se ingrese 0
  printf("==============================================\n\n");
}

void eliminarProductoPedido(List *pedidos,
                            Map *inventario) // Elimina un producto del pedido
{
  int numProd; // Variable de opción
  int size = list_size(pedidos); // Tamaño del pedido
  do {
    mostrarPedido(pedidos); // Mostrar productos en el pedido
    printf("Ingrese número de producto (0 para cancelar): \n");
    scanf("%d", &numProd); // Leer opción
    if (numProd == 0) // Si la opción es 0, termina la función
      return;
    else if (numProd > size || numProd < 0) {
      printf("Opción no valida, intente de nuevo.\n");
      presioneTeclaParaContinuar();
    }
  } while (numProd > size || numProd < 0); // Itera mientras que la opción no sea valida
  int cont = 1; // Variable de contador
  Pedido *aux = list_first(pedidos); // Puntero al primer elemento del pedido
  while (cont != numProd) { // Se recorre la lista hasta el producto deseado
    aux = list_next(pedidos); // Se mueve el puntero al siguiente producto 
    cont++;
  }
  if (cont == numProd) {
    int cant; // Variable de cantidad a eliminar
    printf("Ingrese la cantidad de producto que desea eliminar: ");
    scanf("%d", &cant); // Se lee la cantidad a eliminar
    if (cant >= aux->cantidad)  // Si la cantidad a eliminar es mayor o igual a la cantidad de 
      list_popCurrent(pedidos); // producto en el pedido, se elimina el producto por completo
    else if(cant < 0) printf("Cantidad no valida\n");
    else {
      aux->cantidad -= cant; // Se actualiza la cantidad del pedido
      aux->monto = aux->producto.precioVenta * aux->cantidad; // Se actualiza el monto del pedido
    }
  }
}

void modificarPedido(List *pedidos, Map *inventario) // Modifica el pedido
{
  int op; // Variable de opción
  do { // Se repite hasta que se ingrese 0
    int size = list_size(pedidos); // Tamaño del pedido
    headerModificacionPedido(); // Imprimir header de modificación y opciones
    printf("1) Agregar producto del pedido\n"); // Agregar producto al pedido
    if (size > 0) // Si hay productos en el pedido
      printf("2) Eliminar producto del pedido\n"); // Eliminar producto del pedido
    printf("0) Volver al menú de gestión de pedido\n"); // Volver al menú de gestión de pedido
    printf("Ingresar opción: "); // Leer opción
    scanf("%d", &op); // Leer opción
    if (op == 1) // Si la opción es 1, se llama a la función que corresponda
      agregarProductoPedido(pedidos, inventario); // Agregar producto al pedido
    else if (size > 0 && op == 2) // Si hay productos en el pedido y la opción es 2, se llama a la función que correspon
      eliminarProductoPedido(pedidos, inventario); // Eliminar producto del pedido
    else if (op == 0) // Si la opción es 0, termina la función
      printf("\nVolviendo al menú de gestión de pedido\n");
    else // Si la opción no es valida, se imprime un mensaje de error
      printf("Opción no valida, intente de nuevo.\n");
    presioneTeclaParaContinuar();
  } while (op != 0);
}

void pagoEfectivo(List *pedidos) // Realiza el pago en efectivo
{
  int sumaPrecio = 0; // Variable que guarda el precio total
  Pedido *aux = list_first(pedidos); // Puntero al primer elemento del pedido
  while (aux) { // Se imprimen los productos del pedido, junto a su precio de venta y cantidad
    sumaPrecio += (aux->producto.precioVenta * aux->cantidad); // Se suman los montos de los productos
    aux = list_next(pedidos); // Se mueve el puntero al siguiente producto
  }
  int pago; // Variable que guarda el pago
  do { // Se repite hasta que el pago sea mayor o igual al precio total
    limpiarPantalla(); 
    printf("Monto a pagar: %d$\n", sumaPrecio); // Se imprime el monto a pagar
    printf("Cantidad de dinero pagada: "); // Se lee el pago
    scanf("%d", &pago); // Se lee el pago
    if (pago < sumaPrecio) { // Si el pago es menor al precio total, se imprime un mensaje de error
      printf("Faltan %d$\n", sumaPrecio - pago); // Se imprime el monto faltante
      presioneTeclaParaContinuar(); // Se presiona una tecla para continuar
    } else if (pago == sumaPrecio) // Si el pago es igual al precio total, se imprime un mensaje de confirmación
      printf("Pago exacto, no necesita vuelto\n"); 
    else // Si el pago es mayor al precio total, se imprime el vuelto
      printf("El vuelto es %d$\n", pago - sumaPrecio);
  } while (pago < sumaPrecio); // Se repite hasta que el pago sea mayor o igual al precio total
}

void eliminarPedido(List *pedidos, Map *inventario) { // Elimina el pedido
  Pedido *aux = list_first(pedidos); // Puntero al primer elemento del pedido
  while (aux) { // Se recorre la lista hasta el último elemento
    MapPair *pair = map_search(inventario, &aux->producto.codigo); // Se busca el producto en el inventario
    if (pair) { // Si se encuentra el producto en el inventario
      if (list_size(pair->value) == aux->cantidad) // Si la cantidad del producto es igual a la cantidad del pedido
        map_remove(inventario, &aux->producto.codigo); // Se elimina el producto del inventario
      while (aux->cantidad > 0) { // Se recorre la lista hasta el último elemento
        list_popBack(pair->value); // Se elimina el producto del inventario
        aux->cantidad--; // Se resta la cantidad del pedido
      }
    }
    aux = list_next(pedidos); // Se mueve el puntero al siguiente producto
    list_popBack(pedidos); // Se elimina el producto del pedido
  }
}

void procesarPago(List *pedidos, Map *inventario) // Procesa el pago
{
  int op; // Variable de opción
  do {
    mostrarPedido(pedidos);
    printf("1) Confirmar pedido\n");
    printf("0) Cancelar\n"); // Imprimir opciones
    printf("Ingrese una opción: ");
    scanf("%d", &op); // Se lee la opción
    switch (op) {
    case 1: // Confirmar pedido
      printf("Forma de pago\n");
      printf("1) Pago con tarjeta\n");
      printf("2) Pago en efectivo\n"); // Imprimir opciones
      printf("0) Cancelar Compra\n");
      printf("Ingrese método de pago: ");
      int mp; // Variable de método de pago
      scanf("%d", &mp); // Se lee el método de pago
      switch (mp) {
      case 1: // Pago con tarjeta
        printf("Pago exitoso!\n");
        printf("Gracias por su compra!\n");
        break;
      case 2: // Pago en efectivo
        pagoEfectivo(pedidos); // Se llama a la función para pagos en efectivo
        printf("Gracias por su compra!\n");
        break;
      case 0: // Cancelar compra
        printf("Compra cancelada\n"); // Se cancela la compra
        break;
      default:
        printf("Opción no valida, intente de nuevo.\n");
      }
      if (mp >= 1) { // Si los pagos se realizaron exitosamente, se eliminan los productos del inventario
        eliminarPedido(pedidos, inventario); // Funcion que elimina los productos del inventario
        op = 0; // Se ajusta la opción para terminar la función
      }
      break;
    case 0:
      printf("Volviendo al menú de gestión de pedido\n");
      break;
    }
    presioneTeclaParaContinuar();

  } while (op != 0);
}

void mostrarMenuPedidoVacio() // Muestra el menu de pedido vacio y sus opciones
{
  limpiarPantalla();
  printf("=======================================\n"); // Imprimir header
  printf("             Menú pedido\n"); 
  printf("=======================================\n");
  printf("Actualmente no hay datos de pedido\n");
  printf("Se pueden agregar productos manualmente o utilizando la 2da función "
         "del menú principal\n");
  printf("1) Modificar pedido actual\n");
  printf("0) Volver al menú principal\n");
  printf("Ingrese una opción: ");
}

void mostrarMenuPedido() // Muestra el menu de pedido y sus opciones
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
  int op; // Variable de opción
  do {
    int pedidoSize = list_size(pedidos); // Tamaño del pedido
    if (pedidoSize == 0) { // si el pedido esta vacio
      mostrarMenuPedidoVacio(); // Se llama a la función que muestra el menu de pedido vacio
      scanf("%d", &op);  // Se lee la opción
      switch (op) { // Se procesa la opción
      case 1:
        modificarPedido(pedidos, inventario); // SE llama a la funcion modificarPedido
        break;
      case 0: // Volver al menú principal
        printf("\nVolviendo al menú principal\n"); 
        break;
      default: // Si la opción no es valida, se imprime un mensaje de error
        printf("Opción no valida, intente de nuevo.\n");
        presioneTeclaParaContinuar();
      }
    } else {
      mostrarMenuPedido(); // Se muestra el menú de pedido
      scanf("%d", &op);
      switch (op) {
      case 1: // Ver datos del pedido actual
        mostrarPedido(pedidos);
        presioneTeclaParaContinuar();
        break;
      case 2: // Modificar datos del pedido actual
        modificarPedido(pedidos, inventario);
        break;
      case 3: // Procesar compra
        procesarPago(pedidos, inventario);
        break;
      case 0: // Volver al menú principal
        printf("Volviendo al menú principal\n");
        break;
      default: // Si la opción no es valida, se imprime un mensaje de error
        printf("Opción no valida, intente de nuevo.\n");
        presioneTeclaParaContinuar();
      }
    }
  } while (op != 0); // Itera mientras la opcion sea distina de 0
  return; 
}

void cargarInventario(Map *inventario) { // Carga el inventario
  FILE *archivo = fopen("data/inventario.csv", "r"); // Abre el archivo
  if (archivo == NULL) { // Si no se pudo abrir el archivo
    perror("No se pudo abrir el archivo\n");
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ','); // Lee la primera linea del archivo
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) { // Lee las lineas del archivo
    Producto *producto = crearProducto(); // Crea un producto
    producto->codigo = atoi(campos[0]); // Asigna el codigo
    strcpy(producto->nombre, campos[1]); // Asigna el nombre
    formatearCadena(producto->nombre); // Formatea el nombre
    producto->precioVenta = atoi(campos[2]);// Asigna el precio de venta
    producto->fechaV.dia = atoi(campos[3]); // Asigna el dia de vencimiento
    producto->fechaV.mes = atoi(campos[4]); // Asigna el mes de vencimiento
    producto->fechaV.year = atoi(campos[5]); // Asigna el año de vencimiento
    MapPair *pair = map_search(inventario, &producto->codigo); // Busca el producto en el inventario
    if (pair) { // Si el producto ya existe en el inventario
      List *listaProductos = (List *)pair->value; // Obtiene la lista de productos
      list_pushBack(listaProductos, producto); // Agrega el producto a la lista
    } else { // Si el producto no existe en el inventario
      List *listaProductos = list_create(); // Crea una lista de productos
      list_pushBack(listaProductos, producto); // Agrega el producto a la lista
      map_insert(inventario, &producto->codigo, // Inserta el producto en el inventario
                 listaProductos); // recordar que la key del mapa del inventario
                                  // es el codigo del producto
    }
  }
}

int main(int argc, char *argv[]) { // Funcion principal

  int opcion; // Variable de opción
  List *pedidos = list_create(); // Crea una lista de pedidos
  Map *inventario = map_create(is_equal_int); // Crea un mapa de inventario
  Map *precioProductos = map_create(is_equal_str); // Crea un mapa de precios de productos
  Map *productosPorTipo = map_create(is_equal_str); // Crea un mapa de productos por tipo
  cargarProductos(precioProductos); // Carga los precios de productos
  cargarInventario(inventario); // Cargar el inventario
  actualizarProductosPorTipo(productosPorTipo, precioProductos); // Actualiza los productos por tipo

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
  map_clean(precioProductos);
  map_clean(productosPorTipo);
  list_clean(pedidos);
  
  return 0;
}