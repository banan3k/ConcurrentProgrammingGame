#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <time.h>

#define klucz 11

Display *mydisplay;
Window mywindow;
XSetWindowAttributes mywindowattributes;
XGCValues mygcvalues;
Colormap mycolormap;
int mydepth;
int myscreen;
XEvent myevent;
GC mygc;
Visual *myvisual;


char *adres;
int turns=0;
int whatPlayer;
char players[100];
int plik;
int whatChoosedRect=-1;
void changeGraphic(int x, int y);
int wspolrzedneMenuGlobalne[5][2];
int mazanieX=-1, mazanieY=-1;
int dopasowany=0;
int plansza[1000][1000];

void putForCheck(int x, int y)
{
    int i=0, i2=0;
    
    for(i=0; i<wspolrzedneMenuGlobalne[whatChoosedRect][0]; i++)
    {
        for(i2=0; i2<wspolrzedneMenuGlobalne[whatChoosedRect][1]; i2++)
        {
            plansza[x+i][y+i2]=1;
        }
    }
    
}
void putForCheck2(int x, int y, int w, int h)
{
    int i=0, i2=0;
    
    for(i=0; i<w; i++)
    {
        for(i2=0; i2<h; i2++)
        {
            plansza[x+i][y+i2]=1;
        }
    }
    
}
int checkIfCanPut(int x, int y)
{
    int i=0, i2=0;
    if(x+wspolrzedneMenuGlobalne[whatChoosedRect][0]>1000 || y+wspolrzedneMenuGlobalne[whatChoosedRect][1]>1000)
        return 1;
    for(i=0; i<wspolrzedneMenuGlobalne[whatChoosedRect][0]; i++)
    {
        for(i2=0; i2<wspolrzedneMenuGlobalne[whatChoosedRect][1]; i2++)
        {
            if(plansza[x+i][y+i2]==1)
                return 1;
        }
    }
    
    
    return 0;
}

XColor xcolors[5];
void prepeareColors()
{
    int i;
    
    xcolors[0].red = 3200000;
    xcolors[0].green = 0;
    xcolors[0].blue = 32000;
    xcolors[0].flags = DoRed | DoGreen | DoBlue;
    
    xcolors[1].red = 620000;
    xcolors[1].green = 0;
    xcolors[1].blue = 32000;
    xcolors[1].flags = DoRed | DoGreen | DoBlue;
    
    xcolors[2].red = 32000;
    xcolors[2].green = 62000;
    xcolors[2].blue = 0;
    xcolors[2].flags = DoRed | DoGreen | DoBlue;
    
    xcolors[3].red = 62000;
    xcolors[3].green = 62000;
    xcolors[3].blue = 0;
    xcolors[3].flags = DoRed | DoGreen | DoBlue;
    
    xcolors[4].red = 32000;
    xcolors[4].green = 32000;
    xcolors[4].blue = 32000;
    xcolors[4].flags = DoRed | DoGreen | DoBlue;
    
    for(i=0; i<5; i++)
        XAllocColor(mydisplay, mycolormap, &xcolors[i]);
}

