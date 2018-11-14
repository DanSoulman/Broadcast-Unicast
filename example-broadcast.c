#include "contiki.h"
 #include "net/rime/rime.h"
 #include "random.h"
 #include "dev/sht11/sht11-sensor.h"
 #include "dev/button-sensor.h"
 
 #include "dev/leds.h"
 #include <stdio.h>
/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
PROCESS(example_broadcast_process, "Daniel Coleman Broadcast Example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/
 //when the collector receives a broadcast prints who its from
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  printf("broadcast message received from %d.%d: '%s'\n",
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
  printf("Received Unicast from address: %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}
 //This struct tells the collector what to run on recieval of broadcast
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
 //This struct tells the collector what to run on recieval of unicast
static const struct unicast_callbacks unicast_callbacks = { recv_uc };
static struct unicast_conn uc;
 /*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
  //Makes Timer used in while loop
  static struct etimer et;
   PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
   PROCESS_BEGIN();
   //Opens broadcast connection on 146 and unicast on 130 
  broadcast_open(&broadcast, 146, &broadcast_call);
  unicast_open(&uc, 130, &unicast_callbacks); 
  
 //int val = 0;             
 //float cel = 0;
 while(1) {
  //int val = 0;             
  //float cel = 0;
  while(1) {
     /* Delay 2-4 seconds */
    /* Delay 4 seconds */
    etimer_set(&et, CLOCK_SECOND* 4); //+ random_rand() % (CLOCK_SECOND * 4));
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
@@ -76,10 +70,10 @@ PROCESS_THREAD(example_broadcast_process, ev, data)
     
    //Sends the broadcast
    packetbuf_copyfrom("HELP", 5);
    //packetbud_copyfrom(val);
    packetbuf_copyfrom("Return Temperature", 19);
    
    broadcast_send(&broadcast);
    //printf("broadcast message sent\n");
    
  }
   PROCESS_END();
