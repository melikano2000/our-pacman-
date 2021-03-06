#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL2_rotozoom.h>
#include <string>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<SDL2/SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 800;
SDL_Surface* Surface=NULL;
SDL_Window* window=NULL;
SDL_Renderer* rend=NULL;
bool quit=false;
int count=0;
int score=0;
SDL_Event event;
class pacman;
int i;
int Blocks[55][4]={{20,68,620,78},{20,68,30,282},{20,272,138,282},{128,272,138,356},{20,346,138,356},
{20,400,138,410},{128,400,138,484},{20,474,138,484},{20,484,30,732},{20,722,620,732},
{610,474,620,732},{502,474,620,484},{502,400,512,484},{502,400,620,410},{502,346,620,356},
{502,272,512,356},{502,272,620,282},{610,68,620,282},{310,68,330,164},{74,122,138,164},
{182,122,266,164},{374,122,458,164},{502,122,566,164},{74,208,138,228},{502,208,566,228},
{182,208,202,356},{182,272,256,292},{246,208,394,228},{310,208,330,292},{374,272,458,292},
{438,208,458,356},{246,336,298,346},{342,336,394,346},{246,336,256,420},{384,336,394,420},
{246,410,394,420},{182,400,202,484},{438,400,458,484},{246,464,394,484},{310,464,330,548},
{74,528,138,548},{118,528,138,612},{182,528,266,548},{374,528,458,548},{502,528,566,548},
{502,528,522,612},{20,592,74,612},{566,592,620,612},{74,656,256,676},{182,592,202,676},
{236,592,384,612},{300,592,320,676},{374,656,566,676},{438,592,458,676},{298,338,342,344}};

class ghost
{
public:
      int x1;
      int x2;
      int y1;
      int y2;
      float vx;
      float vy;
      bool turnedblue=false;
      SDL_Rect png[8];
      SDL_Texture* ghost;
      SDL_Rect blue[4];
      SDL_Texture* blueghost;
      SDL_Rect eyes[4];
      SDL_Rect ghostpos={x1,y1,44,44};
      SDL_Texture* ghosteyes;
      int direction;
      void initghost ( std::string path ){
      	SDL_Surface* ghostsurf= IMG_Load( path.c_str() );
      	ghost=SDL_CreateTextureFromSurface( rend, ghostsurf);
		for(int i=0; i<8;i++){
			png[i].x=i*44;
			png[i].y=0;
			png[i].w=44;
			png[i].h=44;
		}
		SDL_FreeSurface( ghostsurf );
      }
      
      void blueinit(std::string path)
      {
      	SDL_Surface* ghostsurf= IMG_Load( path.c_str() );
      	blueghost=SDL_CreateTextureFromSurface( rend, ghostsurf);
		for(int i=0; i<4;i++){
			blue[i].x=i*44;
			blue[i].y=0;
			blue[i].w=44;
			blue[i].h=44;
		}
		SDL_FreeSurface( ghostsurf );
	  }

	void initeyes(std::string path)
      {
      	SDL_Surface* ghostsurf= IMG_Load( path.c_str() );
      	ghosteyes=SDL_CreateTextureFromSurface( rend, ghostsurf);
		for(i=0; i<4;i++){
			eyes[i].x=i*44;
			eyes[i].y=0;
			eyes[i].w=44;
			eyes[i].h=44;
		}
		SDL_FreeSurface( ghostsurf );
	  }
	  
      void show(int x, int y/*,int v,int v2*/)
	  {
		x1=x;
		y1=y;
		//vx=v;
		//vy=v2;
	  	x1+=vx;
	  	y1-=vy;
	  	ghostCollision();
	  	if(direction%2){
	  		direction--;
	  	}else{
	  		direction++;
	  	}
	  	ghostpos.x=x1; ghostpos.y=y1;
	  	SDL_RenderCopy(rend,ghost,&png[direction],&ghostpos);
      }

     

