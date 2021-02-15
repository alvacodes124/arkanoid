#include <allegro.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>


using namespace std;


BITMAP* lienzo;
BITMAP* baground;
int     score = 0;
int     vidas = 3;
int     partidas = 1;

class Alarm{
	
public:
	int cont;
	
	bool initcount(int segunds){
		
		if(segunds == 0){
			return false;
		}
		
		if(cont >= segunds){
			cont = 0;
			return true;
		}
		
		cont++;
		
		return false;
	}
	
	Alarm();
	~Alarm();
};

class Object{
	
public:
	int x,y;
	int w,h;
	int vx,vy;
	BITMAP *sprite;
	int tipe_object;
	
	Object(){
		this->x = 0;
		this->y = 0;
		this->w = 0;
		this->h = 0;
		this->vx = 0;
		this->vy = 0;
		sprite = NULL;
	}
	~Object(){
		destroy_bitmap(sprite);
	}
	
	virtual bool cargar(BITMAP *_buffer) = 0;
	
	void setsprite(BITMAP *_lienzo){
		sprite = _lienzo;
		this->w = sprite->w;
		this->h = sprite->h;
	}
	
	void setposition(int x,int y){
		this->x = x;
		this->y = y;
	}
	
	void move(int x,int y){
		this->x += x;
		this->y += y;
	}
	
	void draw(BITMAP *_buffer){
		masked_blit(this->sprite,_buffer,0,0,this->x,this->y,this->w,this->h);
	}
	
	int gettype(){
		return tipe_object;
	}
	

};

class Button : public Object{
public:
	bool fclock;
	BITMAP *spr,*spr2;
	
	Button(char *img,char *img2,int _x,int _y){
		this->x = _x; 
		this->y = _y; 
		this->fclock = false;
		this->tipe_object = 0; 
		this->spr = load_bitmap(img,NULL);
		this->spr2 = load_bitmap(img2,NULL); 
		this->w = spr2->w; 
		this->h = spr2->h; 
	}                                                                  
	Button(){
		this->tipe_object = 0; 
		this->fclock = false;
		this->x = 0; 
		this->y = 0; 
	}
	~Button(){}
	
	void inisializar(string img,string img2,int _x,int _y){
		x = _x; y = _y; 
		fclock = false;
		spr = load_bitmap(img.c_str(),NULL);
		spr2 = load_bitmap(img2.c_str(),NULL); 
	}
	
	bool cargar(BITMAP *_buffer){
		if(mouse_x > x && mouse_y > y && mouse_x < w+x && mouse_y < h+y){
			masked_blit(spr2 , _buffer , 0,0,x,y , spr2->w , spr2->h);
			if(mouse_b & 1){
				fclock = true;
			}
		}
		else{
			masked_blit(spr , _buffer , 0,0,x,y , spr->w , spr->h);
		}
		
		return fclock;
	}
	
};

class Personaje : public Object{
public:
	
	Personaje(string cadena,int _x,int _y,int _vx,int _vy){
		this->x = _x;
		this->y = _y;
		this->tipe_object = 1;
		this->sprite = load_bitmap(cadena.c_str(),NULL);
		this->w = sprite->w; 
		this->h = sprite->h; 
		this->vx = _vx;
		this->vy = _vy;
	}
	Personaje(){
		this->tipe_object = 1;
		this->x = 0; 
		this->y = 0; 
		this->vx = 0; 
		this->vy = 0;
		this->sprite = NULL;
	}
	~Personaje(){}
	
	void inisializar(string cadena,int _x,int _y,int _vx,int _vy){
		this->x = _x;
		this->y = _y;
		this->vx = _vx;
		this->vy = _vy;
		sprite = load_bitmap(cadena.c_str(),NULL);
	}
	void mover(){
		if(key[KEY_LEFT] && x >= 0){x -= vx;}
		if(key[KEY_RIGHT] && x+w <= 800){x += vx;}
	}
	bool cargar(BITMAP *_buffer){
		draw(_buffer);    
	}
};

class Blocks : public Object{
	bool close__;
public:
	Blocks(string cadena,int x,int y){
		this->sprite           =     load_bitmap(cadena.c_str(),NULL);
		this->tipe_object      =     2;
		this->w                =     sprite->w;
		this->h                =     sprite->h;
		this->x                =     x;
		this->y                =     y;
		this->close__ =  false; 
	}
	Blocks(){
		this->sprite           =     NULL;
		this->tipe_object      =     3;
		this->w                =     0;
		this->h                =     0;
		this->x                =     0;
		this->y                =     0;
		this->close__ =  false; 
	}
	~Blocks(){}
	
