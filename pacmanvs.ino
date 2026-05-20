/*!
 * @file testRGBMatrix.ino
 * @brief Run the routine to test the RGB LED Matrix Panel
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie]](jie.tang@dfrobot.com)
 * @version  V1.0.1
 * @date  2022-03-23
 * @url https://github.com/DFRobot/DFRobot_RGBMatrix
 */
 
#include <DFRobot_RGBMatrix.h> // Hardware-specific library

/* === Pinout / Matrix configuration === */
#define OE   	9
#define LAT 	10
#define CLK 	11
#define A   	A0
#define B   	A1
#define C   	A2
#define D   	A3
#define E   	A4
#define WIDTH 64
#define _HIGH	64

/* === Game globals (configuration and state) === */
int futurz=1;
int mange=0;
int minute=0;
bool over=false;
char mess='8';
int MaxV1 = 0;
int MaxV2 = 0;
int MinV1 = 800;
int MinV2 = 800;
int volatile nextZ=-1;
int sensorValue1 =500;
int sensorValue2 = 500;
int seconde=0;
int tp=49;
int f=1;
int volatile z=2;
int c=0;
int co=0;
int g=0;
int x=59;
int y=32;
const int s=12;
volatile int incx=0;
volatile int incy=0;
int goxy[4][2]={{32,22},{32,22},{31,22},{31,22}};
// rouge bleu rose orange
int incgoxy[4][2]={{0,0},{0,0},{0,0},{0,0}};
int dirgos[4]={1,1,1,1};
//0=gauche 1=haut 2=droite 3=bas
int cordmaze[s][4]={{0,0,64,64},{1,1,62,62},{22,20,20,9},{23,21,18,7},{0,20,8,9},{0,21,7,7},{7,7,5,8},{17,7,8,8},{30,1,4,14},{31,1,2,14},{13,20,4,9}};
int nextpath[4]={0,1,0,0};
int numpath=0;
int futur[5][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
int controle=7;
int basemodeghost[4]={0,0,2,3};
int modeghost[4]={5,0,2,3};

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);
int couleur[9]={matrix.Color333(4, 4, 0),matrix.Color333(4, 0, 0),matrix.Color333(0, 4, 4),matrix.Color333(4, 2, 2),matrix.Color333(4, 2, 0),matrix.Color333(0, 0, 4),matrix.Color333(0, 0, 0),matrix.Color333(4, 4, 4),matrix.Color333(2, 0, 4)};
//0=jaune 1=rouge 2=cyan 3=rose 4=orange 5=bleu 6=noir 7=blanc
int gomme[5][3]={{32,32,1},{4,4,1},{59,4,1},{4,59,1},{59,59,1}};
const int plen=18;
int pointgomme[plen][3]={{4,9,1},{4,13,1},{4,17,1},{7,17,1},{10,17,1},{14,17,1},{19,17,1}
,{23,17,1},{27,17,1},{32,17,1},{36,17,1},{40,17,1},{44,17,1},{49,17,1},{53,17,1},{56,17,1},{59,17,1}




};
int point=0;
int ancienpoint=0;
int count=0;

