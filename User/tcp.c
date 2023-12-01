#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "tcp.h"

void connect_wifi(void)
{
    uint8_t ret;
    char ip_buf[16];
    
    ret = atk_mw8266d_init(115200);
    if (ret != 0) printf("ATK-MW8266D ��ʼ��ʧ��\n");
	else printf("ATK-MW8266D ��ʼ���ɹ�\n");
    
    printf("AP��ʼ����......\n");
    ret  = atk_mw8266d_restore();
	printf("  |- �ָ���������: %d\n",ret);
    ret += atk_mw8266d_at_test();
	printf("  |- AT����: %d\n",ret);
    ret += atk_mw8266d_set_mode(1);
	printf("  |- Stationģʽ����: %d\n",ret);
    ret += atk_mw8266d_sw_reset();
	printf("  |- �����λ: %d\n",ret);
    ret += atk_mw8266d_ate_config(0);
	printf("  |- �رջ��Թ���: %d\n",ret);
	
	delay_ms(1000);
	
    ret += atk_mw8266d_join_ap("123","20050221");
	printf("  |- ����wifi: %d\n",ret);
	//ret += atk_mw8266d_get_ip(ip_buf);
	//printf("  |- ��ȡIP��ַ: %s\n",&ip_buf[0]);
	
	delay_ms(1000);
	
    ret += atk_mw8266d_connect_tcp_server("192.168.0.8","10201");
	printf("  |- ����TCP������: %d\n",ret);
	ret += atk_mw8266d_enter_unvarnished();
	printf("  |- ����͸��ģʽ: %d\n",ret);
	
	delay_ms(1000);
	printf("  |- ��ʼ������Ϣ\n");
	
    atk_mw8266d_uart_printf("1");
    atk_mw8266d_uart_printf("3:100,2000,DO");
    atk_mw8266d_uart_printf("2");
}