	void closet(){
		close__  = true;
	}
	
	void repon(){
		close__ = false;
	}
	
	void inisializar(string cadena,int x,int y){
		this->sprite    =     load_bitmap(cadena.c_str(),NULL);
		this->w         =     sprite->w;
		this->h         =     sprite->h;
		this->x         =     x;
		this->y         =     y;
		this->close__   =  false; 
	}
	
	bool cargar(BITMAP *_buffer){
		if(!close__){
			draw(_buffer);    
		}
	}
	
	
	bool getclose(){
		return !close__;
	}
};

class Pelota : public Object{
	
public:
	bool dir_x,dir_y;
	bool close__;
	
	
	Pelota(string cadena,int _x,int _y,int _vx,int _vy){
		this->x = _x;
		this->y = _y;
		this->vx = _vx;
		this->vy = _vy;
		this->dir_x = false;
		this->dir_y = false;
		this->tipe_object = 3;
		this->sprite = load_bitmap(cadena.c_str(),NULL);
		this->w = sprite->w; 
		this->h = sprite->h;
	}
	Pelota(){
		this->x = 0; 
		this->y = 0; 
		this->dir_x = false; 
		this->dir_y = false;
		this->tipe_object = 3;
	}
	~Pelota(){}
	

	
	void inisializar(string cadena,int _x,int _y,int _vx,int _vy){
		this->x = _x;
		this->y = _y;
		this->vx = _vx;
		this->vy = _vy;
		this->sprite = load_bitmap(cadena.c_str(),NULL);
	}
	void mover(Object &objeto1){
		if(x >= 800-w && dir_y == false && dir_x == false){dir_x = true; }
		if(x >= 800-w && dir_y == true && dir_x == false){dir_x = true;  }
		if(x <= 0 && dir_y == false && dir_x == true){dir_x = false;     }
		if(x <= 0 && dir_y == true && dir_x == true){dir_x = false;      }
		
		if(y >= 600-h && dir_y == true && dir_x == true){dir_y = false;  vidas--;}
		if(y >= 600-h && dir_y == true && dir_x == false){dir_y = false; vidas--;}
		if(y <= 0   && dir_y == false && dir_x == true){dir_y = true;    }
		if(y <= 0 && dir_y == false && dir_x == false){dir_y = true;     }
		
		if(dir_x == 0){x += vx;}if(dir_x == 1){x -= vx;}
		if(dir_y == 0){y -= vy;}if(dir_y == 1){y += vy;}
		
	}
	
	void colision2(Personaje &objeto1){
		if((y+h >= objeto1.y && y <= objeto1.y+objeto1.h) && x+w+1 == objeto1.x){dir_x = true;}
		if((x+w >= objeto1.x && x <= objeto1.x+objeto1.w) && y+h+1 == objeto1.y){dir_y = false;}			
		if((y+h >= objeto1.y && y <= objeto1.y+objeto1.h) && x-1 == objeto1.x+objeto1.w){dir_x = false;}
		if((x+w >= objeto1.x && x <= objeto1.x+objeto1.w) && y-1 == objeto1.y+objeto1.h){dir_y = true;}
	}
	
	void colision(Blocks &objeto1){
		if((y+h >= objeto1.y && y <= objeto1.y+objeto1.h) && x+w+1 == objeto1.x && objeto1.getclose()){score += 10;dir_x = true; objeto1.closet();}                                                            
		if((x+w >= objeto1.x && x <= objeto1.x+objeto1.w) && y+h+1 == objeto1.y && objeto1.getclose()){score += 10;dir_y = false; objeto1.closet();}			
		if((y+h >= objeto1.y && y <= objeto1.y+objeto1.h) && x-1 == objeto1.x+objeto1.w && objeto1.getclose()){score += 10;dir_x = false; objeto1.closet();}
		if((x+w >= objeto1.x && x <= objeto1.x+objeto1.w) && y-1 == objeto1.y+objeto1.h && objeto1.getclose()){score += 10;dir_y = true;  objeto1.closet();}
	}
	
	bool cargar(BITMAP *_buffer){
		draw(_buffer);    
	}
	
};