/* === Drawing & movement routines === */
// Draw Pac-Man sprite at (x,y)
// Uses global variables `x`, `y`, `z` and `f` for orientation/animation
void pac(){
  matrix.fillCircle(x, y, 2,couleur[0]);
  if (f==1){
    switch(z){
      case 0:
        matrix.fillRect(x-2,y-1,1,3,couleur[6]);
        matrix.drawPixel(x-1, y, couleur[6]); 
        matrix.drawPixel(x-2, y-2, couleur[0]); 
        matrix.drawPixel(x-2, y+2, couleur[0]); 
        break;
      case 1:
        matrix.fillRect(x-1,y-2,3,1,couleur[6]);
        matrix.drawPixel(x, y-1, couleur[6]); 
        matrix.drawPixel(x-2, y-2, couleur[0]); 
        matrix.drawPixel(x+2, y-2, couleur[0]); 
        break;
      case 2:
        matrix.fillRect(x+2,y-1,1,3,couleur[6]);
        matrix.drawPixel(x+1, y, couleur[6]); 
        matrix.drawPixel(x+2, y+2, couleur[0]); 
        matrix.drawPixel(x+2, y-2, couleur[0]); 
        break;
      case 3:
        matrix.fillRect(x-1,y+2,3,1,couleur[6]);
        matrix.drawPixel(x, y+1, couleur[6]); 
        matrix.drawPixel(x+2, y+2, couleur[0]); 
        matrix.drawPixel(x-2, y+2, couleur[0]); 
        break;
      default:
      5;
    }
    
  }else{
    matrix.drawPixel(x-2, y-2, couleur[6]); 
    matrix.drawPixel(x-2, y+2, couleur[6]); 
    matrix.drawPixel(x+2, y-2, couleur[6]); 
    matrix.drawPixel(x+2, y+2, couleur[6]); 
  }






  f=1-f;
}
// Update pacman velocity deltas (`incx`, `incy`) from direction `z`
void pacmove(int z){
  switch(z){
    case 0:
      incx=-1;
      incy=0;
      break;
    case 1:
      incx=0;
      incy=-1;
      break;
    case 2:
      incx=1;
      incy=0;
      break;
    case 3:
      incx=0;
      incy=1;
      break;
    case -1:
      incx=0;
      incy=0;
      break;
  }

}


// Compute next pacman direction based on sensors v1/v2 and pathfinding
void pacNextStep(int v1, int v2){
  nextstep(x,y,z,4);
  
  if (((v1 > 550) || (v1<500)) || ((v2>550) || (v2<500))){
    if(abs(v1-516)>abs(v2-516)){
      
      if(v1 < 450){
        nextZ = 1;
      }
      
      if(v1 > 550){
        nextZ = 3;
      }
    }else{
      if(v2 < 450){
          nextZ = 0;
      }
      if(v2 > 550){
        nextZ = 2;
      }
    }
  }

  if (futur[4][0]+futur[4][1]+futur[4][2]+futur[4][3]>1){
    if (futur[4][z] !=1){
      z = -1;
    }
    if (futur[4][nextZ] == 1){
      z = nextZ;
    }
  }


  
  
}



// Clear a 5x5 area around (x,y)
void pagsup(int x,int y){
  matrix.fillRect(x-2, y-2, 5,5,couleur[6]);
}





// Draw a ghost at (x,y) with direction `z` and color index `c`
void ghost(int x,int y,int z,int c){
  if (modeghost[c]==4){
    c=7;
  }
  matrix.fillCircle(x, y, 2,couleur[c+1]);
  matrix.drawPixel(x-1, y-1,couleur[7]);
  matrix.drawPixel(x+1, y-1,couleur[7]);
  switch(z){
    case 0:
      matrix.drawPixel(x-2, y-1,couleur[5]);
      matrix.drawPixel(x, y-1,couleur[5]);
      break;
    case 1:
      matrix.drawPixel(x-1, y-2,couleur[5]);
      matrix.drawPixel(x+1, y-2,couleur[5]);
      break;
    case 2:
      matrix.drawPixel(x+2, y-1,couleur[5]);
      matrix.drawPixel(x, y-1,couleur[5]);
      break;
    case 3:
      matrix.drawPixel(x-1, y,couleur[5]);
      matrix.drawPixel(x+1, y,couleur[5]);
      break;
  }
  if (g==1){
    matrix.drawPixel(x-2, y+2,couleur[c+1]);
    matrix.drawPixel(x+2, y+2,couleur[c+1]);
    matrix.drawPixel(x-1, y+2,couleur[6]);
    matrix.drawPixel(x+1, y+2,couleur[6]);
  }else{
    matrix.drawPixel(x, y+2,couleur[6]);
    matrix.drawPixel(x-2, y+2,couleur[6]);
    matrix.drawPixel(x+2, y+2,couleur[6]);
  }
  g=1-g;
}