void setMenu()
{
    prepeareColors();
    srand(time(0));
    XColor xcolor2;
    XColor xcolor;
    xcolor.red = 32000;
    xcolor.green = 0;
    xcolor.blue = 0;
    xcolor.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(mydisplay, mycolormap, &xcolor);
    
    XNextEvent(mydisplay,&myevent);
    
    XSetForeground(mydisplay,mygc,500);
    XFillRectangle(mydisplay,mywindow,mygc,10,0,1000,200);

    XFlush(mydisplay);  
    int i;
    for(i=0; i<5; i++)
    {
        
        xcolor.red = 500000+4000*i;
        xcolor.flags = DoRed | DoGreen | DoBlue;
        XAllocColor(mydisplay, mycolormap, &xcolor);
        XSetForeground(mydisplay,mygc,xcolor.pixel);
        XFillRectangle(mydisplay,mywindow,mygc,10+i*200,0,200,200);
        XFlush(mydisplay); 
        xcolor2.red = rand() % 64000;
        xcolor2.green = rand() % 64000;
        xcolor2.blue = rand() % 64000;
        XAllocColor(mydisplay, mycolormap, &xcolor2);
        XSetForeground(mydisplay,mygc,xcolors[i].pixel);
        int xT, yT;
        
        xT=(100-(wspolrzedneMenuGlobalne[i][0]/2));
        yT=(100-(wspolrzedneMenuGlobalne[i][1])/2);
        XFillRectangle(mydisplay,mywindow,mygc,10+i*200+xT,yT,wspolrzedneMenuGlobalne[i][0],wspolrzedneMenuGlobalne[i][1]);
        
        XFlush(mydisplay);  
    }
}

void swapMenu(int id)
{
    XColor xcolor;
    xcolor.green = 500000;
    xcolor.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(mydisplay, mycolormap, &xcolor);
    XSetForeground(mydisplay,mygc,xcolor.pixel);
    XFillRectangle(mydisplay,mywindow,mygc,10+id*200,0,200,200);

    XFlush(mydisplay);  
    
    XColor xcolor2;
    xcolor2.red = rand() % 64000;
    xcolor2.green = rand() % 64000;
    xcolor2.blue = rand() % 64000;
    XAllocColor(mydisplay, mycolormap, &xcolor2);
    XSetForeground(mydisplay,mygc,xcolor2.pixel);
    int xT=(100-(wspolrzedneMenuGlobalne[id][0]/2));
    int yT=(100-(wspolrzedneMenuGlobalne[id][1])/2);
    XFillRectangle(mydisplay,mywindow,mygc,10+id*200+xT,yT,wspolrzedneMenuGlobalne[id][0],wspolrzedneMenuGlobalne[id][1]);
        
    XFlush(mydisplay);  
}
int wrong=0;
int whatChoosed(int x, int y)
{
    
    int i=0;
    if(whatChoosedRect==-1)
    {
        if(y<=200)
        {
            for(i=0; i<5; i++)
            {
                int temp=i*200+10;
                if(x>=(temp) && x<=temp+200)
                {
                    swapMenu(i);
                    return i;
                }
            }
        }
    }
    else
    {
        if(y>200)
        {
            changeGraphic(x,y);
            return 0;
        }
    }
    return -1;
}

void setGraphical()
{
    mydisplay = XOpenDisplay("");
    
    myscreen = DefaultScreen(mydisplay);
    myvisual = DefaultVisual(mydisplay,myscreen);
    mydepth = DefaultDepth(mydisplay,myscreen);
    
    mywindowattributes.background_pixel = XWhitePixel(mydisplay,myscreen);
    mywindowattributes.override_redirect = False;
   
    mywindow = XCreateWindow(mydisplay,XRootWindow(mydisplay,myscreen),
                            10,100,1000,1000,10,mydepth,InputOutput,
                            myvisual,CWBackPixel|CWOverrideRedirect,
                            &mywindowattributes);
     
    XSelectInput(mydisplay,mywindow,ExposureMask|KeyPressMask|ButtonReleaseMask|ButtonPressMask|ButtonMotionMask);
    
    mycolormap = DefaultColormap(mydisplay,myscreen);                 
                            
    XMapWindow(mydisplay,mywindow);
   
    mygc = DefaultGC(mydisplay,myscreen);
 
    setMenu();

}


