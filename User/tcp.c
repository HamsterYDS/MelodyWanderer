#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "tcp.h"

void connect_wifi(void)
{
    uint8_t ret;
    char ip_buf[16];
    
    ret = atk_mw8266d_init(115200);
    if (ret != 0) printf("ATK-MW8266D 初始化失败\n");
	else printf("ATK-MW8266D 初始化成功\n");
    
    printf("AP初始化中......\n");
    ret  = atk_mw8266d_restore();
	printf("  |- 恢复出厂设置: %d\n",ret);
    ret += atk_mw8266d_at_test();
	printf("  |- AT测试: %d\n",ret);
    ret += atk_mw8266d_set_mode(1);
	printf("  |- Station模式开启: %d\n",ret);
    ret += atk_mw8266d_sw_reset();
	printf("  |- 软件复位: %d\n",ret);
    ret += atk_mw8266d_ate_config(0);
	printf("  |- 关闭回显功能: %d\n",ret);
	
	delay_ms(1000);
	
    ret += atk_mw8266d_join_ap("123","20050221");
	printf("  |- 连接wifi: %d\n",ret);
	//ret += atk_mw8266d_get_ip(ip_buf);
	//printf("  |- 获取IP地址: %s\n",&ip_buf[0]);
	
	delay_ms(1000);
	
    ret += atk_mw8266d_connect_tcp_server("192.168.0.8","10201");
	printf("  |- 连接TCP服务器: %d\n",ret);
	ret += atk_mw8266d_enter_unvarnished();
	printf("  |- 进入透传模式: %d\n",ret);
	
	delay_ms(1000);
	printf("  |- 开始发送信息\n");
	
    atk_mw8266d_uart_printf("1");
    atk_mw8266d_uart_printf("3:100,2000,DO");
    atk_mw8266d_uart_printf("2");
}
