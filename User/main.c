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

int current_time=0;							//����ʱ�� ��λ:us
 
arm_cfft_radix4_instance_f32 scfft;
extern float fft_in[1024 * 2];				//FFT��������
float fft_out[1024];						//��ǰ�ε�FFT���

void search_notes();						//ƥ�����е�����

extern int cnt;								//������

extern int bar_heights[21];
extern int note_stamp[14];

extern int state; 							//��ǰ״̬����ͣ/��ʼ

int main(void){
    HAL_Init();								//��ʼ��HAL��
    sys_stm32_clock_init(RCC_PLL_MUL9);		//��ʼ��ʱ��
    delay_init(72); 						//��ʼ����ʱ
	usart_init(115200); 					//��ʼ������
	lcd_init(); 							//��ʼ��LCD��ʾ��
    led_init();                     	    /* ��ʼ��LED */

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
	
	gtim_timx_int_init(125  - 1, 36 - 1); 			// 1Mhz�ļ���Ƶ�ʣ�����125-21��Ϊ125-21us
	
	adc_dma_init((uint32_t)&adc_buf);		 		//��ʼ��ADC-DMA�ɼ�
	arm_cfft_radix4_init_f32(&scfft, 1024, 0, 1);	//��ʼ��scfft�ṹ�壬�趨FFT��ز���
	
	int spectrum_height[21]; 						//Ƶ������������
	float bar_fragments[21]; 						//Ƶ������������
	float bar_diff[21]; 							//Ƶ������������
	
    while(1){ 
		if(cnt<1024){
			//������ʾ
			if(cnt<21*48){							//Ƶ�׵Ķ��� 0~209ticks
				int index=cnt%21;					//��ǰˢ�µ�Ƶ�������
				int process=cnt/21;					//��ǰˢ�½���(0~10)
				int current_height=bar_heights[index];	//��ǰ�߶�
				if(process==0){
					int height=0;						//�����߶ȵ���Ҫ���أ�����Ƶ�ʣ�
					for(int i=3*index+16;i<3*index+20;i++)
						height=height<fft_out[i]?fft_out[i]:height;
					spectrum_height[index]=height>120?120:height;
					bar_diff[index]=(spectrum_height[index]-current_height)/48.000;
					bar_fragments[index]=0;
				}
				if(current_height!=spectrum_height[index]){	//�����û��ʾ��
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
		} else {									//ADC�ռ���ϣ�����һ��FFT����
			cnt=-1;									//��ֹADC�ɼ����Է���FFT����ʱ��Զ���ڲɼ����ڣ�
			arm_cfft_radix4_f32(&scfft, fft_in); 
			arm_cmplx_mag_f32(fft_in, fft_out, 1024);
			search_notes();
			ctp_scan();
			if(state)
				lcd_stave_update();  				//�����׵Ķ���
			cnt=0;									//��������ADC�ɼ�schedule
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