void koniec()
{
    int liczbaGraczy=0;
	if(whatPlayer==-1)
	{   
	    remove("plik.kom");
	    remove("plansza.kom");
        remove("graphic.kom");
        remove("menu.kom");
	}
    else
    {
        
        plik = open("plik.kom", O_RDWR, 0744);
		read(plik, players,10);
        close(plik);
        liczbaGraczy= strlen(players);
        printf("liczba : %d", liczbaGraczy);
        if(liczbaGraczy<=1)
        {
            remove("plik.kom");
           remove("plansza.kom");
            remove("graphic.kom");
            remove("menu.kom");
        }
        else
        {
            int i=0; 
            for(i=0; i<strlen(players); i++)
            {
               if(whatPlayer==(int)(players[i])-48)
                {
                   players[i]='\0';
                   if(i+1<strlen(players))     
                    players[i]=players[i+1];
                }

            }
            plik = open("plik.kom", O_RDWR | O_TRUNC, 0744);
            write(plik, players,strlen(players));
            close(plik);
        }
    }
 
    exit(0);
}   

int whatPlayerToStart=0;

int passTurn=0;

char wspolrzedne[1000];

int tempX=0;
void putOthersGraphic(int x, int y, int w, int h)
{
   // printf("teraz :%d %d\n", x,y);

    int id=0, i;
    for(i=0; i<5; i++)
    {
       
        if(wspolrzedneMenuGlobalne[i][0]==w && wspolrzedneMenuGlobalne[i][1]==h)
            id=i;
    }
    
    //XSetForeground(mydisplay,mygc,xcolors[id].pixelxcolors[id].pixel);
    tempX+=50;
    
    putForCheck2(x,y,w,h);
    
    XFillRectangle(mydisplay,mywindow,mygc,x,y,w,h);
  
    XFlush(mydisplay);  
}

void updateGraphic()
{
    int i;
    
    plik = open("graphic.kom", O_RDONLY, 0744);
	read(plik, wspolrzedne,1000);
    close(plik);
    char *argumenty;
    char *argumenty2;
    //printf("%s\n", wspolrzedne);
    
    argumenty2 = strtok(wspolrzedne,";");
    
    char argumentyGraczy[1000][1000];
    int idForArgument=0, licznik=0;
    for(i=0; i<strlen(wspolrzedne); i++)
    {
        if(wspolrzedne[i]=='+')
        {
           idForArgument++;
            licznik=0;
        }
        else
        {
            argumentyGraczy[idForArgument][licznik]=wspolrzedne[i];
            licznik++;
        }
    }
	
    int i2=0;
    int licznik2=0;
    int globalID=0;
    char allCont[100][100][100];
    allCont[0][0][0]='a';
    int howMuchAnswers=0;
    for(i=0; i<idForArgument; i++)
    {
        globalID=0;
        licznik2=0;
        if(strlen(argumentyGraczy[i])>0)
        {
            for(i2=0; i2<strlen(argumentyGraczy[i]); i2++)
            {
                if(argumentyGraczy[i][i2]!=',')
                {
                    allCont[i][globalID][licznik2]=argumentyGraczy[i][i2];
                    licznik2++;
                }
                else
                {
                    licznik2=0;
                    globalID++;
                  
                }
            }
            howMuchAnswers++;
            //printf("\n");
        }
       
       
    }

    for(i=0; i<howMuchAnswers; i++)
    {
        printf("%d  \n", i);
        putOthersGraphic(atoi(allCont[i][0]),atoi(allCont[i][1]),atoi(allCont[i][2]),atoi(allCont[i][3]));
    }
   
    
}


int actualX=12, actualY=1, actualW=0, actualH=0;

int doneSaving=0;
void saveGraphic()
{
    plik = open("graphic.kom",  O_WRONLY | O_APPEND, 0744);
    char playerToFile[4][100];
    sprintf(playerToFile[0],"%d", actualX);
    sprintf(playerToFile[1],"%d", actualY);
    sprintf(playerToFile[2],"%d", actualW);
    sprintf(playerToFile[3],"%d", actualH);
   
    int i=0;
    for(i=0; i<4; i++)
    {
        if(i<3)
            strcat(playerToFile[i],",");
        else
            strcat(playerToFile[i],"+");
        write(plik, playerToFile[i],strlen(playerToFile[i]));
    }
    close(plik); 
    
    putForCheck(actualX, actualY);
    mazanieX=-1;
    mazanieY=-1;
    dopasowany=0;

    if(doneSaving==1)
    {
        remove("zclick.kom");
        doneSaving=0;
    }
                  
    
    whatChoosedRect=-1;
}


