#include "contiki.h"
#include "net/rime.h"
 #include "dev/button-sensor.h"
 #include "dev/sht11-sensor.h"
 #include "dev/leds.h"
 #include <stdio.h>
 /*
This is for the different versions of contiki
#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include "dev/sht11/sht11-sensor.h"
*/
 /*---------------------------------------------------------------------------*/
PROCESS(example_unicast_process, "Example unicast");
AUTOSTART_PROCESSES(&example_unicast_process);
/*---------------------------------------------------------------------------*/
 static struct unicast_conn uc;
 /*---------------------------------------------------------------------------*/
 //static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from){
  
  printf("broadcast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
   //linkaddr_t destination;
  rimeaddr_t destination;
   packetbuf_copyfrom("Hello", 6);
   destination.u8[0] = from->u8[0];
  destination.u8[1] = from->u8[1];
   if(!rimeaddr_cmp(&destination, &rimeaddr_node_addr)) {
      unicast_send(&uc, &destination);
  }
   
 }
 static const struct broadcast_callbacks broadcast_call = { broadcast_recv };
static struct broadcast_conn broadcast;
 /*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_unicast_process, ev, data)
{
  PROCESS_EXITHANDLER(unicast_close(&uc);)
    
  PROCESS_BEGIN();
  
  SENSORS_ACTIVATE(sht11_sensor);
  
  broadcast_open(&broadcast, 146, &broadcast_call);
  unicast_open(&uc, 130, &unicast_callbacks);
   }
   PROCESS_END();
}
/*---------------------------------------------------------------------------*/
 //********************TO BE ADDED TO UNICAST****
    /*Prints out the temperature correctly
    SENSORS_ACTIVATE(sht11_sensor);
    val = sht11_sensor.value(SHT11_SENSOR_TEMP);
    val = (-39.60 + (0.01*val));
    //printf("Temp = %d.%02u C (%d) \n", val);
    printf("Temp = %d.%02u C \n", val);
    //---------------------------------------    
     //Trying to get average-----------------
    //TODO: Loop thing, add each val and divide by no of val
 
    //Currently bust------------------------
    char temp[10];
    char words[10];
    sprintf(temp, "%d.%02u C \n", val);
    sprintf(words, "C is the temp");
    strcat(temp, words);
    */