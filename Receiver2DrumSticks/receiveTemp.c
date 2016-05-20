

#include <stdio.h>
#include <stdlib.h>

#include "hal.h"
#include "hal_map_p.h"
#include "kal.h"
#include "rac.h"
#include "d7.h"



#if 1
#define DPRINT(...)      hal_dbg_print(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

#define MOTOR1_a HAL_GPIO5
#define MOTOR1_b HAL_GPIO4
#define HAL_ISR_MOTOR1_a HAL_ISR_P1_1
#define HAL_ISR_MOTOR1_b HAL_ISR_P1_0
HAL_IO_DEFINE(MOTOR1_a, 1, 1)
HAL_IO_DEFINE(MOTOR1_b, 1, 0)

#define MOTOR2_a HAL_ADIO0
#define MOTOR2_b HAL_ADIO1
#define HAL_ISR_MOTOR2_a HAL_ISR_P2_5
#define HAL_ISR_MOTOR2_b HAL_ISR_P2_4
HAL_IO_DEFINE(MOTOR2_a, 2, 5)
HAL_IO_DEFINE(MOTOR2_b, 2, 4)


#define FAST_HIT_DELAY 60
#define SONG_SIZE      256
#define BPM            4

typedef struct {
    rac_rx_cfg_t rx_cfg;
    rac_tx_cfg_t tx_cfg;
} ctx_t;
ctx_t g_ctx;


//structure holding song details
typedef struct song_detail_T 
{
    u8 beats_per_minute ; 
    u8 beats_per_measure;
    u8 current_measure  ;  //keeps track of current measure
    u8 measure_count    ;  //tracking total measures 
} song_detail_T;  

song_detail_T song_detail; 


typedef struct measure_T {
    u8   beat_data[BPM];
    u32  measure_number;
    u8   empty_measure;
} measure_T;

measure_T song_stack[SONG_SIZE];  //frame buffer for the song. 





KAL_PROCESS(master_p, USER_PID(0));
KAL_PROCESS(drum_p,   USER_PID(1));

static void fastHit();

static void delay(u32 time);



//global inits
u8 hit_motor        = 0x00;  //defines which motor to hit next. Starting with motor 0


//u8 test[4];
//measure_T testing; 



