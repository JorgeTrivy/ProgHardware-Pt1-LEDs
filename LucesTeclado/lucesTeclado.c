#include <linux/init.h>
#include <linux/kd.h> /*KDSETLED, activacion de leds */
#include <linux/module.h>
#include <linux/tty.h> //Estructura TTY
#include <linux/vt.h> //MAX_NR_CONSOLES
#include <linux/vt_kern.h> //fg_console
#include <linux/console_struct.h> //vc_cons

#define BLINK_DELAY HZ / 5
#define ALL_LEDS_ON 0x07
#define RESTORE_LEDS 0xFF

static unsigned long estadoLed = 0; //Almacena el estado de los LEDs del teclado
static struct tty_driver *driverTty; //Puntero a la estructura del contrololador TTY
static struct timer_list temp; //Estructura del temporizador para controlar parpade


/*Proboca el parpadeo de los LED del teclado*/
static void my_timer_func(struct timer_list *unused){
	//Acceso al terminal asociado a la consola virtual en primer plano para un acceso al driveer de terminal
	struct tty_struct *t = vc_cons[fg_console].d->port.tty;

	//Enciende y apaga todos los LEDs
	if (estadoLed == ALL_LEDS_ON){
 		estadoLed = RESTORE_LEDS;
 	}else{
 		estadoLed = ALL_LEDS_ON;
	}
	
 	(driverTty->ops->ioctl)(t, KDSETLED, estadoLed); //Comando para activar o desactivar LEDs

	//Establece proximo tiempo de expiracion del temporizador
 	temp.expires = jiffies + BLINK_DELAY;
 	add_timer(&temp); //Agregar temporizador
}

//Funcion inicializacion
static int __init lucesTeclado_init(void){
 	int i;

 	printk(KERN_INFO "Driver Cargado\n");
 	printk(KERN_INFO "Numero de Consolas Activas %x\n", fg_console);
 	
 	for (i = 0; i < MAX_NR_CONSOLES; i++){
 		if (!vc_cons[i].d)
 			break;
 		printk(KERN_INFO "----Consola[%i/%i] #%i, tty %p\n", i, MAX_NR_CONSOLES,
 		vc_cons[i].d->vc_num, (void *)vc_cons[i].d->port.tty);
 	}
 
 	printk(KERN_INFO "Escaneo de consolas finalizado\n");
 	driverTty = vc_cons[fg_console].d->port.tty->driver; //Obtiene controlador del TTY
 	printk(KERN_INFO "Nombre del controlador TTY %s\n", driverTty->driver_name);

 	/*Configuración del temporizador en caso de que sea la primera vez*/
 	timer_setup(&temp, my_timer_func, 0);
 	//Jiffies= unidad de tiempo que representa numero de interrupciones del reloj del sistema desde que se inició.
 	//BLINK_DELAY= constante que representa el retardo entre cada parpadeo del teclado
 	temp.expires = jiffies + BLINK_DELAY;
	add_timer(&temp);

 	return 0;
}

static void __exit lucesTeclado_cleanup(void){
	printk(KERN_INFO"Descargando driver\n");
	
	//Restaura el estado predeterminado de los LEDs
 	del_timer(&temp);
 	(driverTty->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
}

module_init(lucesTeclado_init);
module_exit(lucesTeclado_cleanup);
            
MODULE_DESCRIPTION("Parpadeo de LEDs del teclado");
MODULE_LICENSE("GPL");