void changeGraphic(int x, int y)
{
     
    actualX=x;
    actualY=y;
    actualW=wspolrzedneMenuGlobalne[whatChoosedRect][0];
    actualH=wspolrzedneMenuGlobalne[whatChoosedRect][1];
    
    
    
    XSetForeground(mydisplay,mygc,500+10);
   
    
    
    XFillRectangle(mydisplay,mywindow,mygc,actualX,actualY,actualW,actualH);
  
    XFlush(mydisplay);  
    
    saveGraphic();
}

int checkCloseBy(int x, int y)
{
    int i=0;
    if(x==0 || y==201 || x+wspolrzedneMenuGlobalne[whatChoosedRect][0]>=999 || y+wspolrzedneMenuGlobalne[whatChoosedRect][1]>=999 )
        return 1;
    for(i=0; i<wspolrzedneMenuGlobalne[whatChoosedRect][1]; i++)
    {
        if(plansza[x-1][y+i]==1)
            return 1;
    }
    for(i=0; i<wspolrzedneMenuGlobalne[whatChoosedRect][1]; i++)
    {
        if(plansza[x+1+wspolrzedneMenuGlobalne[whatChoosedRect][0]][y+i]==1)
            return 1;
    }
    for(i=0; i<wspolrzedneMenuGlobalne[whatChoosedRect][0]; i++)
    {
        if(plansza[x+i][y-1]==1)
            return 1;
    }
    for(i=0; i<wspolrzedneMenuGlobalne[whatChoosedRect][0]; i++)
    {
        if(plansza[x+i][y+1+wspolrzedneMenuGlobalne[whatChoosedRect][1]]==1)
            return 1;
    }
    
    return 0;
}
int dopasowaneX=0, dopasowaneY=0;
int przeciaganieProst(int x, int y)
{
    XSetForeground(mydisplay,mygc,xcolors[whatChoosedRect].pixel);
    
    if(mazanieX==0 && x<0)
        x=0;
    if(mazanieY==201 && y<=200)
        y=201;
    if(x>=0 && y>200)
    {
        if(checkIfCanPut(x,y)==0)
        {
            if(mazanieX!=-1)
                XClearArea(mydisplay,mywindow,mazanieX,mazanieY,wspolrzedneMenuGlobalne[whatChoosedRect][0],wspolrzedneMenuGlobalne[whatChoosedRect][1],0);
            XFillRectangle(mydisplay,mywindow,mygc,x,y,wspolrzedneMenuGlobalne[whatChoosedRect][0],wspolrzedneMenuGlobalne[whatChoosedRect][1]);
            XFlush(mydisplay); 
dopasowany=0;
            mazanieX=x;
            mazanieY=y;
            if(checkCloseBy(x,y)==1)
            {
                dopasowany=1;
                dopasowaneX=x;
                dopasowaneY=y;
                return 2;
                
            }
            else
                return 1;
        }
    }
    return 0;
    
     
}
int toJa=0;
int reload=0;
int whatIsGoing=0;
void waitForClick()
{
    int waitForClicking=0; 
    int x, y,xw,yw;
    int licznikMotion=0, licznikMotionOld=0, licznikMotionNew=0;
    int canEnd=0;
    int plikKlik=0;
    
     int yol;
     
    while (waitForClicking!=2)
    {
        
        if(whatIsGoing==0 || whatChoosedRect==-1)
        {
            XCheckMaskEvent(mydisplay, 4 | KeyPressMask, &myevent);
        }
        else
            XNextEvent(mydisplay,&myevent);
        
        yol = open("zclick.kom",  O_RDONLY, 0744); 
        close(yol);
  updateGraphic();
         if(yol==-1 || toJa==1)
         {
//printf("%d plik %d\n", yol, toJa);     
        
if(reload==1)
              {     
                 reload=0;
                 updateGraphic();
		printf("wczytuje!!\n");
		
              }

              switch (myevent.type)
              {

                  case ButtonPress:
                      printf("klikam\n");
                       whatIsGoing=1;

                       toJa=1;
                       plikKlik = open("zclick.kom",  O_CREAT, 0744);

                       close(plikKlik);
                       x=myevent.xbutton.x;

                       y=myevent.xbutton.y;

                       int temp = whatChoosed(x,y);
                       whatChoosedRect=temp;
                      
                       // printf("rr %d", whatChoosedRect);
                       if(x>=0 && y>=0)
                            waitForClicking=1;
                      
                      if(whatChoosedRect==-1)
                      {
                        
                          waitForClicking=2;
                          whatIsGoing=0;
                          toJa=0;
                            printf("ev: %d\n", myevent.type);
                          remove("zclick.kom");
                          // XCheckMaskEvent(mydisplay, 4 | 12, &myevent);
                          
                      while(XCheckMaskEvent(mydisplay, 4 | 12, &myevent))
                      {
                          
                          printf("ev: %d\n", myevent.type);
                      }
                        
                    /*  XNextEvent(mydisplay,&myevent);
                          printf("ev: %d\n", myevent.type);
                            XNextEvent(mydisplay,&myevent);
                                                    printf("ev: %d\n", myevent.type);
                             XNextEvent(mydisplay,&myevent);
                                                    printf("ev: %d\n", myevent.type);*/
                      //     XNextEvent(mydisplay,&myevent);
                         
                        //   XNextEvent(mydisplay,&myevent);
                        //printf("ev: %d", myevent);
                          
                       /*    waitForClicking=2;
                          toJa=0;

                          wrong=1;

                          whatIsGoing=0;

                          whatChoosedRect=-1;
                        */

                          
                         //  whatIsGoing=0;
                     
                            break;
                      }
                      
                       break;
                  case MotionNotify:
                        x=myevent.xmotion.x;
                   
                        y=myevent.xmotion.y;
                     
                        int t=przeciaganieProst(x,y);
                        if(t==2 || (t==1 && dopasowany==1))
                        {
                            canEnd=1;   

                        }
                        else if(dopasowany==0)
                        {
                            canEnd=0;
                        }
                    
                        break;
                  case KeyPress:
                        printf("koniec!\n");
                        XCloseDisplay(mydisplay);
                        koniec();
                        break;

                  case ButtonRelease:
                   //  printf("odpuszczam\n");
                      waitForClicking=2;
                      toJa=0;
                      if(canEnd>=1)
                      {
                          waitForClicking=2;
                          doneSaving=1;

                      }
                      else if(whatIsGoing!=0)
                      {
                          wrong=1;

                          whatIsGoing=0;

                         
                          if(mazanieX!=-1)
                                XClearArea(mydisplay,mywindow,mazanieX,mazanieY,wspolrzedneMenuGlobalne[whatChoosedRect][0],wspolrzedneMenuGlobalne[whatChoosedRect][1],0);
 whatChoosedRect=-1;
                          remove("zclick.kom");

                      }
                  //    updateGraphic();  
                      whatIsGoing=0;
                      break;
              }
        }
        else
        {
          //  printf("nic nie robic\n");
            reload=1;
        }
         
         
     }
    
    int temp;
    if(canEnd==1)
        temp = whatChoosed(dopasowaneX,dopasowaneY);
    else if(mazanieX!=-1)
        temp = whatChoosed(mazanieX,mazanieY);
    whatChoosedRect=temp;
}



