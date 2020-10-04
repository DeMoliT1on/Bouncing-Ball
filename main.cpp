#include <windows.h>
#define Show(Window) RedrawWindow(Window,0,0,0);ShowWindow(Window,SW_SHOW);
#define AppName "BouncingBall1"
#define Caption "Bouncing Ball ..."
char BCX_STR [1024*1024];
static int     BCX_GetDiaUnit;
static int     BCX_cxBaseUnit;
static int     BCX_cyBaseUnit;
static int     BCX_ScaleX;
static int     BCX_ScaleY;
static HANDLE  Form1;
double  MIN (long,long);
double  MAX (long,long);
int     WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR, int);
void    FormLoad (HANDLE);
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
// create the form and show it (somewhat older style)
void FormLoad (HANDLE hInst)
{
  // get the scale factors
  BCX_ScaleX = BCX_cxBaseUnit/4;
  BCX_ScaleY = BCX_cyBaseUnit/8;
  // now the form
  Form1=CreateWindow(AppName,Caption,
    DS_MODALFRAME|WS_POPUP|WS_CAPTION|WS_SYSMENU,
    10*BCX_ScaleX,20*BCX_ScaleY,250*BCX_ScaleX,175*BCX_ScaleY,NULL,
    (HMENU)NULL,(HINSTANCE)hInst,NULL);
  Show((HWND)Form1);
}
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  static  HANDLE  hBitmap;
  static  HBRUSH  hBrush;
  static  HDC  hdc;
  static  HDC  hdcMem;
  static int cxClient;
  static int cyClient;
  static int xCenter;
  static int yCenter;
  static int cxTotal;
  static int cyTotal;
  
  while(1)
  {
    if (Msg == WM_CREATE)
    {
      hdc = GetDC(hWnd);
      xPixel = GetDeviceCaps(hdc,ASPECTX);
      yPixel = GetDeviceCaps(hdc,ASPECTY);
      ReleaseDC(hWnd,hdc);
      return 0;
      break;
    }
    // draw the ball
    if (Msg == WM_SIZE)
    {
      xCenter = (cxClient=LOWORD(lParam))/2;
      yCenter = (cyClient=HIWORD(lParam))/2;
      nScale = (int)MIN(cxClient*xPixel,cyClient*yPixel)/16;
      cxRadius = nScale/xPixel;
      cyRadius = nScale/yPixel;
      if (hBitmap)
      {
        DeleteObject(hBitmap);
      }
      hdc = GetDC(hWnd);
      hdcMem = CreateCompatibleDC(hdc);
      hBitmap = CreateCompatibleBitmap(hdc,cxTotal,cyTotal);
      ReleaseDC(hWnd,hdc);
      SelectObject(hdcMem,hBitmap);
      Ellipse(hdcMem,cxMove,cyMove,cxTotal-cxMove,cyTotal-cyMove);
      DeleteDC(hdcMem);
      DeleteObject(hBrush);
      return 0;
      break;
    }
    // move the ball
    if (Msg == WM_TIMER)
    {
      if (!hBitmap)
      {
        break;
      }
      hdc = GetDC(hWnd);
      hdcMem = CreateCompatibleDC(hdc);
      SelectObject(hdcMem,hBitmap);
      BitBlt(hdc,xCenter-cxTotal/2,yCenter-cyTotal/2,cxTotal,cyTotal,hdcMem,0,0,SRCCOPY);
      ReleaseDC(hWnd,hdc);
      DeleteDC(hdcMem);
      xCenter += cxMove;
      yCenter += cyMove;
      return 0;
      break;
    }
    // clean up and exit program
    if (Msg == WM_DESTROY)
    {
      if (hBitmap)
      {
        DeleteObject(hBitmap);
      }
      KillTimer((HWND)Form1,1);
      PostQuitMessage(0);
      return 0;
    }
    break;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR CmdLine, int CmdShow)
{
  int r,g,b;
  static  WNDCLASS  Wc;
  memset(&Wc,0,sizeof(Wc));
  static  MSG  Msg;
  memset(&Msg,0,sizeof(Msg));
  Wc.lpszMenuName=NULL;
  Wc.lpszClassName=AppName;
  RegisterClass(&Wc);
  FormLoad(hInst);
  // 50ms here, lower value gives higher speed
  SetTimer((HWND)Form1,1,10,NULL);
  // ye olde event message loop

  while(GetMessage(&Msg,NULL,0,0))
  {
    if (!IsWindow((HWND)Form1)||!IsDialogMessage((HWND)Form1,&Msg))
    {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  }
  return Msg.wParam;
}
