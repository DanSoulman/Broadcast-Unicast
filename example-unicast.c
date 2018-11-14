#include "contiki.h"
#include "net/rime.h"
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
 /* This block of code executes when the collector receives a unicast */
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from){
 }
 //This struct lets the collector know what method to invoke when a unicast is received
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
 /*---------------------------------------------------------------------------*/
 //static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
//This is the value that will be printed out when a broadcast is received
static int print_val;
 //This array will be used to hold the moving average of the nodes temperature
static int values[4];
 /* 
This block of code is executed when a broadcast is received.
It prints out the message that the node receives from the collector
 We convert our temperature value to a string and then copy it to the
packet buffer.
Then we send a unicast to the collector by
setting our destination address to the two halves of the source address */
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from){
  
  printf("broadcast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
   //linkaddr_t destination;
  rimeaddr_t destination;
   packetbuf_copyfrom("Hello", 6);
  char str[10];
   sprintf(str, "%d", print_val);
   packetbuf_copyfrom(str, 5);
   destination.u8[0] = from->u8[0];
  destination.u8[1] = from->u8[1];
@@ -49,7 +65,7 @@ static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from){
 }
 //This struct lets the collector know what method to invoke when a broadcast is received
static const struct broadcast_callbacks broadcast_call = { broadcast_recv };
static struct broadcast_conn broadcast;
 @@ -60,36 +76,76 @@ PROCESS_THREAD(example_unicast_process, ev, data)
  PROCESS_EXITHANDLER(unicast_close(&uc);)
    
  PROCESS_BEGIN();
  
   //Activates the sensor so that we can read the temperature from it
  SENSORS_ACTIVATE(sht11_sensor);
  
   /*
  We open a broadcast connection on band 146
  and open a unicast connection on band 140
  */ 
  broadcast_open(&broadcast, 146, &broadcast_call);
  unicast_open(&uc, 130, &unicast_callbacks);
   unicast_open(&uc, 140, &unicast_callbacks);
   //This will be used as the counter for our for loop
  int i = 0;
   /* 
  We populate the array that holds the moving
  average for the temperature with 0s
  */
  for(i = 0; i < 4; i++){
	  values[i] = 0;
  }
   PROCESS_END();
}
/*---------------------------------------------------------------------------*/
  /*
  This block of code will run until the mote is terminated
  It periodically takes the temperature and stores it
  and then calculates the new moving average
  */
  while(1) {
     static struct etimer et;
    
    etimer_set(&et, CLOCK_SECOND);
    
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
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
    int store = print_val = sht11_sensor.value(SHT11_SENSOR_TEMP);
     /*
    This block of code pushes out the first value
    read in out of the array and compresses it leaving 
    the last index free to add in our new value
    */
    
     for(i = 0; i < 4; i++){
	    if((i + 1) < 4){
	    values[i] = values[i + 1];
      }
    }
     //We set the last index to our new value
    values[3] = store;
     store = 0;
     /*
    This block of the code gets the sum
    of the total array
    */
	  for(i = 0; i < 4; i++){
		  store = store + values[i];
	  }
     //We get the average by diving the sum by four
    print_val = store / 4;
   }
   PROCESS_END();
}
/*---------------------------------------------------------------------------*/