// Initialize the maze static elements on the matrix
void initmaze(){
  for(int i=0;i<s;i++){
    matrix.drawRect(cordmaze[i][0],cordmaze[i][1],cordmaze[i][2],cordmaze[i][3],couleur[5]);
    matrix.drawRect(64-cordmaze[i][0]-cordmaze[i][2],cordmaze[i][1],cordmaze[i][2],cordmaze[i][3],couleur[5]);
    matrix.drawRect(cordmaze[i][0],64-cordmaze[i][1]-cordmaze[i][3],cordmaze[i][2],cordmaze[i][3],couleur[5]);
    matrix.drawRect(64-cordmaze[i][0]-cordmaze[i][2],64-cordmaze[i][1]-cordmaze[i][3],cordmaze[i][2],cordmaze[i][3],couleur[5]);

    
  }
  matrix.drawRect(0,29,2,6,couleur[7]);
  matrix.drawRect(62,29,2,6,couleur[7]);
  matrix.drawRect(29,20,6,2,couleur[6]);
  matrix.drawRect(29,20,6,1,couleur[7]);
  matrix.drawRect(0,22,2,5,couleur[6]);
  matrix.drawRect(0,37,2,5,couleur[6]);
  matrix.drawRect(62,22,2,5,couleur[6]);
  matrix.drawRect(62,37,2,5,couleur[6]);
  matrix.drawRect(23,36,18,7,couleur[6]);
}









// Update ghosts velocity deltas from direction array `z[]`
void gosmove(int z[]){
  g=1-g;
  for(int i=0;i<4;i++){
    switch(z[i]){
      case 0:
        incgoxy[i][0]=-1;
        incgoxy[i][1]=0;
        break;
      case 1:
        incgoxy[i][0]=0;
        incgoxy[i][1]=-1;
        break;
      case 2:
        incgoxy[i][0]=1;
        incgoxy[i][1]=0;
        break;
      case 3:
        incgoxy[i][0]=0;
        incgoxy[i][1]=1;
        break;
      case 4:
        incgoxy[i][0]=0;
        incgoxy[i][1]=0;
    }
  }
}


