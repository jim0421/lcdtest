Interface
/* the screen should be (640, 480) 
 * num:int -> number of display line (1-4) no error check
 * side:int -> style of layout (both:0, left:-1, right:1) no error check
 * open:bool -> whether the switch is open or closed
 * type:int -> 0 stands for 压力 and 1 stands for 位移
 * name:int -> 1-4 represents A-D
 * Chinese character gap -> 28px
 * w1 -> (20+56,30) w2 -> (230,30)
 */
void init_frame(int num, int side, int type, bool open, int name)

/* type:int -> 0 stands for 压力 and 1 stands for 位移
 * open:bool -> whether the switch is open or closed
 * value:struct D_VALUEBRANCH * -> records all the colors and values
 */
void change_value(int type, bool open, struct D_VALUEBRANCH *value)

Declaration
<graphics.h> 
putpixel(int x, int y, int color);
initgraph(int x, int y);
closegraph();