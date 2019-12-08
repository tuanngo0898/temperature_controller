/*
 * timer.c
 *
 *  Created on: Sep 6, 2019
 *      Author: 16138
 */
#include "timer.h"

#include "tim.h"
#include "Src/user_code/user_define.h"


static callback_t callbacks[MAX_CALLBACK_FUNC];

timer_ID_t timer_register_callback(timer_callback_func_t timer_callback_func, uint16_t period_ms, uint8_t* context, timer_mode_t mode){
	for(uint8_t i = 0; i < MAX_CALLBACK_FUNC; i++){
		if(callbacks[i].timer_callback_func == 0){
			callbacks[i].timer_callback_func = timer_callback_func;
			callbacks[i].period_ms = period_ms;
			callbacks[i].context = context;
			callbacks[i].mode = mode;
			callbacks[i].cnt = period_ms;
			callbacks[i].id = i;
			return i;
		}
	}
	return -1;
}

void timer_unregister_callback(timer_ID_t id){
	callbacks[id].timer_callback_func = 0;
}

uint64_t milis(){
	return HAL_GetTick();
}

void user_systick()
{
	for(uint8_t i = 0; i < MAX_CALLBACK_FUNC; i++){
		if(callbacks[i].timer_callback_func != 0){
			callbacks[i].cnt--;
			if(callbacks[i].cnt == 0){
				callbacks[i].timer_callback_func(callbacks[i].context);
				callbacks[i].cnt = callbacks[i].period_ms;
				if(callbacks[i].mode == TIMER_MODE_ONE_SHOT) callbacks[i].timer_callback_func = 0;
			}
		}
	}
}