void inisializar_blocks(Blocks BLOCKE[9][13]){
	int sum = 0;
	int sum2 = 0;
	
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 13; j++){
			BLOCKE[i][j].inisializar("Recursos/Block.bmp",151+sum,220+sum2);
			
			sum += 39;
		}
		sum2 += 18;
		sum = 0;
	}
}

void colisions_blocks(Blocks BLOCKE[9][13],Pelota &pelota){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 13; j++){
			pelota.colision(BLOCKE[i][j]); 
		}
	}
}

bool get_close_blocks(Blocks BLOCKE[9][13]){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 13; j++){
			if(BLOCKE[i][j].getclose()){
				return false;
			}
		}
	}
	return true;
}

void cargar_blocks(Blocks BLOCKE[9][13],BITMAP *_lienzo){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 13; j++){
			BLOCKE[i][j].cargar(_lienzo);
		}
	}
}

void repon_blocks(Blocks BLOCKE[9][13]){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 13; j++){
			BLOCKE[i][j].repon();
		}
	}
}


int fgettype(string filename){
	ifstream file;
	size_t size = 0;
	file.open(filename.c_str(),ios::in | ios::binary | ios::ate);
	size = file.tellg();
	file.seekg(0,ios::end);
	return size;
}

char *string_file(FILE *file,string filename){
	char *cadena = (char*) malloc(fgettype(filename)*sizeof(char));
	for(int i = 0; i < fgettype(filename); i++){
		fread(&cadena[i],sizeof(char),1,file);
	}
	return cadena;
}


bool salida = false;
bool parte2 = false;

volatile bool Close = true;
void Handler(){Close = false;}

int main(){
	allegro_init();
	install_timer();
	install_keyboard();
	install_mouse();
	
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	
	lienzo = create_bitmap(800,600);
	baground = load_bitmap("Recursos/Arcanoid.bmp",NULL);
	
	LOCK_FUNCTION(Handler);
	set_close_button_callback(Handler);
	
	FONT *font_t = load_font("Recursos/pixel2.pcx",NULL,NULL);
	
	Button button((char*)"Recursos/comensar.bmp",(char*)"Recursos/comensar2.bmp",100,100);
	Button button2((char*)"Recursos/salir.bmp",(char*)"Recursos/salir2.bmp",100,200);
	Personaje personaje("Recursos/personaje.bmp",350,500,1,1);
	Pelota pelota("Recursos/pelota.bmp",256,462,1,1);
	
	Blocks blockes[9][13];
	
	inisializar_blocks(blockes);
	
	while(!salida){	
		show_mouse(screen);
		rectfill(lienzo,9,9,120,75,makecol(120,156,192));
		textprintf_ex(lienzo,font_t,21,11,makecol(255,255,255),-1,"score : %d",score);
		textprintf_ex(lienzo,font_t,21,31,makecol(255,255,255),-1,"partida : %d",partidas);
		textprintf_ex(lienzo,font_t,21,51,makecol(255,255,255),-1,"vidas : ");
		
		for(int i = 0; i < vidas;i++){
			textprintf_ex(lienzo,font,80+(i*10),60,makecol(255,255,255),-1,"%c",42);
		}
		
		if(key[KEY_ESC] || !Close){
			salida = true;
		}
		
		if(!parte2){
			if(button2.cargar(lienzo)){salida = true;}
			if(button.cargar(lienzo)){
				baground = load_bitmap("Recursos/Arcanoid2.bmp",NULL);
				parte2 = true;
			}
		}
		
		if(parte2){
			
			pelota.mover(personaje);
			pelota.colision2(personaje);
			colisions_blocks(blockes,pelota);
			
			if(vidas == 0){
				salida = true;
				allegro_message("Game Over record de : %d" , score);
			}
			
			if(get_close_blocks(blockes)){
				allegro_message("has ganado felisitaciones %d partidas",partidas);
				partidas++;
				repon_blocks(blockes);
			}
					
			pelota.cargar(lienzo);
			personaje.mover();
			personaje.cargar(lienzo);
			cargar_blocks(blockes,lienzo);
		}
			
		                     
		
		
		
		draw_sprite(screen,lienzo,0,0);
		clear_to_color(lienzo,0xffffff);
		blit(baground,lienzo,0,0,0,0,800,600);
	}
	
	

	
	clear_keybuf();
	return 0;
}
END_OF_MAIN()