    void ghostCollision(){
	bool up=false,down=false,right=false,left=false;
	for(i=0; i<54; i++){
		if(vx!=0){
			if(x1+44==Blocks[i][0] && ((y1+1>=Blocks[i][1] && y1+1<=Blocks[i][3]) || (y1+43>=Blocks[i][1] && y1+43<=Blocks[i][3]) || (Blocks[i][1]>y1 && Blocks[i][1]<y1+44)) ){
				right=true;
			}
			else if(x1==Blocks[i][2] && ((y1+1>=Blocks[i][1] && y1+1<=Blocks[i][3]) || (y1+43>=Blocks[i][1] && y1+43<=Blocks[i][3]) || (Blocks[i][1]>y1 && Blocks[i][1]<y1+44)) ){
				left=true;
			}
		}
		else{
			if(y1+44==Blocks[i][1] && ((x1+1>=Blocks[i][0] && x1+1<=Blocks[i][2]) || (x1+43>=Blocks[i][0] && x1+43<=Blocks[i][2]) || (Blocks[i][0]>x1 && Blocks[i][0]<x1+44)) ){
				down=true;
			}
			else if(y1==Blocks[i][3] && ((x1+1>=Blocks[i][0] && x1+1<=Blocks[i][2]) || (x1+43>=Blocks[i][0] && x1+43<=Blocks[i][2]) || (Blocks[i][0]>x1 && Blocks[i][0]<x1+44)) ){
				up=true;
			}
		}
	}
	if(vx!=0){
		if((right && vx>0) || (left && vx<0)){
			vx=0;
			if(rand()%2){
				if(down) vy=2;
				else vy=-2;
				changepng();
			}
			else{
				if(up) vy=-2;
				else vy=2;
				changepng();
			}
		}
		else if((!right && vx>0) || (!left && vx<0)){
			switch(rand()%3){
				case 0: if(up){
					vx=0;
					vy=-2;
					changepng();
					break;
				}
				case 1: if(down){
					vx=0;
					vy=2;
					changepng();
					break;
				}
			}
		}
	}else{
		if((down && vy<0) || (up && vy>0)){
			vy=0;
			if(rand()%2){
				if(left) vx=2;
				else vx=-2;
				changepng();
			}
			else{
				if(right) vx=-2;
				else vx=2;
				changepng();
			}
		}
		else if((!down && vy<0) || (!up && vy>0)){
			switch(rand()%3){
				case 0: if(left){
					vy=0;
					vx=-2;
					changepng();
					break;
				}
				case 1: if(right){
					vy=0;
					vx=2;
					changepng();
					break;
				}
			}
		}
	}
} 

      void changepng(){
		if(vx>0) direction=6;
		else if(vx<0) direction=4;
		else if(vy<0) direction=2;
		else direction=0;
      }

      void ghostpacmanCollision();
};
ghost Inky;
ghost Pinky;
ghost Blinky;
ghost Clyde;


class pacman
{
  public:
  	int x1;
    int x2;
    int y1;
    int y2;
    float vx=0;
    float vy=0;
    SDL_Rect pac[9];
    SDL_Texture* pacman;
    SDL_Rect dead[11];
    SDL_Texture* deadpac;
    SDL_Rect Pacmanpos={x1,y1,44,44};
    int pacdirection;

void initpacman( std::string path ){
	 SDL_Surface* pacsurf= IMG_Load( path.c_str() );
      	pacman=SDL_CreateTextureFromSurface( rend, pacsurf);
		for(i=0; i<8;i++){
			pac[i].x=(i%2)*44;
			pac[i].y=(i/2)*44;
			pac[i].w=44;
			pac[i].h=44;
		}
	pac[8].x=88; pac[8].y=0; pac[8].w=44; pac[8].h=44;
	SDL_FreeSurface( pacsurf );
}

void initdead( std::string path ){
	 SDL_Surface* pacsurf= IMG_Load( path.c_str() );
      	deadpac=SDL_CreateTextureFromSurface( rend, pacsurf);
		for(int i=0; i<11;i++){
			dead[i].x=i*44;
			dead[i].y=0;
			dead[i].w=44;
			dead[i].h=44;
		}
	SDL_FreeSurface( pacsurf );
}

bool pacshow(int x, int y){
	x1=x;
	y1=y;
	PacmanWallCollision(vx,vy);
	x1+=vx;
	y1-=vy;
	if(vx || vy){
		if(pacdirection%2){
	  		pacdirection--;
	  	}else{
	  		pacdirection++;
	  	}
	}else if(pacdirection%2){
		pacdirection--;
	}
 	Pacmanpos.x=x1; Pacmanpos.y=y1;
	SDL_RenderCopy(rend,pacman,&pac[pacdirection],&Pacmanpos);
}

 void move(int x1,int x2,int y1,int y2,float vx,float vy){
      //if(){
        //y1+=vy;
        //y2+=vy;
      //}
      /*if(){
      	x1+=vx;
      	x2+=vy;
      }*/
      }

