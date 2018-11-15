//unicast Dan Coleman

/*
#include "contiki.h"
#include "net/rime.h"
#include "dev/button-sensor.h"
#include "dev/sht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
 */
//This is for the different versions of contiki
#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include "dev/sht11/sht11-sensor.h"

 /*---------------------------------------------------------------------------*/
PROCESS(example_unicast_process, "Example unicast");
AUTOSTART_PROCESSES(&example_unicast_process);
/*---------------------------------------------------------------------------*/
 // runs when the collector receives a unicast 
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from){
 }
 //lets collector know what to do when a unicast is received
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
 /*---------------------------------------------------------------------------*/

static int print_val;

//Will hold values of temp 
static int values[5];

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from){
  printf("broadcast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
  
  linkaddr_t destination;
  //rimeaddr_t destination;
  
  //Says hello to which node it gets message from. TODO: Check this 
  packetbuf_copyfrom("Hello", 6);
  char str[10];
  sprintf(str, "%d", print_val);
  packetbuf_copyfrom(str, 5);
  destination.u8[0] = from->u8[0];
  destination.u8[1] = from->u8[1];
 if(!linkaddr_cmp(&destination, &linkaddr_node_addr)) 
    {
        unicast_send(&uc, &destination);
    }
 }



 //lets collector know which function to do when broadcast is received
 static const struct broadcast_callbacks broadcast_call = { broadcast_recv };
 static struct broadcast_conn broadcast;




  
  PROCESS_THREAD(example_unicast_process, ev, data){

  PROCESS_EXITHANDLER(unicast_close(&uc);)
  PROCESS_BEGIN();
  
  SENSORS_ACTIVATE(sht11_sensor);
  
  broadcast_open(&broadcast, 146, &broadcast_call);
  unicast_open(&uc, 140, &unicast_callbacks);
   
   //counter for loop
  int i = 0;

  // stores each value for average
  for(i = 0; i < 5; i++){
	  values[i] = 0;
  }
/*---------------------------------------------------------------------------*/
  
  while(1) {
     static struct etimer et;
     linkaddr_t addr;
     etimer_set(&et, CLOCK_SECOND);
     
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

     //Prints out the temperature correctly
     SENSORS_ACTIVATE(sht11_sensor);
     int val = sht11_sensor.value(SHT11_SENSOR_TEMP);
     print_val = (-39.60 + (0.01*val));
     printf("Temp = %d.%02u C \n", print_val);
    
      for(i = 0; i < 5; i++){
	      if((i + 1) < 5){
	      values[i] = values[i + 1];
      }
    }

    values[4] = val;
    val = 0;
    //Gets sum 
	  for(i = 0; i < 5; i++){
		  val = val + values[i];
	  }
     //We get the average by diving the store by five
    print_val = val / 5;
   }
   PROCESS_END();
}
/*---------------------------------------------------------------------------*/