//process to play measures
KAL_PROC_THREAD(drum_p, ev, evp)
{
    u8 clock_tim;
    static u8 tim = KAL_NO_TIMER;

    u8 bps       = (int)( ( (double)song_detail.beats_per_minute/ 60.0  )  * 1000.0 );

    //u8 bps       = 100;

    u8 currrent_beat          = 0x00;
    u8 currrent_beat_data     = 0x00;
    u8 sub_beat               = 0x00;
    
    u8 release_flag           = 1;

    u8 mask                   = 0x80; 

    measure_T temp; 


    //DPRINT("Playing measure:%d\n", current_measure );

    

    KAL_PROC_BEGIN();

    while(1 & release_flag)
    {
        temp = song_stack[song_detail.current_measure]; 
        
        //DPRINT("Measure Number: %d\n", current_measure );

        temp = song_stack[song_detail.current_measure]; 

        while(currrent_beat < song_detail.beats_per_measure)
        {
            currrent_beat_data = temp.beat_data[currrent_beat];

            DPRINT("BPS: %d Measure: %d Beat %d data: %X\n", bps, song_detail.current_measure, currrent_beat, currrent_beat_data);
            
            while (sub_beat < 8) 
            {

                delay(bps);

                //hal_dbg_print("Sending Message\n");
                
                if( mask & (currrent_beat_data << (sub_beat)) ) //will only hit if there is stored a 1
                {
                    fastHit(); //hit the motor
                    
                    DPRINT("Hit\n");
                    
                }

                //DPRINT("Sub_beat Count: %d\n", sub_beat);
                sub_beat++; //go to the next 32nd note
            }

            sub_beat = 0;
            currrent_beat_data =0;
            currrent_beat ++;
        } 

        
        //DPRINT("Measure Count: %d\n\n", current_measure);

        if(song_detail.current_measure >= song_detail.measure_count)
        {
            song_detail.current_measure --; //reset the current_measure 

             //release_flag = 0; // have some logic that so it knows when there is an end of the song. 
        }
    
        song_detail.current_measure += 1; //Probs should increment to next measure about to being played
        currrent_beat = 0;
        

        

    }

    DPRINT("Ending Process\n\n\n");
    KAL_PROC_END();
}
KAL_PROC_THREAD(master_p, ev, evp)
{
    static u8 tim = KAL_NO_TIMER;
    rac_rx_res_t* pkt;
    static fg_t fg;

    KAL_PROC_EXITHANDLER(
        kal_timer_stop(tim);
        tim = KAL_NO_TIMER;
        KAL_PROC_EXIT();
    )

    pkt = (rac_rx_res_t*)evp.msg;
    KAL_PROC_BEGIN();
    g_ctx.rx_cfg         = RX_CONTINUOUS_CFG;
    // start Rx
    rac_rx_start(&g_ctx.rx_cfg);

    while (1)
    {
        KAL_PROC_WAIT_EVENT();
	    hal_led_off(HAL_LED_GREEN);
        if (ev==RAC_EVT_RX_PKT)
        {
           u16 tmp[4];
           fg.src_addr = (u8*)tmp;
           if (!d7_get_fg(pkt,&fg))
           {
               kal_free(pkt);
               continue;
           }
            
           if (fg.data[0] == 0x4D)
           {
                DPRINT("PONG\n");
           }
           else if (fg.data[0] == 0x51)  //Strike Left Side
           {
                DPRINT("STRIKING\n");
            	hal_led_on(HAL_LED_GREEN);
            	hal_io_set(MOTOR1_b);
            	hal_io_clr(MOTOR1_a);
           }
           else if(fg.data[0] == 0x52)   //Strike Right Side
           {
            	DPRINT("STRIKING\n");
            	hal_led_on(HAL_LED_GREEN);
            	hal_io_set(MOTOR2_b);
            	hal_io_clr(MOTOR2_a);
           }
           else if(fg.data[0] == 0x53)   //Load measure packet
           {
                //add packet to stack
           }
           else if(fg.data[0] == 0x4C)    // Packet recieved but don't do anything
           {
            	DPRINT("Packet Received\n");
	       }
           else
           {
                DPRINT("Unknown CMD:%#x\n",fg.data[0]);
           }
           kal_free(pkt);
        }
    }

    KAL_PROC_END();
}

static void fastHit()
{
        u8 clock_tim;

        clock_tim = kal_timer_start(KAL_ETIMER,
        &drum_p, 0, FAST_HIT_DELAY);

        if(hit_motor)
        {
            hal_io_set(MOTOR1_a);
            hal_io_clr(MOTOR1_b);
            delay(500);
            hal_io_set(MOTOR1_b);
            hal_io_clr(MOTOR1_a);
            hit_motor = ~hit_motor;
        }
        else
        {
            hal_io_set(MOTOR2_a);
            hal_io_clr(MOTOR2_b);
            delay(500);
            hal_io_set(MOTOR2_b);
            hal_io_clr(MOTOR2_a);
            hit_motor = ~hit_motor;
        }


        hal_led_off(HAL_LED_RED|HAL_LED_YELLOW|HAL_LED_GREEN);
        hal_led_on(HAL_LED_YELLOW);

        delay(25);

        hal_led_on(HAL_LED_RED|HAL_LED_YELLOW|HAL_LED_GREEN);
        hal_led_off(HAL_LED_YELLOW);

        delay(25);

        hal_led_off(HAL_LED_RED|HAL_LED_YELLOW|HAL_LED_GREEN);
        hal_led_on(HAL_LED_YELLOW);
}

static void delay(u32 time)
{
    u32 i = 0;

    for(i = 0 ; i < 10000*time; i++)
    {
        __asm__("nop");
    }

    // clock_tim = kal_timer_start(KAL_ETIMER,
    //                             &drum_p, 0, 250);

    // KAL_PROC_WAIT_EVENT_UNTIL(ev == KAL_EVT_TIMER);

}