void createLoadMenu()
{
    plik = open("menu.kom",  O_CREAT| O_EXCL | O_RDWR, 0744);
    int randSizeOfRect[5][2];
    int i=0, i2=0;
    if(plik!=-1)
    {
        plik = open("menu.kom",  O_RDWR, 0744);
        srand(time(0));
        for(i=0; i<5; i++)
        {
            randSizeOfRect[i][0]=(rand() % 80)+21;
            randSizeOfRect[i][1]=(rand() % 80)+21;
            
            char number[3];
            sprintf(number,"%d", randSizeOfRect[i][0]);
            write(plik, number,strlen(number));
            write(plik, ":",1);
            sprintf(number,"%d", randSizeOfRect[i][1]);
            write(plik, number,strlen(number));
            if(i<4)
                write(plik, ",",1);
            
            wspolrzedneMenuGlobalne[i][0]= randSizeOfRect[i][0];
            wspolrzedneMenuGlobalne[i][1]= randSizeOfRect[i][1];
        }
        
        
      
        
    }
    else
    {
        plik = open("menu.kom",  O_RDWR, 0744);
	    char wspolrzedneMenuTemp[1000];
        read(plik, wspolrzedneMenuTemp,1000);
        int ktoreWspolrzedne=0, idWspolrzednychTemp=0, idTemp=0;
        char wspolrzedneString[3];
        for(i=0; i<strlen(wspolrzedneMenuTemp); i++)
        {
            if(wspolrzedneMenuTemp[i]==',')
            {
                wspolrzedneMenuGlobalne[ktoreWspolrzedne][idWspolrzednychTemp]=atoi(wspolrzedneString);
                ktoreWspolrzedne++;
                idWspolrzednychTemp=0;
                idTemp=0;
            }
            else if(wspolrzedneMenuTemp[i]==':')
            {
                wspolrzedneMenuGlobalne[ktoreWspolrzedne][idWspolrzednychTemp]=atoi(wspolrzedneString);
                idWspolrzednychTemp++;
                idTemp=0;
                for(i2=0; i2<3; i2++)
                    wspolrzedneString[i2]='\0';
               
            }
            else
            {
                 wspolrzedneString[idTemp]=wspolrzedneMenuTemp[i];
                idTemp++;
               
            }
        }
         wspolrzedneMenuGlobalne[ktoreWspolrzedne][idWspolrzednychTemp]=atoi(wspolrzedneString);
    }
    close(plik);

    
}