// Decide and set movement for ghost `f` based on AI mode `ca`
void stepghost(int z,int f,int ca,bool c,int x,int y){
  int xz=goxy[f][0];
  int yz=goxy[f][1];
  nextstep(xz,yz,z,f);
  int r=random(10);
  if (controle!=f){
    switch(ca){
      
      case 0:

        if ((futur[f][0]+futur[f][1]+futur[f][2]+futur[f][3])>2){
          
          int dx=xz-x;
          int dy=yz-y;
          if (dx<0){
            if (dy<0){
              
              // haut gauche
              if (dx>=dy){
                
                if (((z+2)%4!=3) && futur[f][3]==1){
                  dirgos[f]=3;
                }else{
                  if (((z+2)%4!=2) && futur[f][2]==1){
                    dirgos[f]=2;
                  }else{
                    if (futur[f][0]==1){
                      dirgos[f]=0;
                    }else{
                      dirgos[f]=1;
                    }
                    
                  }
                }
                
              }else{
                if (((z+2)%4!=2) && futur[f][2]==1){
                  dirgos[f]=2;
                }else{
                  if (((z+2)%4!=3) && futur[f][3]==1){
                    dirgos[f]=3;
                  }else{
                    if (futur[f][1]==1){
                      dirgos[f]=1;
                    }else{
                      dirgos[f]=0;
                    }
                  }
                }
              }
            }else{
              
              // bas gauche
              if (dx>=(-dy)){
                if ((((z+2)%4)!=1) && (futur[f][1]==1)){
                  dirgos[f]=1;
                }else{
                  if (((z+2)%4!=2) && futur[f][2]==1){
                    dirgos[f]=2;
                  }else{
                    if (futur[f][0]==1){
                      dirgos[f]=0;
                    }else{
                      dirgos[f]=3;
                    }
                  }
                }
              }else{
                if (((z+2)%4!=2) && futur[f][2]==1){
                  dirgos[f]=2;
                }else{
                  if (((z+2)%4!=1) && futur[f][1]==1){
                    dirgos[f]=1;
                  }else{
                    if (futur[f][3]==1){
                      dirgos[f]=3;
                    }else{
                      dirgos[f]=0;
                    }
                  }
                } 
              }
            }
          }else{
            if (dy<0){
              
              //haut droit
              
              if (dx<=(-dy)){
                
                if (((z+2)%4!=3) && futur[f][3]==1){
                  dirgos[f]=3;
                }else{
                  if (((z+2)%4!=0) && futur[f][0]==1){
                    dirgos[f]=0;
                  }else{
                    if (futur[f][2]==1){
                      dirgos[f]=2;
                    }else{
                      dirgos[f]=1;
                    }
                  }
                }
              
              }else{
                if (((z+2)%4!=0) && futur[f][0]==1){
                  dirgos[f]=0;
                }else{
                  if (((z+2)%4!=3) && futur[f][3]==1){
                    dirgos[f]=3;
                  }else{
                    if (futur[f][1]==1){
                      dirgos[f]=1;
                    }else{
                      dirgos[f]=2;
                    }
                  }
                }
              }
            }else{
              
              //bas droit
              if (dx<=dy){
                if (((z+2)%4!=1) && futur[f][1]==1){
                  dirgos[f]=1;
                }else{
                  if (((z+2)%4!=0) && futur[f][0]==1){
                    dirgos[f]=0;
                  }else{
                    if (futur[f][2]==1){
                      dirgos[f]=2;
                    }else{
                      dirgos[f]=3;
                    }
                  }
                }
                
              }else{
                if (((z+2)%4!=0) && futur[f][0]==1){
                  dirgos[f]=0;
                }else{
                  if (((z+2)%4!=1) && futur[f][1]==1){
                    dirgos[f]=1;
                  }else{
                    if (futur[f][3]==1){
                      dirgos[f]=3;
                    }else{
                      dirgos[f]=2;
                    }
                  }
                }
              }
            }
          }
        }else{
          
          for(int i=0;i<4;i++){
            if ((futur[f][i]==1) && (i!=((z+2)%4))){
              dirgos[f]=i;
            }
          }
        }

        break;
      case 1:
        
        if (nextpath[0]==1){
          if (r<8){
            dirgos[f]=(z+3)%4;
          }else{
            if(nextpath[1]==1){
              if (r==8){
                dirgos[f]=z;
              }else{
                if(nextpath[2]==1){
                  dirgos[f]=(z+1)%4;
                }else{
                  dirgos[f]=(z+3)%4;
                }
              }
            }else{
              if(nextpath[2]==1){
                dirgos[f]=(z+1)%4;
              }else{
                dirgos[f]=(z+3)%4;
              }
            }
          }
          
        }else{
          if (nextpath[1]==1){
            if (r<9){
              dirgos[f]=z;
            }else{
              if(nextpath[2]==1){
                dirgos[f]=(z+1)%4;
              }else{
                dirgos[f]=z;
              }
            }
          }else{
            if(nextpath[2]==1){
              dirgos[f]=(z+1)%4;
            }else{
              dirgos[f]=(z+1)%4;
            }
          } 





        }
        break;
      case 2:
        if (nextpath[2]==1){
          if (r<8){
            dirgos[f]=(z+1)%4;
          }else{
            if(nextpath[1]==1){
              if (r==8){
                dirgos[f]=z;
              }else{
                if(nextpath[0]==1){
                  dirgos[f]=(z+3)%4;
                }else{
                  dirgos[f]=(z+1)%4;
                }
              }
            }else{
              if(nextpath[0]==1){
                dirgos[f]=(z+3)%4;
              }else{
                dirgos[f]=(z+1)%4;
              }
            }
          }
          
        }else{
          if (nextpath[1]==1){
            if (r<9){
              dirgos[f]=z;
            }else{
              if(nextpath[0]==1){
                dirgos[f]=(z+3)%4;
              }else{
                dirgos[f]=z;
              }
            }
          }else{
            if(nextpath[0]==1){
              dirgos[f]=(z+3)%4;
            }else{
              dirgos[f]=(z)%4;
            }
          } 





        }
        break;




      case 3:
        if(nextpath[1]==1){
          if(r<3){
            dirgos[f]=z;
          }else{
            if(nextpath[0]==1){
              if(r<6){
                dirgos[f]=(z+3)%4;
              }else{
                if(nextpath[2]==1){
                  dirgos[f]=(z+1)%4;
                }else{
                  dirgos[f]=(z)%4;
                }
              }
            }else{
              if(nextpath[2]==1){
                dirgos[f]=(z+1)%4;
              }else{
                dirgos[f]=z;
              }
            }
            
          }
        }else{
          if(nextpath[0]==1){
            if(r<6){
              dirgos[f]=(z+3)%4;
            }else{
              if(nextpath[2]==1){
                dirgos[f]=(z+1)%4;
              }else{
                dirgos[f]=(z+3)%4;
              }
            }
          }else{
            if(nextpath[2]==1){
              dirgos[f]=(z+1)%4;
            }else{
              dirgos[f]=(z)%4;
            }
          }
        }
        break;
      case 4:
          if ((futur[f][0]+futur[f][1]+futur[f][2]+futur[f][3])>2){
          int dx=xz-x;
          int dy=yz-y;
          if (dx>0){
            if (dy>0){
              // haut gauche
              if (dx<=dy){
                
                if (((z+2)%4!=3) && futur[f][3]==1){
                  dirgos[f]=3;
                }else{
                  if (((z+2)%4!=2) && futur[f][2]==1){
                    dirgos[f]=2;
                  }else{
                    if (futur[f][0]==1){
                      dirgos[f]=0;
                    }else{
                      dirgos[f]=1;
                    }
                    
                  }
                }
                
              }else{
                if (((z+2)%4!=2) && futur[f][2]==1){
                  dirgos[f]=2;
                }else{
                  if (((z+2)%4!=3) && futur[f][3]==1){
                    dirgos[f]=3;
                  }else{
                    if (futur[f][1]==1){
                      dirgos[f]=1;
                    }else{
                      dirgos[f]=0;
                    }
                  }
                }
              }
            }else{
              // bas gauche
              if (dx<=(-dy)){
                if ((((z+2)%4)!=1) && (futur[f][1]==1)){
                  dirgos[f]=1;
                }else{
                  if (((z+2)%4!=2) && futur[f][2]==1){
                    dirgos[f]=2;
                  }else{
                    if (futur[f][0]==1){
                      dirgos[f]=0;
                    }else{
                      dirgos[f]=3;
                    }
                  }
                }
              }else{
                if (((z+2)%4!=2) && futur[f][2]==1){
                  dirgos[f]=2;
                }else{
                  if (((z+2)%4!=1) && futur[f][1]==1){
                    dirgos[f]=1;
                  }else{
                    if (futur[f][3]==1){
                      dirgos[f]=3;
                    }else{
                      dirgos[f]=0;
                    }
                  }
                } 
              }
            }
          }else{
            if (dy>0){
              //haut droit
              
              if (dx>=(-dy)){
                
                if (((z+2)%4!=3) && futur[f][3]==1){
                  dirgos[f]=3;
                }else{
                  if (((z+2)%4!=0) && futur[f][0]==1){
                    dirgos[f]=0;
                  }else{
                    if (futur[f][2]==1){
                      dirgos[f]=2;
                    }else{
                      dirgos[f]=1;
                    }
                  }
                }
              
              }else{
                if (((z+2)%4!=0) && futur[f][0]==1){
                  dirgos[f]=0;
                }else{
                  if (((z+2)%4!=3) && futur[f][3]==1){
                    dirgos[f]=3;
                  }else{
                    if (futur[f][1]==1){
                      dirgos[f]=1;
                    }else{
                      dirgos[f]=2;
                    }
                  }
                }
              }
            }else{
              // bas droit
              if (dx>=dy){
                if (((z+2)%4!=1) && futur[f][1]==1){
                  dirgos[f]=1;
                }else{
                  if (((z+2)%4!=0) && futur[f][0]==1){
                    dirgos[f]=0;
                  }else{
                    if (futur[f][2]==1){
                      dirgos[f]=2;
                    }else{
                      dirgos[f]=3;
                    }
                  }
                }
                
              }else{
                if (((z+2)%4!=0) && futur[f][0]==1){
                  dirgos[f]=0;
                }else{
                  if (((z+2)%4!=1) && futur[f][1]==1){
                    dirgos[f]=1;
                  }else{
                    if (futur[f][3]==1){
                      dirgos[f]=3;
                    }else{
                      dirgos[f]=2;
                    }
                  }
                }
              }
            }
          }
        }else{
          
          for(int i=0;i<4;i++){
            if ((futur[f][i]==1) && (i!=((z+2)%4))){
              dirgos[f]=i;
            }
          }
        }

        break;

      case 5:
        
        if(mess=='0' || mess=='1'|| mess=='2'|| mess=='3'){
          if (futur[4][0]+futur[4][1]+futur[4][2]+futur[4][3]>2){
            if (futur[f][(int)mess-48] ==1){
              dirgos[f] = futurz;
              mess='9';
              count=30;
              
            }

          }
          if (futur[4][0]+futur[4][1]+futur[4][2]+futur[4][3]>1){
            if (futur[f][(int)mess-48] !=1){
              dirgos[f] = 4;
              mess='9';
              futurz=(int)mess-48;
              stepghost(z,f,0,false,x,y);
              count=100;
              
            }

            if (futur[f][(int)mess-48] == 1){
              dirgos[f] = (int)mess-48;
              futurz=4;
              count=100;
              mess='9';
            }if (futur[f][dirgos[f]]==0){
              dirgos[f]=4;
            }
          }
          
        }else{
          if (count<1){
            stepghost(z,f,0,false,x,y);
          }else{if (futur[4][0]+futur[4][1]+futur[4][2]+futur[4][3]>1){
            if (futur[4][futurz] !=1){
              dirgos[f] = 4;
            }
            if (futur[4][futurz] == 1){
              dirgos[f] = futurz;
            }
          }/**/}
          
        }
        break;
    }
  }else{
    
  }
}

