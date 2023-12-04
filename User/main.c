#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/LCD/lcd.h"
#include "./CMSIS/DSP/Include/arm_math.h"
#include "./BSP/TIMER/gtim.h"
#include "tcp.h"
#include "touch.h"
#include "lcd.h"
#include "./BSP/LED/led.h"

extern uint16_t adc_buf[1]; 				// ADC DMA BUF
extern uint8_t g_adc_dma_sta;
extern TIM_HandleTypeDef g_timx_handle;

int current_time=0;							//歌曲时间 单位:us
 
arm_cfft_radix4_instance_f32 scfft;
extern float fft_in[1024 * 2];				//FFT输入数组
float fft_out[1024];						//当前次的FFT结果

void search_notes();						//匹配结果中的音符

extern int cnt;								//计数器

extern int bar_heights[21];
extern int note_stamp[14];

extern int state; 							//当前状态，暂停/开始

int main(void){
    HAL_Init();								//初始化HAL库
    sys_stm32_clock_init(RCC_PLL_MUL9);		//初始化时钟
    delay_init(72); 						//初始化延时
	usart_init(115200); 					//初始化串口
	lcd_init(); 							//初始化LCD显示屏
    led_init();                     	    /* 初始化LED */

	lcd_clear(BLACK);
	//lcd_scale_debug();
	lcd_sections_init();
	lcd_spectrum_init();
	lcd_stave_init();
	lcd_buttons_init();
	lcd_logo_init();
	lcd_char_init();
	lcd_note_init();
	stop();
	
	ctp_init();
	
	//connect_wifi();
	
	gtim_timx_int_init(125  - 1, 36 - 1); 			// 1Mhz的计数频率，计数125-21次为125-21us
	
	adc_dma_init((uint32_t)&adc_buf);		 		//初始化ADC-DMA采集
	arm_cfft_radix4_init_f32(&scfft, 1024, 0, 1);	//初始化scfft结构体，设定FFT相关参数
	
	int spectrum_height[21]; 						//频谱条动画缓冲
	float bar_fragments[21]; 						//频谱条动画缓冲
	float bar_diff[21]; 							//频谱条动画缓冲
	
    while(1){ 
		if(cnt<1024){
			//动画显示
			if(cnt<21*48){							//频谱的动画 0~209ticks
				int index=cnt%21;					//当前刷新的频谱条序号
				int process=cnt/21;					//当前刷新进度(0~10)
				int current_height=bar_heights[index];	//当前高度
				if(process==0){
					int height=0;						//决定高度的主要因素（本条频率）
					for(int i=3*index+16;i<3*index+20;i++)
						height=height<fft_out[i]?fft_out[i]:height;
					spectrum_height[index]=height>120?120:height;
					bar_diff[index]=(spectrum_height[index]-current_height)/48.000;
					bar_fragments[index]=0;
				}
				if(current_height!=spectrum_height[index]){	//如果还没显示完
					int diff_int=bar_diff[index];
					bar_fragments[index]+=bar_diff[index]-diff_int;
					if(bar_fragments[index]>=1) {
						diff_int+=1;
						bar_fragments[index]-=1;
					}
					if(bar_fragments[index]<=-1) {
						diff_int-=1;
						bar_fragments[index]+=1;
					}
					int new_height=current_height+diff_int;
					lcd_spectrum_set_height(index,new_height);
				}
			}
		} else {									//ADC收集完毕，进入一次FFT运算
			cnt=-1;									//终止ADC采集，以防误差（FFT运算时间远大于采集周期）
			arm_cfft_radix4_f32(&scfft, fft_in); 
			arm_cmplx_mag_f32(fft_in, fft_out, 1024);
			search_notes();
			ctp_scan();
			if(state)
				lcd_stave_update();  				//五线谱的动画
			cnt=0;									//重新启动ADC采集schedule
		}
    }
}
void search_notes(){	
	for(int i=0;i<14;i++)
		if(note_stamp[i]++>=1)
			lcd_note_press(i,0);
	 
	int index=-1;
	int maxMag=-1;
	for(int i=16;i<65;i++){
		if(fft_out[i]>=30 && maxMag<=fft_out[i]){
			maxMag=fft_out[i];
			index=i;
		}
	}
	
	int tmp=index;
	for(int i=2;i<=4;i++)
		for(int j=-1;j<=1;j++)
			if(fft_out[tmp/i+j]>15 && tmp/i+j>10)
				index=tmp/i+j;
	
	if(index==16 || index==17) lcd_stave_note(0);
	if(index==18 || index==19) lcd_stave_note(1);
	if(index==21) lcd_stave_note(2);
	if(index==22 || index==23) lcd_stave_note(3);
	if(index==25) lcd_stave_note(4);
	if(index==28 || index==29) lcd_stave_note(5);
	if(index==31 || index==32) lcd_stave_note(6);
	
	if(index==33 || index==34) lcd_stave_note(7);
	if(index==37 || index==38) lcd_stave_note(8);
	if(index==42 || index==43) lcd_stave_note(9);
	if(index==44 || index==45) lcd_stave_note(10);
	if(index==50 || index==51) lcd_stave_note(11);
	if(index==56 || index==57) lcd_stave_note(12);
	if(index==63 || index==64) lcd_stave_note(13);
}