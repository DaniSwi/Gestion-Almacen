# Proyecto grupo Xes

## El programa tiene por finalidad ayudar Pymes chilenas con el creciente problema de gestionar sus negocios.

### Creado por Daniel Cornejo, Julián Guerrero, Benjamín Soto y Diego Zúñiga.


### TDAs
En la carpeta `tdas` se encuentran implementados distintos TDAs que se pudieron llegar a utilizar (list.h y map.h). 

Para este programa se utilizaron principal y únicamente listas y mapas.

## Cosas importantes a saber!

### Sobre los mapas
El programa cuenta con 3 mapas, inventario, precioProductos y productosPorTipo
#### **Inventario**:
El mapa de inventario tiene por **key** el código del producto y los **value** del mapa son listas de productos que contienen ese código. Entonces, el mapa almacena listas de productos con códigos iguales.
#### **precioProductos**:
El mapa de precioProductos tiene por **key** el nombre del producto y los **value** que guarda el mapa son los productos. Este mapa es útil en conjunto con el mapa del inventario ya que se complementan, por ejemplo, si no sabes el código de un producto, pero si su nombre, se hace una búsqueda en el precioProductos con el nombre del producto y se obtiene toda la información.
#### **productosPorTipo**:
El mapa de productosPoripo tiene por **key** el tipo de producto y el **value** que guarda es una lista de productos, en el que están todos los productos que contienen ese tipo de producto.

### Sobre las listas
Como se vio anteriormente, algunos mapas contienen como **value** listas de productos, igualmente, en el programa se incluye una lista, listaPedidos

#### **listaPedidos**:
La lista de pedidos se encarga de almacenar pedidos que realiza un cliente, normalmente esta lista se pobla con pedidos están en un CSV, o en la función de pagar. La lista almacena pedidos y es práctica para el programa.

# ¿Cómo ejecutar el código?

