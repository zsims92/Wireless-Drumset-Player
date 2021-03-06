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



typedef struct {
    rac_rx_cfg_t rx_cfg;
    rac_tx_cfg_t tx_cfg;
} ctx_t;
ctx_t g_ctx;


KAL_PROCESS(master_p, USER_PID(0));
KAL_PROCESS(drum_p, USER_PID(1));

int BPM = 60;
u8 beatsPerMin;
u16 curMeasure;
u8 i = 0;
bool start = false;

KAL_PROC_THREAD(drum_p, ev, evp)
{
    u8 clock_tim;
    u16 bps = 60.0/BPM * 1000;
    u16 timePer16 = bps/16;
    KAL_PROC_BEGIN();
    DPRINT("%d %d %d, left over: %d\n", BPM, bps, timePer16, bps-(16*timePer16));
    while (1) {
	if(!start){
		int x;
		x++;
		clock_tim = kal_timer_start(KAL_ETIMER,
				&drum_p, 0, timePer16);
		KAL_PROC_WAIT_EVENT_UNTIL(ev == KAL_EVT_TIMER);
		//DPRINT("Waiting\n");
	}
	else if(i < 16){
		i = i+1;
		if(curMeasure & (1 << (16-i))){
			clock_tim = kal_timer_start(KAL_ETIMER,
				&drum_p, 0, timePer16);
			DPRINT("HIT\n");
			hal_led_on(HAL_LED_RED);
			hal_io_set(MOTOR1_b);
			hal_io_clr(MOTOR1_a);
			KAL_PROC_WAIT_EVENT_UNTIL(ev == KAL_EVT_TIMER);
			hal_led_off(HAL_LED_RED);
			hal_io_set(MOTOR1_a);
			hal_io_clr(MOTOR1_b);

		}
		else{
		    DPRINT("DON'T\n");
		    clock_tim = kal_timer_start(KAL_ETIMER,
		        &drum_p, 0, timePer16);
		    KAL_PROC_WAIT_EVENT_UNTIL(ev == KAL_EVT_TIMER);
		}
	}
	else{
		i = 0;
		start = false;
	}
    }
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
		
        if (ev==RAC_EVT_RX_PKT)
        {
            u16 tmp[4];
            fg.src_addr = (u8*)tmp;
		if (!d7_get_fg(pkt,&fg))
		{
			kal_free(pkt);
			continue;
		}
		if (fg.data[0] == 0x00)
		{
			DPRINT("START ");
			start = true;
		}
		else if (fg.data[0] == 0x01)
		{
			beatsPerMin = fg.data[1];
			DPRINT("BPM = %d\n", beatsPerMin);
		}
	    else if(fg.data[0] == 0x02)
	    {
			curMeasure = fg.data[2] | (fg.data[1] << 8);
			DPRINT("First Measure %d\n", curMeasure);
	    }
		else if(fg.data[0] == 0x03)
		{
			curMeasure = fg.data[4] | (fg.data[3] << 8);
			start = true;
			i=0;
			DPRINT("New Measure %d\n", curMeasure);
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

void main(void) {

    // Open HAL layer    
    hal_open();
    // Open KAL layer
    kal_open();
    // Initialize buttons
	hal_button_open(&master_p);
    // Initialize Serial
    hal_serial_open(HAL_SERIAL_MODE_PACKET, HAL_SERIAL_BAUDRATE_115200,NULL);

    DPRINT("Receiver Ride ("__DATE__ " " __TIME__ ")\n");
    hal_io_sel_gpio(MOTOR1_a);
    hal_io_set_out(MOTOR1_a);
    hal_io_sel_gpio(MOTOR1_b);
    hal_io_set_out(MOTOR1_b);	
    hal_io_set(MOTOR1_a);
    hal_io_clr(MOTOR1_b);

    hal_led_off(HAL_LED_RED|HAL_LED_YELLOW|HAL_LED_GREEN);
    hal_led_on(HAL_LED_YELLOW);
    // Start processes
    rac_open(&master_p);
    kal_proc_start(&master_p, (evt_param_t)NULL);
    kal_proc_start(&drum_p, (evt_param_t)NULL);
    // Run scheduler
    kal_proc_schedule();
}