void main(void) {

    // Open HAL layer    
    hal_open();
    // Open KAL layer
    kal_open();
    // Initialize buttons
	hal_button_open(&master_p);
    // Initialize Serial
    hal_serial_open(HAL_SERIAL_MODE_PACKET, HAL_SERIAL_BAUDRATE_115200,NULL);

    DPRINT("Receiver Cymbals ("__DATE__ " " __TIME__ ")\n");
    hal_io_sel_gpio(MOTOR1_a);
    hal_io_set_out(MOTOR1_a);
    hal_io_sel_gpio(MOTOR1_b);
    hal_io_set_out(MOTOR1_b);	
    hal_io_set(MOTOR1_a);
    hal_io_clr(MOTOR1_b);

    hal_io_sel_gpio(MOTOR2_a);
    hal_io_set_out(MOTOR2_a);
    hal_io_sel_gpio(MOTOR2_b);
    hal_io_set_out(MOTOR2_b);	
    hal_io_set(MOTOR2_a);
    hal_io_clr(MOTOR2_b);

    hal_led_off(HAL_LED_RED|HAL_LED_YELLOW|HAL_LED_GREEN);
    hal_led_on(HAL_LED_YELLOW);

    //setup dumby song


    song_detail.beats_per_minute    = 1500; //Must set song details before anything else.
    song_detail.beats_per_measure   = BPM; 
    song_detail.current_measure     = 0;
    song_detail.measure_count       = 3;






    song_stack[0].beat_data[0]      = 0x80;
    song_stack[0].beat_data[1]      = 0x88;
    song_stack[0].beat_data[2]      = 0xAA;
    song_stack[0].beat_data[3]      = 0xFF;
    song_stack[0].measure_number    = 0x0;
    song_stack[0].empty_measure     = 0x0;

    // testing.beat_data[0]      = 0x80;
    // testing.beat_data[1]      = 0x88;
    // testing.beat_data[2]      = 0xAA;
    // testing.beat_data[3]      = 0xFF;
    // measure_T testing = song_stack[0];


    // DPRINT("Data: %X\n",     song_stack[0].beat_data[0]);
    // DPRINT("Data: %X\n",     song_stack[1].beat_data[1]);
    // DPRINT("Data: %X\n",     song_stack[2].beat_data[2]);
    // DPRINT("Data: %X\n\n\n", song_stack[3].beat_data[3]);

    // DPRINT("Data: %X\n",     testing.beat_data[0]);
    // DPRINT("Data: %X\n",     testing.beat_data[1]);
    // DPRINT("Data: %X\n",     testing.beat_data[2]);
    // DPRINT("Data: %X\n\n\n", testing.beat_data[3]);


    song_stack[1].beat_data[0]      = 0x80;
    song_stack[1].beat_data[1]      = 0x00;
    song_stack[1].beat_data[2]      = 0x00;
    song_stack[1].beat_data[3]      = 0x00;
    song_stack[1].measure_number    = 0x1;
    song_stack[1].empty_measure     = 0x0;

    song_stack[2].beat_data[0]      = 0x00;
    song_stack[2].beat_data[1]      = 0x00;
    song_stack[2].beat_data[2]      = 0xAA;
    song_stack[2].beat_data[3]      = 0x00;
    song_stack[2].measure_number    = 0x2;
    song_stack[2].empty_measure     = 0x0;
 
    song_stack[3].beat_data[0]      = 0xAA;
    song_stack[3].beat_data[1]      = 0xBB;
    song_stack[3].beat_data[2]      = 0xCC;
    song_stack[3].beat_data[3]      = 0xDD;
    song_stack[3].measure_number    = 0x3;
    song_stack[3].empty_measure     = 0x0;

    


    // Start processes
    rac_open(&master_p);
    kal_proc_start(&master_p, (evt_param_t)NULL);
    kal_proc_start(&drum_p, (evt_param_t)NULL);

    // Run scheduler
    kal_proc_schedule();
}