// Compute available moves (futur[f]) and nextpath relative to `z`
void nextstep(int x, int y ,int z,int f){
  futur[f][0]=0;
  futur[f][1]=0;
  futur[f][2]=0;
  futur[f][3]=0;
  numpath=-1;
  if ((!appartient(x-3,y-2) &&  !appartient(x-3,y+2)) && !appartient(x-3,y)){
    futur[f][0]=1;
    numpath++;
    
  }
  if (((!appartient(x-2,y-3)) &&  (!appartient(x+2,y-3))) && (!(appartient(x,y-3))) ){
    futur[f][1]=1;
    numpath++;
    
  }
  if (!appartient(x+3,y-2) &&  !appartient(x+3,y+2) && !(appartient(x+3,y))){
    futur[f][2]=1;
    numpath++;
    
  }
  if (!appartient(x-2,y+3) &&  !appartient(x+2,y+3) && !(appartient(x,y+3))){
    futur[f][3]=1;
    numpath++;
    
  }
  
  nextpath[0]=futur[f][(z+3)%4];
  nextpath[1]=futur[f][z];
  nextpath[2]=futur[f][(z+1)%4];
  nextpath[3]=futur[f][(z+2)%4];
  
}









// Test whether coordinate (x,y) lies inside maze walls
bool appartient(int x,int y){
  
  for (int i=2;i<s;i++){
    if((cordmaze[i][0]<=x && x<(cordmaze[i][0]+cordmaze[i][2])) && (cordmaze[i][1]<=y && y<(cordmaze[i][1]+cordmaze[i][3]))){
      return true;

    }
    if((((64-cordmaze[i][0])-cordmaze[i][2])<=x && x<(64-cordmaze[i][0])) && (cordmaze[i][1]<=y && y<(cordmaze[i][1]+cordmaze[i][3]))){
      return true;
    }
    if((cordmaze[i][0]<=x && x<(cordmaze[i][0]+cordmaze[i][2])) && ((64-cordmaze[i][1]-cordmaze[i][3])<=y && y<(64-cordmaze[i][1]))){
      return true;
    }
    if(((64-cordmaze[i][0]-cordmaze[i][2])<=x && x<(64-cordmaze[i][0]))  && ((64-cordmaze[i][1]-cordmaze[i][3])<=y && y<(64-cordmaze[i][1]))){
      return true;
    }
  }
  if (x<2 || x>61 || y<2 || y>61){
    return true;
  }if (y==29 && ( x<8 || (13<=x && x<=16) || (21<x && x<=41) || (46<x && x<=50) || (56<=x) )){
    return true;
  }
  
  
  
  return false;
}