 bool PacmanWallCollision(int v1, int v2){
    bool up=false,down=false,right=false,left=false;
	for(i=0; i<54; i++){
		if(v1!=0){
			if(x1+44==Blocks[i][0] && ((y1+1>=Blocks[i][1] && y1+1<=Blocks[i][3]) || (y1+43>=Blocks[i][1] && y1+43<=Blocks[i][3]) || (Blocks[i][1]>y1 && Blocks[i][1]<y1+44)) ){
				right=true;
			}
			else if(x1==Blocks[i][2] && ((y1+1>=Blocks[i][1] && y1+1<=Blocks[i][3]) || (y1+43>=Blocks[i][1] && y1+43<=Blocks[i][3]) || (Blocks[i][1]>y1 && Blocks[i][1]<y1+44)) ){
				left=true;
			}
		}
		else{
			if(y1+44==Blocks[i][1] && ((x1+1>=Blocks[i][0] && x1+1<=Blocks[i][2]) || (x1+43>=Blocks[i][0] && x1+43<=Blocks[i][2]) || (Blocks[i][0]>x1 && Blocks[i][0]<x1+44)) ){
				down=true;
			}
			else if(y1==Blocks[i][3] && ((x1+1>=Blocks[i][0] && x1+1<=Blocks[i][2]) || (x1+43>=Blocks[i][0] && x1+43<=Blocks[i][2]) || (Blocks[i][0]>x1 && Blocks[i][0]<x1+44)) ){
				up=true;
			}
		}
	} 
	if(v1==vx && v2==vy){
		if(vx>0 && right) vx=0;
	else if(vx<0 && left) vx=0;
	else if(vy>0 && up) vy=0;
	else if(vy<0 && down) vy=0;
	return true;
	}
	else {
		if((v1>0 && right) || (v1<0 && left) || (v2>0 && up) || (v2<0 && down)) return false;
		else return true;
	}
}

	void showdead(){
		for(i=0;i<12;i++){
		SDL_Delay(50);
		SDL_SetRenderDrawColor( rend, 0, 0, 0, 0xFF );
		SDL_RenderClear( rend );
		for(int i=0;i<54;i++){
					boxRGBA(rend,Blocks[i][0],Blocks[i][1],Blocks[i][2],Blocks[i][3],0,0,255,255);
				}
		boxRGBA(rend,Blocks[54][0],Blocks[54][1],Blocks[54][2],Blocks[54][3],255,255,255,255);
		Pinky.show(Pinky.x1,Pinky.y1);
		Inky.show(Inky.x1,Inky.y1);
		Blinky.show(Blinky.x1,Blinky.y1);
		Clyde.show(Clyde.x1,Clyde.y1);
		SDL_RenderCopy(rend,deadpac,&dead[i],&Pacmanpos);
		SDL_RenderPresent( rend );
		}
	}
	


};
pacman Player;

void ghost::ghostpacmanCollision(){
      	if(turnedblue){
      		if( SDL_HasIntersection( &ghostpos , &Player.Pacmanpos )){
		count++;
		score=count*200;
		//ghost.showeyes();
		}
	} else if( SDL_HasIntersection( &ghostpos,&Player.Pacmanpos) ){
		Player.showdead();
		}
}

class fruit
{
 public:
	int x1;
	int x2;
	int y1;
	int y2;
	SDL_Rect fruit;
	SDL_Texture* food;
	SDL_Rect fruitpos={x1,y1,x2-x1,y2-y1};

	void initfruit( std::string path ){
		SDL_Surface* fruitsurf= IMG_Load( path.c_str() );
      	food=SDL_CreateTextureFromSurface( rend, fruitsurf);
		fruit.x=0;
		fruit.y=0;
		fruit.w=44;
		fruit.h=44;
      SDL_FreeSurface( fruitsurf);
	}

	
	
};
fruit Foody;

void init(){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	//TTF_Init();
	
	window= SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if(!window){
		printf("error creating window: %s\n", SDL_GetError());
		SDL_Quit();
	}
	
	rend= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!rend)
 	{
    	 printf("error creating renderer: %s\n", IMG_GetError());
     	getchar();
     	SDL_DestroyWindow(window);
     	SDL_Quit();
	 }
	 
}