Para ejecutar el main primero se coloca este comando en la pestaña de **Shell**
````
gcc tdas/*.c main.c -Wno-unused-result -o main
````

Y luego ejecutar:
````
./main
````

### Instrucciones del codigo 

Se encuentran `10` funciones principales las cuales se encuentran realcianadas con la gestión de un almacén.

### Función 1: `agregarProductoInventarioUser(inventario)`

**Descripción**:
Permite al usuario agregar manualmente un producto al inventario. La función solicita al usuario la información necesaria para crear un nuevo producto, como el código, nombre, tipo, cantidad y precio, y luego añade este producto al inventario.

**Entradas**:
inventario: Mapa que representa el inventario actual de productos.

**Salidas**:
Actualización del inventario con el nuevo producto añadido.

### Función 2: `cargarPedidosCSV(pedidos, inventario, precioProductos)`

**Descripción**:
Carga los pedidos desde un archivo CSV y los añade a la lista de pedidos. La función lee el archivo CSV, extrae la información de los pedidos y actualiza el inventario y los precios de los productos en función de los pedidos realizados.

**Entradas**:
pedidos: Lista donde se almacenarán los pedidos cargados.
inventario: Mapa que representa el inventario actual de productos.
precioProductos: Mapa que almacena los precios de los productos.

**Salidas**:
Actualización de la lista de pedidos, inventario y precios de productos.

### Función 3: `visualizacionInventario(inventario)`

**Descripción**:
Muestra una visualización detallada del inventario actual. La función recorre el mapa del inventario y presenta la información de cada producto, como el código, nombre, tipo, cantidad y precio.

**Entradas**:
inventario: Mapa que representa el inventario actual de productos.

**Salidas**:
Una visualización detallada del inventario en la consola.

### Función 4: `cargarCargaDesdeCSV(inventario, precioProductos)`

**Descripción**:
Carga la información de los productos desde un archivo CSV y actualiza el inventario y los precios de los productos. La función lee el archivo CSV y actualiza el inventario y los precios en función de la información contenida en el archivo.

**Entradas**:
inventario: Mapa que representa el inventario actual de productos.
precioProductos: Mapa que almacena los precios de los productos.

**Salidas**:
Actualización del inventario y los precios de los productos.

### Función 5: `exportarInventario(inventario)`

**Descripción**:
Exporta el inventario actual a un archivo CSV. La función toma la información del inventario y la escribe en un archivo CSV para su almacenamiento o análisis posterior.

**Entradas**:
inventario: Mapa que representa el inventario actual de productos.

**Salidas**:
Archivo CSV que contiene la información del inventario.

### Función 6: `mostrarProductos(precioProductos)`

**Descripción**:
Muestra una lista de todos los productos y sus precios actuales. La función recorre el mapa de precios de productos y presenta la información de cada producto y su precio correspondiente.

**Entradas**:
precioProductos: Mapa que almacena los precios de los productos.

**Salidas**:
Una visualización de los productos y sus precios en la consola.

### Función 7: `cuadrarInventario(inventario, precioProductos)`

**Descripción**:
Revisa y ajusta el inventario y los precios de los productos para asegurar que sean consistentes. La función puede realizar verificaciones y ajustes necesarios para que el inventario y los precios reflejen correctamente la realidad.

**Entradas**:
inventario: Mapa que representa el inventario actual de productos.
precioProductos: Mapa que almacena los precios de los productos.

**Salidas**:
Actualización del inventario y los precios de los productos si se encuentran discrepancias.

### Función 8: `buscarProductoPorCodigo(inventario)`

**Descripción**:
Permite al usuario buscar un producto específico en el inventario mediante su código. La función solicita al usuario el código del producto y luego busca y muestra la información del producto correspondiente.

**Entradas**:
inventario: Mapa que representa el inventario actual de productos.

**Salidas**:
Información del producto encontrado, si existe, mostrada en la consola.

### Función 9: `buscarProductosPorTipo(productosPorTipo)`

**Descripción**:
Permite al usuario buscar productos específicos en el inventario mediante su tipo. La función solicita al usuario el tipo de producto y luego busca y muestra la información de todos los productos que pertenecen a ese tipo.

**Entradas**:
productosPorTipo: Mapa que clasifica los productos según su tipo.

**Salidas**:
Información de los productos encontrados, si existen, mostrada en la consola.

### Función 10: `procesarVenta(pedidos, inventario)`

**Descripción**:
Procesa una venta de productos. La función actualiza el inventario restando la cantidad vendida de los productos correspondientes y añade el pedido a la lista de pedidos.

**Entradas**:
pedidos: Lista donde se almacenarán los pedidos procesados.
inventario: Mapa que representa el inventario actual de productos.

**Salidas**:
Actualización del inventario con las cantidades vendidas y adición del pedido a la lista de pedidos.

## Cosas utiles a saber del codigo: 

### Estructura del Código

**Inventario**:
Representado como un mapa, donde cada entrada corresponde a un producto.
Cada producto tiene atributos como código, nombre, tipo, cantidad y precio.

**Pedidos**:
Manejados como una lista que contiene la información de cada pedido realizado.

**Precios de Productos**:
Representados como un mapa donde cada clave es el código del producto y el valor es su precio actual.

### Funcionalidades Clave

`Agregar Productos`
Los productos pueden ser agregados manualmente por el usuario o cargados desde un archivo CSV.
Es importante que el código del producto sea único para evitar conflictos en el inventario.

`Visualización del Inventario`
Permite ver todos los productos disponibles con sus atributos.
Es útil para obtener una vista general del estado actual del inventario.

`Carga y Exportación de Datos`
Los productos y pedidos pueden ser cargados desde archivos CSV para facilitar la gestión de grandes volúmenes de datos.
El inventario también puede ser exportado a un archivo CSV para su almacenamiento o análisis posterior.

`Búsqueda y Filtrado`
Los productos pueden ser buscados por su código o tipo.
Estas funcionalidades son útiles para encontrar rápidamente productos específicos o ver categorías de productos.

`Procesamiento de Ventas`
Al procesar una venta, la cantidad de los productos vendidos se actualiza en el inventario.
Se asegura de que no se vendan más productos de los que están disponibles.

`Cuadratura del Inventario`
Función para revisar y ajustar el inventario y los precios de los productos.
Ayuda a mantener la coherencia y exactitud de los datos.

### Consideraciones Importantes

* Manejo de Errores:
Archivos CSV mal formateados, códigos de productos duplicados o cantidades inválidas pueden generar errores en el código.

* Actualización y Sincronización:
Asegurarse de que las actualizaciones en el inventario se reflejen correctamente en los precios y pedidos.
Sincronizar cambios realizados manualmente con los datos cargados desde archivos CSV.

* Interfaz de Usuario:
Las funciones que interactúan con el usuario tienen una interfaz clara y fácil de usar.
Las solicitudes de entrada deben ser manejadas cuidadosamente para evitar errores.

* Escalabilidad:
Se considera la escalabilidad del código para manejar un gran número de productos y pedidos.

* Documentación:
El código se encuentra comentadp en cada función con descripciones claras de su propósito, entradas y salidas.
Ayuda a otros a entender y usar el código correctamente.

* Validación de Datos:
Validar las entradas del usuario y los datos de los archivos CSV.
Previene errores y asegura la calidad de los datos.

* Pruebas:
Se realizaron pruebas exhaustivas para asegurar que todas las funciones se comporten correctamente.

