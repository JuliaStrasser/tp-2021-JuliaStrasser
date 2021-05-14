#include <windows.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <wingdi.h>
#include <cmath>
#include <stdlib.h> 

#define ID_PRZYCISK1 501
#define ID_PRZYCISK2 502

LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
HWND button1,button2;
HDC hdcWindow;
//HWND g_hPrzycisk;

using namespace std;

struct point
{
	float x,y,z;
};

point Points[2062];

void int_to_lpcstr(float number, char *cache)
{
    wsprintf(cache, "%d", number);
}



void FileReading()// WCZYTYWANIE Z PLIKU
{
	ifstream input("outputRobotForwardB01.log");
	//string word;
	string nrStr;
	//float nrStr;
	//LPCSTR letter[30];
	char numberChar[30];
	int i=0;
	int lineNr=0;
	
	while(input>>nrStr)
	{
		
        LPSTR numberLPCSTR=&nrStr[0];
        
		float numberFloat=atof(nrStr.c_str()); // konwersja stringa na floata
		numberFloat *= 1000;
		numberFloat = ceil(numberFloat);
		numberFloat /= 1000; 	
		
		if((i%12)==0)
		{
			Points[lineNr].x= numberFloat;
			//MessageBox( NULL,numberLPCSTR,"x", MB_ICONEXCLAMATION );
		}
		if((i%12)==1)
		{
			Points[lineNr].y= numberFloat;
			//MessageBox( NULL,numberLPCSTR,"y", MB_ICONEXCLAMATION );
		}
		if((i%12)==2)
		{
			Points[lineNr].z=numberFloat;
			lineNr++;
			//MessageBox( NULL,numberLPCSTR,"z", MB_ICONEXCLAMATION );
		}
		
	 i++;
		
	}

    /*char liczba[10000];
    int_to_lpcstr(i, liczba);
	LPCSTR p= liczba;
	MessageBox( NULL,p,"nowy numerek z pliku", MB_ICONEXCLAMATION ); liczby w komunikacie*/
}

void calibration(int& nPixFor1x,int& nPixFor1y)
{
	float xMax=0;
    float yMax=0;
    
    for(int i=0;i<2062;i++)
    {
    	if(Points[i].x>xMax)
    	{
    		xMax=Points[i].x;
		}
		if(Points[i].y>yMax)
    	{
    		yMax=Points[i].y;
		}
	}
	nPixFor1x= 400/xMax;
	nPixFor1y= 150/yMax;
	/*string xMAXstr,yMAXstr;
	xMAXstr=to_string(xMax);
	MessageBox( NULL,xMAXstr,"x", MB_ICONEXCLAMATION );*/
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	   
	
	FileReading();
	
   
    // WYPELNIANIE STRUKTURY
    WNDCLASSEX wc;
   
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
   
    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }
   
    // TWORZENIE OKNA
    HWND hwnd;
   
    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 1000, 500, NULL, NULL, hInstance, NULL );                     		 //TWORZENIE OKNA
   

    button1 = CreateWindowEx( 0, "BUTTON", "Pierwszy", WS_CHILD | WS_VISIBLE | WS_BORDER,50,50,150, 30,
	 hwnd, (HMENU) ID_PRZYCISK1, hInstance, NULL );                                                              		 //TWORZENIE PRZYCISKU
	 
	button2= CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "Drugi", WS_CHILD | WS_VISIBLE |
	WS_BORDER, 50, 100, 150, 30, hwnd,( HMENU ) ID_PRZYCISK2, hInstance, NULL );
	

    ShowWindow( hwnd, nCmdShow ); // Pokaz okienko...
    UpdateWindow( hwnd );
   
    // Petla komunikatów
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }
    
     
  
    return Komunikat.wParam;
}

// OBSLUGA ZDARZEN
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	//MALOWANIE PROSTYCH LINII
	
	hdcWindow= GetDC(hwnd);
	POINT oldPoint;
	
	MoveToEx(hdcWindow,400,50,& oldPoint);
	LineTo(hdcWindow,400,200);
	LineTo(hdcWindow,800,200);
	
	//Malowanie piksela
	//SetPixel( hdcWindow, 600, 100, 0x000000FF );
	
	//MALOWANIE WYKRESU
	int nPixFor1x=0;    //ile pikseli przypada na 1 jednostke na osi x
	int nPixFor1y=0; 
	int xPrim;
	int yPrim;
	
	calibration(nPixFor1x,nPixFor1y);
	MoveToEx(hdcWindow,400,200,NULL);
	  
	for(int i=0;i<2062;i++)
	{
		xPrim=(Points[i].x * nPixFor1x)+400;   //pikselowe wspolrzedne punktow
		yPrim=-(Points[i].y * nPixFor1y)+200;
		
		/*if(i=0)
		{
			MoveToEx(hdcWindow,xPrim,yPrim,& oldPoint);
		}
		else
		{*/
			LineTo(hdcWindow,xPrim,yPrim);
			MoveToEx(hdcWindow,xPrim,yPrim,NULL);  //?
		//}
	}
	
    switch( msg )
    {
    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;
       
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    case WM_COMMAND:
    	switch(wParam)
    	{
    		case ID_PRZYCISK1:
    			MessageBox(hwnd,"Wcisnales przycisk 1","Test",MB_ICONINFORMATION);
    			break;
    		case ID_PRZYCISK2:
    			MessageBox(hwnd,"Wcisnales przycisk 2","Test",MB_ICONINFORMATION);
    			break;
    		default:
    			MessageBox(hwnd,"Co ty czlowieku wcisnales ?","Test",MB_ICONINFORMATION);
    			break;
		}
		break;
    default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }
    
   
    return 0;
}
