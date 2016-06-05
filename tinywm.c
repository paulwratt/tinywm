/* TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
  Display * dpy;
  XWindowAttributes attr;
  XButtonEvent start{0};
  XEvent ev;
  Window w = None;
  int close = 0;
   
  if(!(dpy = XOpenDisplay(0x0))) return 1;

  KeyCode XK = XKeysymToKeycode(dpy, XStringToKeysym("x"));
  KeyCode RK = XKeysymToKeycode(dpy, XStringToKeysym("r"));
  KeyCode HK = XKeysymToKeycode(dpy, XStringToKeysym("h"));
  KeyCode FK = XKeysymToKeycode(dpy, XStringToKeysym("f"));
  KeyCode EK = XKeysymToKeycode(dpy, XK_Escape);

  XGrabKey(dpy, XK, Mod1Mask,   DefaultRootWindow(dpy), True, 
    GrabModeAsync, GrabModeAsync);

  XGrabKey(dpy, RK, Mod1Mask,   DefaultRootWindow(dpy), True, 
    GrabModeAsync, GrabModeAsync);

  XGrabKey(dpy, HK, Mod1Mask,   DefaultRootWindow(dpy), True, 
    GrabModeAsync, GrabModeAsync);

  XGrabKey(dpy, FK, Mod1Mask,   DefaultRootWindow(dpy), True, 
    GrabModeAsync, GrabModeAsync);

  XGrabKey(dpy, EK, Mod1Mask,   DefaultRootWindow(dpy), True, 
    GrabModeAsync, GrabModeAsync);

  XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
    ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

  XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
    ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

  for(;;)
  {
    XNextEvent(dpy, &ev);
    switch(ev.type)
    {
    case KeyPress:
      if(ev.xkey.state & Mod1Mask && RK == ev.xkey.keycode)
        system("dmenu_run &");
      else 
      if(ev.xkey.state & Mod1Mask && FK == ev.xkey.keycode && ev.xkey.subwindow != None)
        XRaiseWindow(dpy, ev.xkey.subwindow);
      else 
      if(close == 0 && ev.xkey.state & Mod1Mask && XK == ev.xkey.keycode && ev.xkey.subwindow != None)
      {
	close = 1;
        XKillClient(dpy, ev.xkey.subwindow);
        ev.xkey.subwindow = None;
      }
      else 
      if(ev.xkey.state & Mod1Mask && HK == ev.xkey.keycode)
      {
        if(ev.xkey.subwindow != None && w == None)
        {
          w = ev.xkey.subwindow;
          XUnmapWindow(dpy, ev.xkey.subwindow);
        }
        else 
        if(ev.xkey.subwindow == None && w != None)
        {
          XMapWindow(dpy, w);
          w = None;
        }
      }
      else 
      if(ev.xkey.state & Mod1Mask && EK == ev.xkey.keycode)
        return 0;
    case KeyRelease:
      close = 0;
      break;
    case ButtonPress:
      if(ev.xbutton.subwindow != None)
      {
        XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
        start = ev.xbutton;
      }
      break;
    case MotionNotify:
      if(start.subwindow != None)
      {
        int xdiff = ev.xbutton.x_root - start.x_root;
        int ydiff = ev.xbutton.y_root - start.y_root;
        XMoveResizeWindow(dpy, start.subwindow,
          attr.x + (start.button==1 ? xdiff : 0),
          attr.y + (start.button==1 ? ydiff : 0),
          MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
          MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
      }
      break;
    case ButtonRelease:
      start.subwindow = None;
      break;
    }
  }
}

