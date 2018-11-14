#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/sht11/sht11-sensor.h"
#include "dev/button-sensor.h"
 #include "dev/leds.h"
 #include <stdio.h>
/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  printf("broadcast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
 /*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
  static struct etimer et;
   PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
   PROCESS_BEGIN();
   broadcast_open(&broadcast, 129, &broadcast_call);
   int val = 0;             
  float cel = 0;
  while(1) {
     /* Delay 2-4 seconds */
    etimer_set(&et, CLOCK_SECOND* 4); //+ random_rand() % (CLOCK_SECOND * 4));
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
     //Prints out the temperature correctly
    SENSORS_ACTIVATE(sht11_sensor);
    val = sht11_sensor.value(SHT11_SENSOR_TEMP);
    val = (-39.60 + (0.01*val));
    //printf("Temp = %d.%02u C (%d) \n", val);
    printf("Temp = %d.%02u C \n", val);
    //---------------------------------------    
 
    //Currently bust------------------------
    char temp[10];
    char words[10];
    sprintf(temp, "%d.%02u C \n", val);
    sprintf(words, "C is the temp");
    strcat(temp, words);
    //--------------------------------------
    
    //Sends the broadcast
    packetbuf_copyfrom("HELP", 5);
    //packetbud_copyfrom(val);
    broadcast_send(&broadcast);
    //printf("broadcast message sent\n");
  }
   PROCESS_END();
}
/*---------------------------------------------------------------------------*/ 