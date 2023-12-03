#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "tcp.h"

void connect_wifi(void)
{
    uint8_t ret;
    char ip_buf[16];
    
    ret = atk_mw8266d_init(115200);
    
    ret  = atk_mw8266d_restore();
    ret += atk_mw8266d_at_test();
    ret += atk_mw8266d_set_mode(1);
    ret += atk_mw8266d_sw_reset();
    ret += atk_mw8266d_ate_config(0);
	
	delay_ms(1000);
	
    ret += atk_mw8266d_join_ap("123","20050221");
	//ret += atk_mw8266d_get_ip(ip_buf);
	//printf("  |- ��ȡIP��ַ: %s\n",&ip_buf[0]);
	
	delay_ms(1000);
	
    ret += atk_mw8266d_connect_tcp_server("192.168.0.8","10201");
	ret += atk_mw8266d_enter_unvarnished();
	
	delay_ms(1000);
	
    atk_mw8266d_uart_printf("1");
    atk_mw8266d_uart_printf("3:100,2000,DO");
    atk_mw8266d_uart_printf("2");
}