main()
{
    int l=0, l2=0;
    
    
     if (plik = open("plik.kom", O_CREAT| O_EXCL | O_RDWR, 0744) == -1)
        {
               // perror("blabla podczas tworzenia pliku ");
		
        plik = open("plik.kom", O_RDONLY, 0744);
		read(plik, players,10);
            close(plik);
          // printf("players: %d\n",strlen(players)); 
            int i=0;
            int idForPlayer=0;
            for(i=0; i<strlen(players); i++)
            {
              //  printf("%c\n", (int)(players[i])-48);
                if(idForPlayer==(int)(players[i])-48)
                {
                    idForPlayer++;
                    i=0;
                }
            }
            
            whatPlayer=idForPlayer;
     }
    else
        whatPlayer=0;
    
    plik = open("plik.kom",  O_WRONLY | O_APPEND, 0744);
		//for(i=0; i<9; i++)
        char playerToFile[2];
        playerToFile[0]=whatPlayer+'0';
		write(plik, playerToFile,1);
        close(plik);    
    
	   printf("What player : %d \n",whatPlayer);
   
    
    
    
    //whatPlayer=0;

    plik = open("graphic.kom",  O_CREAT, 0744);
    close(plik);

     createLoadMenu();
    
     plik = open("plik.kom",  O_WRONLY | O_APPEND, 0744);

    setGraphical(); 
  // printf("aaa\n");
        updateGraphic();
    while (1)
    {
  
        waitForClick();
        wrong=0;
          
        whatChoosedRect=-1;
          
		turns++;
    }   
}       
