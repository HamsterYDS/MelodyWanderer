void lcd_scale_init();								//��߳�ʼ��

void lcd_sections_init();							//�ĸ�����Ļ����߳�ʼ��
void lcd_logo_init();								//LOGO��ʼ��
void lcd_char_init();								//���ֳ�ʼ��
void lcd_buttons_init();							//��ť��ʼ��

void lcd_spectrum_init();							//Ƶ�׳�ʼ��
void lcd_spectrum_set_height(int index,int height);	//����һ��Ƶ�����ĸ߶�

void lcd_stave_init();								//�����׳�ʼ��
void lcd_stave_note(int pitch);						//��ĳ����������Ϊ��Ӧ
void lcd_stave_line(int pitch,int index,int litup); //Ⱦɫ��Ӧ������Ӧλ�õ����� 
void lcd_stave_update();							//����������

void lcd_note_init();								//�ټ���ʼ��
void lcd_note_press(int pitch,int press);			//��ĳ���ټ���ʾΪ���� 


