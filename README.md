# Parpadeo de LEDs de teclado
La función de este driver consiste en producir un parpadeo de las luces del teclado del pc.

Al principio del programa se encuentran las variables que se usarán para saber el estado de los LEDs, activados o no, la estructura en donde se guardará el controlador TTY, el cuál es necesario para activar o desactivar estos LEDs, y la variable con la estructura del temporizador, indicando cada cuanto se encenderán y apagarán.

Una vez iniciado, se obtienen las consolas activas y el controlador TTY y se configuran los tiempos de parpadeo a partir de las siguientes variables:
  'jiffies', que representa el número de interrupciones de reloj del sistema desde el inicio.
  'BLINK_DELAY', que es una constante que representa el retardo entre cada parpadeo del teclado.

Hecho esto, se llama a la función 'my_time_func', que será la que realice el parpadeo. Comprueba si el estado actual de los LEDs y efectúa el efecto contario, apagandolos todos o encendiendolos. Ésta acción se realizará con una función situada en la estructura del controlador TTY, 'driverTty->ops->ioctl', en donde se le deberá pasar, nuestra estructura TTY, obtenida justo al principio de la función, el KDSETLED, para la activación, y el estado del led que queramos. Por último, renovamos el tiempo de expiración.

Para terminar, la función de descarga del driver tendrá una línea para limpiar y otra para dejar en su estado normal los LEDs del teclado.
