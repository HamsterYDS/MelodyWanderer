void lcd_scale_init();								//标尺初始化

void lcd_sections_init();							//四个区域的划分线初始化
void lcd_logo_init();								//LOGO初始化
void lcd_char_init();								//文字初始化
void lcd_buttons_init();							//按钮初始化

void lcd_spectrum_init();							//频谱初始化
void lcd_spectrum_set_height(int index,int height);	//设置一个频谱条的高度

void lcd_stave_init();								//五线谱初始化
void lcd_stave_note(int pitch);						//将某个音调设置为响应
void lcd_stave_line(int pitch,int index,int litup); //染色对应音符对应位置的区域 
void lcd_stave_update();							//滚动五线谱

void lcd_note_init();								//琴键初始化
void lcd_note_press(int pitch,int press);			//将某个琴键显示为按下 