// Handle collisions between Pac-Man and ghosts; show end screens
void mort(){
  for(int i=0;i<4;i++){
    int dx=x-goxy[i][0];
    if (dx<0){
      dx=-dx;
    }int dy=y-goxy[i][1];
    if (dy<0){
      dy=-dy;
    }
    if (dx<5 && dy<5){
      if (mange==0){
        over=true;
        matrix.setTextSize(1); 
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(8, 28);
        matrix.setTextColor(couleur[7]);
        matrix.println("Game Over");
        matrix.println(" Ghost Win");
      }else{
        goxy[i][0]=32;
        goxy[i][1]=22;
        dirgos[i]=1;
        point = point+200;
      }

    }
  }if (seconde>599){
    over=true;
      matrix.setTextSize(1); 
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(8, 28);
      matrix.setTextColor(couleur[7]);
      matrix.println("Game Over");
      matrix.println("Pacman Win");
    
  };
}








// Draw a small pellet at (x,y)
void pacgomme(int x,int y){
  matrix.fillCircle(x, y, 1,couleur[7]);
}
// Draw a point (used for pellets visuals)
void points(int x,int y){
  matrix.drawPixel(x, y,couleur[7]);
}





// === Arduino setup: initialize hardware and game state ===
void setup(void) {
  matrix.begin();
  Serial.begin(9600);
  Serial1.begin(9600);

  initmaze();
  //testmaze();
  matrix.setTextSize(1); 
  matrix.setCursor(23, 36);
  minute=seconde/600;
  matrix.fillRect(23,36,18,7,couleur[6]);
  matrix.print(minute);
  matrix.setCursor(30, 36);
  matrix.drawPixel(28, 38,couleur[7]);
  matrix.drawPixel(28, 40,couleur[7]);
  matrix.drawRect(29,20,6,1,couleur[7]);
  matrix.drawRect(0,29,2,6,couleur[7]);
  matrix.drawRect(62,29,2,6,couleur[7]);
  ghost(goxy[0][0],goxy[0][1],dirgos[0],c);
  ghost(goxy[1][0],goxy[1][1],dirgos[1],c+1);
  ghost(goxy[2][0],goxy[2][1],dirgos[2],c+2);
  ghost(goxy[3][0],goxy[3][1],dirgos[3],c+3);
  Serial1.println(point);
  sensorValue1 = analogRead(A12);
  sensorValue2 = analogRead(A13);
  pac();
 
}
// === Main loop: input, update, render ===
void loop(void) {
  if (!over){
    if (Serial1.available() > 0) { 
      mess=Serial1.read();
      Serial.println(mess);
      if(mess=='G'){
        co=(int)Serial1.read();
        Serial.println(c);
      }
    }
    if(mange==1){
      for(int i=0;i<4;i++){
        modeghost[i]=basemodeghost[i];
      }
    }if(mange==29){
      for(int i=0;i<4;i++){
        modeghost[i]=4;
      }
    }modeghost[co]=5;
    if(mange!=0){
      mange--;

    }
    for(int i=0;i<5;i++){
      if (gomme[i][2]==1){
        pacgomme(gomme[i][0],gomme[i][1]);
        if(((x)==gomme[i][0]) && (y==gomme[i][1])){
          gomme[i][2]=0;
          mange=50;
          modeghost[0];
          point = point+150;
        }
      }
      
    }
    for (int i=0;i<plen;i++){
      if (pointgomme[i][2]){
        points(pointgomme[i][0],pointgomme[i][1]);
        if(((x)==pointgomme[i][0]) && (y==pointgomme[i][1])){
          pointgomme[i][2]=0;
          point = point+50;
          
        }
      }
    }
    if (ancienpoint!=point){
      Serial1.println(point);
      ancienpoint=point;
    }
    
    pacNextStep(sensorValue1, sensorValue2);
    pagsup(x,y);
    pacmove(z);
    seconde++;
    x=(x+incx)%64;
    y=(y+incy)%64;
    if (x==-1){
      x=63;
    }
    if (y==-1){
      y=63;
    }
    int st=40;
    if (seconde>(4*st)){
      int r=random(10);
      stepghost(dirgos[0],0,modeghost[0],false,x,y);
      if ((((-10)<goxy[0][0]-goxy[1][0] && goxy[0][0]-goxy[1][0]<10 && (-10)<goxy[0][1]-goxy[1][1] && goxy[0][1]-goxy[1][1]<10)||r>7) && goxy[1][1]!=32){
        
        if(mange==0){
          stepghost(dirgos[1],1,1,false,goxy[0][0],goxy[0][1]);
        }else{
          stepghost(dirgos[1],1,modeghost[1],false,x,y);
        }
      }else{
        if(mange==0){
          stepghost(dirgos[1],1,modeghost[1],false,goxy[0][0],goxy[0][1]);
        }else{
          stepghost(dirgos[1],1,modeghost[1],false,x,y);
        }
        
      }
      stepghost(dirgos[2],2,modeghost[2],false,x,y);
      stepghost(dirgos[3],3,modeghost[3],false,x,y);
    }else{
      if (seconde<=st){
        stepghost(dirgos[0],0,0,false,55,5);
        stepghost(dirgos[1],1,0,false,5,5);
        stepghost(dirgos[2],2,0,false,55,5);
        stepghost(dirgos[3],3,0,false,5,5);
      }else{
        if(seconde<=(2*st)){
          stepghost(dirgos[1],1,0,false,5,5);
          stepghost(dirgos[0],0,modeghost[0],false,x,y);
          stepghost(dirgos[2],2,0,false,55,5);
          stepghost(dirgos[3],3,0,false,5,5);
        }else{
          if(seconde<=(3*st)){
            stepghost(dirgos[0],0,modeghost[0],false,x,y);
            stepghost(dirgos[1],1,modeghost[1],false,goxy[0][0],goxy[0][1]);
            stepghost(dirgos[2],2,0,false,55,5);
            stepghost(dirgos[3],3,0,false,5,5);
          }else{
            if(seconde<=(4*st)){
              stepghost(dirgos[0],0,modeghost[0],false,x,y);
              stepghost(dirgos[1],1,modeghost[1],false,goxy[0][0],goxy[0][1]);
              stepghost(dirgos[2],2,modeghost[2],false,x,y);
              stepghost(dirgos[3],3,0,false,5,5);
            }
            
          }
          
        }
        
      }
    }//mort();
    if(!over){
    if (count!=0){
      count--;
    }
    
    gosmove(dirgos);
    for (int i=0;i<4;i++){
      pagsup(goxy[i][0],goxy[i][1]);
      
    }
    for (int i=0;i<4;i++){
      goxy[i][0]=(goxy[i][0]+incgoxy[i][0])%64;
      goxy[i][1]=(goxy[i][1]+incgoxy[i][1])%64;
      if(goxy[i][0]==-1){
        goxy[i][0]=63;
      }
      if(goxy[i][1]==-1){
        goxy[i][1]=63;
      }

    }
    //tp pacman
    //tp++;
    if (tp==50){
      x=random(5,59);
      y=random(5,59);
      tp=0;
    }

    /*
    */

    
    matrix.setCursor(23, 36);
    minute=seconde/600;
    matrix.fillRect(23,36,18,7,couleur[6]);
    matrix.print(minute);
    matrix.setCursor(30, 36);
    matrix.drawPixel(28, 38,couleur[7]);
    matrix.drawPixel(28, 40,couleur[7]);
    if ((seconde%600)<100){
      matrix.print(0);
    }
    matrix.print((seconde/10)%60);
    matrix.drawRect(29,20,6,1,couleur[7]);
    matrix.drawRect(0,29,2,6,couleur[7]);
    matrix.drawRect(62,29,2,6,couleur[7]);
    ghost(goxy[0][0],goxy[0][1],dirgos[0],c);
    ghost(goxy[1][0],goxy[1][1],dirgos[1],c+1);
    ghost(goxy[2][0],goxy[2][1],dirgos[2],c+2);
    ghost(goxy[3][0],goxy[3][1],dirgos[3],c+3);




    
    /**/

    pac();
    
    sensorValue1 = analogRead(A12);
    sensorValue2 = analogRead(A13);
    }
  }
  delay(100);
    
    

}
