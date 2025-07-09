#include "LINUX.hpp"

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <SFML/Window.hpp>

void makeAlwaysOnTop(sf::Window& window) 
{
    Display* display = XOpenDisplay(nullptr);
    if (!display) return;

    Window x11Window = window.getNativeHandle();

    Atom wmState = XInternAtom(display, "_NET_WM_STATE", False);
    Atom wmStateAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

    XEvent xev = {};
    xev.xclient.type = ClientMessage;
    xev.xclient.send_event = True;
    xev.xclient.window = x11Window;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1; // _NET_WM_STATE_ADD
    xev.xclient.data.l[1] = wmStateAbove;
    xev.xclient.data.l[2] = 0;

    XSendEvent(display, DefaultRootWindow(display), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xev);

    XFlush(display);
    XCloseDisplay(display);
}
#endif