void close()
{
	SDL_DestroyTexture(Blinky.ghosteyes);
	SDL_DestroyTexture(Blinky.ghost);
	SDL_DestroyTexture(Blinky.blueghost);
	SDL_DestroyTexture(Inky.ghosteyes);
	SDL_DestroyTexture(Inky.ghost);
	SDL_DestroyTexture(Inky.blueghost);
	SDL_DestroyTexture(Pinky.ghosteyes);
	SDL_DestroyTexture(Pinky.ghost);
	SDL_DestroyTexture(Pinky.blueghost);
	SDL_DestroyTexture(Clyde.ghosteyes);
	SDL_DestroyTexture(Clyde.ghost);
	SDL_DestroyTexture(Clyde.blueghost);
	SDL_DestroyTexture(Player.pacman);
	SDL_DestroyTexture(Player.deadpac);
	SDL_DestroyTexture(Foody.food);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow( window );
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	srand(time(0));
	init();
	Blinky.initghost("Blinky.png");
	Pinky.initghost("Pinky.png");
	Inky.initghost("Inky.png");
	Clyde.initghost("Clyde.png");
	Blinky.blueinit("Blue Ghosts.png");
	Inky.blueinit("Blue Ghosts.png");
	Pinky.blueinit("Blue Ghosts.png");
	Clyde.blueinit("Blue Ghosts.png");
	Blinky.initeyes("A pair of Eyes.png");
	Inky.initeyes("A pair of Eyes.png");
	Pinky.initeyes("A pair of Eyes.png");
	Clyde.initeyes("A pair of Eyes.png");
	Player.initpacman("Pacman.png");
	//Player.initdead("");
	Foody.initfruit("Cherry");
	Pinky.direction=1;
	Blinky.direction=3;
	Inky.direction=5;
	Clyde.direction=6;
	Player.pacdirection=2;
	Blinky.x1=298; Blinky.y1=292; Blinky.vx=2;Blinky.vy=0;
	Pinky.x1=254;Pinky.y1=356;Pinky.vx=0; Pinky.vy=-2;
	Inky.x1=298;Inky.y1=356;Inky.vx=-2; Inky.vy=0;
	Clyde.x1=342;Clyde.y1=356;Clyde.vx=0; Clyde.vy=2;
	Player.x1=298; Player.y1=420;
	while(!quit){
		while(SDL_PollEvent(&event)!=0){
			if( event.type == SDL_QUIT )
			{
				quit = true;
				return 0;
			}
			else if(event.type==SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                	case SDLK_LEFT:
                	if(Player.PacmanWallCollision(-4,0)){
                		Player.vy=0;
                		Player.vx=-4;
                		if(Player.pacdirection!=0 && Player.pacdirection!=1)
                			Player.pacdirection=0;
                	}
                	break;
                	case SDLK_RIGHT:
                	if(Player.PacmanWallCollision(4,0)){
                		Player.vy=0;
                		Player.vx=4;
                		if(Player.pacdirection!=2 && Player.pacdirection!=3)
                			Player.pacdirection=2;
                	}
                	break;
                	case SDLK_UP:
                	if(Player.PacmanWallCollision(0,4)){
                		Player.vy=4;
                		Player.vx=0;
                		if(Player.pacdirection!=4 && Player.pacdirection!=5)
                			Player.pacdirection=4;
                	}
                	break;
                	case SDLK_DOWN:
                	if(Player.PacmanWallCollision(0,-4)){
                		Player.vy=-4;
                		Player.vx=0;
                		if(Player.pacdirection!=6 && Player.pacdirection!=7)
                			Player.pacdirection=6;
                	}
                	break;
                }
            }
		}
		SDL_Delay(50);
		SDL_SetRenderDrawColor( rend, 0, 0, 0, 0xFF );
		SDL_RenderClear( rend );
		for(int i=0;i<54;i++){
					boxRGBA(rend,Blocks[i][0],Blocks[i][1],Blocks[i][2],Blocks[i][3],0,0,255,255);
				}
		boxRGBA(rend,Blocks[54][0],Blocks[54][1],Blocks[54][2],Blocks[54][3],255,255,255,255);
		Player.pacshow(Player.x1,Player.y1);
		Pinky.show(Pinky.x1,Pinky.y1);
		Inky.show(Inky.x1,Inky.y1);
		Blinky.show(Blinky.x1,Blinky.y1);
		Clyde.show(Clyde.x1,Clyde.y1);
		Pinky.ghostpacmanCollision();
		Blinky.ghostpacmanCollision();
		Inky.ghostpacmanCollision();
		Clyde.ghostpacmanCollision();
		//Player.pacshow(298,420);
		//SDL_RenderCopy(rend,Pinky.ghost,&Pinky.png[Pinky.direction],&Pinky.ghostpos);
		SDL_RenderPresent( rend );
	}

	close();
	return 0;
}
