/*
Copyright 2020 Stephen Lane-Walsh

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation 
files (the "Software"), to deal in the Software without 
restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DUI_H
#define DUI_H

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int CharWidth;
    int CharHeight;

    int LinePadding;            // = 4

    int PanelPadding;           // = 8

    int ButtonPadding;          // = 4
    int ButtonMargin;           // = 8

    int TabPadding;             // = 8
    int TabMargin;              // = 8

    uint8_t ColorBackground[4];
    uint8_t ColorBorder[4];
    uint8_t ColorHover[4];
    uint8_t ColorDefault[4];

} DUI_Style;

/* Initialize the Debug UI.
 *
 * @param window: The SDL window to draw to. This will be used
 *   to get the SDL_Renderer for drawing, and the WindowID for
 *   determining which events are relevant. 
 */
void DUI_Init(SDL_Window * window);

/* Terminate the Debug UI.
 */
void DUI_Term();

/* Update DUI state, call once per frame.
 */
void DUI_Update();

/* Render DUI Foreground and Overlay, call at the end of every frame.
 */
void DUI_Render();

/* Set the style.
 *
 * @param style: The DUI_Style to use
 */
void DUI_SetStyle(DUI_Style style);

/* Get a mutable reference to the current style.
 *
 * @return: A mutable reference to the current style.
 */
DUI_Style * DUI_GetStyle();

/* Set the Render Draw Color of the SDL Renderer to the
 *  color specified in Style.ColorBackground.
 */
void DUI_SetColorBackground();

/* Set the Render Draw Color of the SDL Renderer to the
 *  color specified in Style.ColorBorder.
 */
void DUI_SetColorBorder();

/* Set the Render Draw Color of the SDL Renderer to the
 *  color specified in Style.ColorHover.
 */
void DUI_SetColorHover();

/* Set the Render Draw Color of the SDL Renderer to the
 *  color specified in Style.ColorDefault.
 */
void DUI_SetColorDefault();

/* Move the DUI cursor.
 *
 * @param x: The new x coordinate. This will be used as the start
 *   of future lines when DUI_Newline is called.
 *
 * @param y: The new y coordinate.
 */
void DUI_MoveCursor(int x, int y);

/* Move the DUI cursor relative to its current position.
 *
 * @param dx: The value to add to the x coordinate. The resulting
 *   value will be used as the start of future lines when
 *   DUI_Newline is called.
 *
 * @param dy: The value to add to the y coordinate.
 */
void DUI_MoveCursorRelative(int dx, int dy);

/* Get the current cursor position.
 *
 * @param x: A pointer to the int to put the x coordinate in.
 * 
 * @param y: A pointer to the int to put the y coordinate in.
 */
void DUI_GetCursor(int * x, int * y);

/* Move the cursor to a new line.
 *
 * Reset the x coordinate to the beginning of the line, and
 *   increment the y by the CharHeight and LinePadding, set in the style.
 */
void DUI_Newline();

/* Print formatted text at the current cursor
 * The cursor will be moved to the end of the printed text.
 * Newlines ('\n') will call DUI_Newline.
 *
 * @param format: The format string to pass to vsnprintf.
 */
void DUI_Print(const char * format, ...);

/* Shortcut for calling both DUI_Print and DUI_Newline
 */
#define DUI_Println(format, ...)            \
    do {                                    \
        DUI_Print(format, ##__VA_ARGS__);   \
        DUI_Newline();                      \
    } while (0)

/* Print formatted text at the given location.
 *
 * The cursor will not be updated.
 *
 * @param x: The x coordinate.
 *
 * @param y: The y coordinate.
 *
 * @param format: The format string to pass to vsnprintf.
 */
#define DUI_PrintAt(x, y, format, ...)      \
    do {                                    \
        int tmpX, tmpY;                     \
        DUI_GetCursor(&tmpX, &tmpY);        \
        DUI_MoveCursor(x, y);               \
        DUI_Print(format, ##__VA_ARGS__);   \
        DUI_MoveCursor(tmpX, tmpY);         \
    } while (0)

/* Store the current panel title, and minimum size
 *
 * Always call DUI_PanelEnd() after calling this.
 * 
 * @param title: Optional title to display in the panel.
 *
 * @param width: The minimum width of the panel to draw.
 *
 * @param height: The minimum height of the panel to draw.
 * 
 * @param fixed: True if the panel should not grow to fit its contents.
 */
void DUI_PanelStart(const char * title, int width, int height, bool fixed);

/* Draw the current panel outline and title, then discard the
 *   current panel information.
 * 
 * Call this only after DUI_PanelStart().
 *
 * The border will be drawn with ColorBorder.
 * The background will be filled with ColorBackground.
 * The PanelPadding value will be added to the cursor's
 *   x and y coordinates.
 */
void DUI_PanelEnd();

/* Draw a button with the specified text.
 *
 * The border will be drawn with ColorBorder.
 * The background will be filled with ColorHover if the
 *   mouse is over the button, or ColorDefault otherwise.
 * The ButtonPadding value will be added to the cursor's
 *   x and y coordinates before drawing text.
 * The ButtonMargin value will be added to the cursor's
 *   x coordinate after the button is drawn.
 *
 * @param text: The text to draw in the button.
 *
 * @return: True if the button is being clicked.
 */
bool DUI_Button(const char * text);

/* Draw a button at a specified location, with the given text.
 *
 * The cursor will not be updated.
 * 
 * @param x: The x coordinate.
 *
 * @param y: The y coordinate.
 *
 * @param text: The text to draw in the button.
 *
 * @return: True if the button is being clicked.
 */
bool DUI_ButtonAt(int x, int y, const char * text);

/* Draw a checkbox with the specified text.
 *
 * The border will be drawn with ColorBorder.
 * The background will be filled with ColorHover if the
 *   mouse is over the checkbox, or ColorDefault otherwise.
 * The ButtonPadding value will be added to the cursor's
 *   x and y coordinates before drawing text.
 * The ButtonMargin value will be added to the cursor's
 *   x coordinate after the checkbox is drawn.
 *
 * @param text: The text to draw in the checkbox.
 * 
 * @param checked: A pointer to a boolean to store the 
 *   state of the checkbox.
 *
 * @return: True if the checkbox is being clicked.
 */
bool DUI_Checkbox(const char * text, bool * checked);

/* Draw a checkbox at a specified location, with the given text.
 *
 * The cursor will not be updated.
 * 
 * @param x: The x coordinate.
 *
 * @param y: The y coordinate.
 *
 * @param text: The text to draw in the checkbox.
 * 
 * @param checked: A pointer to a boolean to store the 
 *   state of the checkbox.
 *
 * @return: True if the checkbox is checked.
 */
bool DUI_CheckboxAt(int x, int y, const char * text, bool * checked);

/* Draw a radio button with the specified text.
 *
 * The border will be drawn with ColorBorder.
 * The background will be filled with ColorHover if the
 *   mouse is over the radio button, or ColorDefault otherwise.
 * The ButtonPadding value will be added to the cursor's
 *   x and y coordinates before drawing text.
 * The ButtonMargin value will be added to the cursor's
 *   x coordinate after the radio button is drawn.
 *
 * @param text: The text to draw in the radio button.
 * 
 * @param index: The index of this radio button.
 * 
 * @param currentIndex: A pointer to the current index of this
 *   radio button group. If this radio button is clicked, the 
 *   currentIndex will be set to this radio button's index.
 *
 * @return: True if the radio button is selected.
 */
bool DUI_Radio(const char * text, int index, int * currentIndex);

/* Draw a radio button with the specified text.
 *
 * The cursor will not be updated.
 * 
 * @param x: The x coordinate.
 *
 * @param y: The y coordinate.
 *
 * @param text: The text to draw in the checkbox.
 * 
 * @param index: The index of this radio button.
 * 
 * @param currentIndex: A pointer to the current index of this
 *   radio button group. If this radio button is clicked, the 
 *   currentIndex will be set to this radio button's index.
 *
 * @return: True if the radio button is selected.
 */
bool DUI_RadioAt(int x, int y, const char * text, int index, int * currentIndex);

/* Prepares for tabs to be rendered.
 *
 * Saves the position of the next tab to be rendered, so that 
 *   render calls trigged by the tab being selected don't break it.
 */
void DUI_BeginTabBar();

/* Draw a tab with the specified text.
 *
 * The border will be drawn with ColorBorder.
 * The background will be filled with ColorHover if the
 *   mouse is over the tab, or ColorDefault otherwise.
 * The TabPadding value will be added to the cursor's
 *   x and y coordinates before drawing text.
 * The TabMargin value will be added to the cursor's
 *   x coordinate after the tab is drawn.
 *
 * @param text: The text to draw in the tab.
 * 
 * @param index: The index of this tab.
 * 
 * @param currentIndex: A pointer to the current index of this
 *   tab group. If this tab is clicked, the currentIndex will 
 *   be set to this tab's index.
 *
 * @return: True if the tab is selected.
 */
bool DUI_Tab(const char * text, int index, int * currentIndex);

#endif // DUI_H

#if defined(DUI_IMPLEMENTATION)

#if defined(DUI_FONT_ANONYMOUS)
#   include <DUI/DUI_FontAnonymous.h>
#else
#   include <DUI/DUI_FontGB.h>
#endif

SDL_Window *   _duiWindow      = NULL;
SDL_Renderer * _duiRenderer    = NULL;
SDL_Texture *  _duiFontTexture = NULL;

int _duiWindowID = 0;

typedef struct
{
    bool Fixed;
    SDL_Rect Bounds;
    const char * Title;
    SDL_Texture * Texture;

} DUI_PanelInfo;

#ifndef DUI_PANEL_STACK_DEPTH
#   define DUI_PANEL_STACK_DEPTH (10)
#endif // DUI_PANEL_STACK_DEPTH

DUI_PanelInfo _duiPanelStack[DUI_PANEL_STACK_DEPTH + 1];
int _duiPanelStackIndex = 0;

SDL_Texture * _duiOverlayTexture = NULL;

DUI_Style _duiStyle = {
    .CharWidth = DUI_FONT_CHAR_WIDTH,
    .CharHeight = DUI_FONT_CHAR_HEIGHT,

    .LinePadding = 4,

    .PanelPadding = 8,

    .ButtonPadding = 4,
    .ButtonMargin  = 8,

    .TabPadding = 8,
    .TabMargin  = 8,

    .ColorBackground = { 0xEE, 0xEE, 0xEE, 0xFF },
    .ColorBorder     = { 0x00, 0x00, 0x00, 0xFF },
    .ColorHover      = { 0xEE, 0xEE, 0xEE, 0xEE },
    .ColorDefault    = { 0xAA, 0xAA, 0xAA, 0xAA },
};

int _duiLineStart = 0;

SDL_Point _duiMouse     = { 0, 0 };
SDL_Point _duiCursor    = { 0, 0 };
SDL_Point _duiTabCursor = { 0, 0 };

int _duiWindowWidth;
int _duiWindowHeight;

bool _duiMouseDown = false;
bool _duiClicked = false;

DUI_PanelInfo * DUI_getCurrentPanel()
{
    return &_duiPanelStack[_duiPanelStackIndex];
}

void DUI_popPanel()
{
    --_duiPanelStackIndex;
}

DUI_PanelInfo * DUI_pushPanel()
{
    ++_duiPanelStackIndex;
    DUI_PanelInfo * panel = DUI_getCurrentPanel();
    panel->Bounds = (SDL_Rect){ 0, 0, 0, 0 };
    return panel;
}

void DUI_growPanel()
{
    DUI_PanelInfo * panel = DUI_getCurrentPanel();
    if (panel->Fixed) {
        return;
    }

    int x = (_duiCursor.x - panel->Bounds.x);
    int y = (_duiCursor.y - panel->Bounds.y);

    if (x > panel->Bounds.w) {
        panel->Bounds.w = x;
    }

    if (y > panel->Bounds.h) {
        panel->Bounds.h = y;
    }
}

void DUI_Init(SDL_Window * window)
{
    _duiWindow = window;
    _duiRenderer = SDL_GetRenderer(window);
    _duiWindowID = SDL_GetWindowID(window);

    SDL_SetRenderDrawBlendMode(_duiRenderer, SDL_BLENDMODE_BLEND);

    SDL_GetWindowSize(window, &_duiWindowWidth, &_duiWindowHeight);

    int pitch;
    void * pixels;

    // The Window's Rendering Target
    _duiPanelStack[0].Fixed = true;
    _duiPanelStack[0].Bounds = (SDL_Rect){ 0, 0, _duiWindowWidth, _duiWindowHeight };
    _duiPanelStack[0].Title = NULL;
    _duiPanelStack[0].Texture = NULL;

    for (int i = 1; i <= DUI_PANEL_STACK_DEPTH; ++i) {
        _duiPanelStack[i].Texture = SDL_CreateTexture(_duiRenderer, 
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_TARGET,
            _duiWindowWidth, _duiWindowHeight);

        SDL_SetTextureBlendMode(_duiPanelStack[i].Texture, SDL_BLENDMODE_BLEND);
    }

    _duiOverlayTexture = SDL_CreateTexture(_duiRenderer, 
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_TARGET,
            _duiWindowWidth, _duiWindowHeight);

    SDL_SetTextureBlendMode(_duiOverlayTexture, SDL_BLENDMODE_BLEND);

    SDL_RWops * fontMem = SDL_RWFromConstMem(DUI_FONT_BMP, sizeof(DUI_FONT_BMP));
    SDL_Surface * fontSurface = SDL_LoadBMP_RW(fontMem, 1);

    _duiFontTexture = SDL_CreateTextureFromSurface(_duiRenderer, fontSurface);
    SDL_FreeSurface(fontSurface);

    SDL_SetRenderTarget(_duiRenderer, _duiPanelStack[0].Texture);
}

void DUI_Term()
{
    SDL_DestroyTexture(_duiFontTexture);
        
    for (int i = 1; i < DUI_PANEL_STACK_DEPTH; ++i) {
        SDL_DestroyTexture(_duiPanelStack[i].Texture);
        _duiPanelStack[i].Texture = NULL;
    }
}

void DUI_Update()
{
    int state = SDL_GetMouseState(&_duiMouse.x, &_duiMouse.y);
    bool pressed = (state & SDL_BUTTON(SDL_BUTTON_LEFT));
    _duiClicked = (pressed && !_duiMouseDown);
    _duiMouseDown = pressed;
}

void DUI_Render()
{
}

void DUI_SetStyle(DUI_Style style)
{
    _duiStyle = style;
}

DUI_Style * DUI_GetStyle()
{
    return &_duiStyle;
}

void DUI_SetColorBackground()
{
    SDL_SetRenderDrawColor(_duiRenderer, 
        _duiStyle.ColorBackground[0],
        _duiStyle.ColorBackground[1],
        _duiStyle.ColorBackground[2],
        _duiStyle.ColorBackground[3]);
}

void DUI_SetColorBorder()
{
    SDL_SetRenderDrawColor(_duiRenderer, 
        _duiStyle.ColorBorder[0],
        _duiStyle.ColorBorder[1],
        _duiStyle.ColorBorder[2],
        _duiStyle.ColorBorder[3]);
}

void DUI_SetColorHover()
{
    SDL_SetRenderDrawColor(_duiRenderer, 
        _duiStyle.ColorHover[0],
        _duiStyle.ColorHover[1],
        _duiStyle.ColorHover[2],
        _duiStyle.ColorHover[3]);
}

void DUI_SetColorDefault()
{
    SDL_SetRenderDrawColor(_duiRenderer, 
        _duiStyle.ColorDefault[0],
        _duiStyle.ColorDefault[1],
        _duiStyle.ColorDefault[2],
        _duiStyle.ColorDefault[3]);
}

void DUI_MoveCursor(int x, int y)
{
    _duiCursor.x = x;
    _duiCursor.y = y;
    _duiLineStart = _duiCursor.x;
}

void DUI_MoveCursorRelative(int dx, int dy)
{
    _duiCursor.x += dx;
    _duiCursor.y += dy;
    _duiLineStart = _duiCursor.x;
}

void DUI_GetCursor(int * x, int * y)
{
    *x = _duiCursor.x;
    *y = _duiCursor.y;
}

void DUI_Newline()
{
    _duiCursor.y += _duiStyle.CharHeight + _duiStyle.LinePadding;
    _duiCursor.x = _duiLineStart;

    DUI_growPanel();
}

void DUI_Print(const char * format, ...)
{
    static char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    size_t length = strlen(buffer);

    SDL_Rect src = { 
        .x = 0,
        .y = 0,
        .w = DUI_FONT_CHAR_WIDTH,
        .h = DUI_FONT_CHAR_HEIGHT,
    };

    SDL_Rect dst = { 
        .x = _duiCursor.x,
        .y = _duiCursor.y,
        .w = _duiStyle.CharWidth,
        .h = _duiStyle.CharHeight,
    };

    int charPerLine = (DUI_FONT_MAP_WIDTH / DUI_FONT_CHAR_WIDTH);

    char * questionMark = strchr(DUI_FONT_MAP, '?');

    for (size_t i = 0; i < length; ++i) {
        if (buffer[i] == ' ') {
            dst.x += _duiStyle.CharWidth;
            continue;
        }

        if (buffer[i] == '\n') {
            DUI_Newline();
            dst.x = _duiCursor.x;
            dst.y = _duiCursor.y;
            continue;
        }

        char search = buffer[i];
        if (DUI_FONT_UPPERCASE) {
            search = toupper(buffer[i]);
        }

        char * index = strchr(DUI_FONT_MAP, search);

        if (index == NULL) {
            index = questionMark;
        }

        size_t offset = index - DUI_FONT_MAP;

        src.x = (offset % charPerLine) * DUI_FONT_CHAR_WIDTH;
        src.y = (offset / charPerLine) * DUI_FONT_CHAR_HEIGHT;
        SDL_RenderCopy(_duiRenderer, _duiFontTexture, &src, &dst);

        dst.x += _duiStyle.CharWidth;
    }

    _duiCursor.x = dst.x;
    _duiCursor.y = dst.y;

    DUI_growPanel();
}

void DUI_PanelStart(const char * title, int width, int height, bool fixed)
{
    DUI_PanelInfo * panel = DUI_pushPanel();
    panel->Fixed = fixed;
    panel->Title = title;
    panel->Bounds.x = _duiCursor.x;
    panel->Bounds.y = _duiCursor.y;
    panel->Bounds.w = width + _duiStyle.PanelPadding;
    panel->Bounds.h = height + _duiStyle.PanelPadding;

    if (panel->Title) {
        panel->Bounds.y += (_duiStyle.CharHeight / 2);
        DUI_MoveCursorRelative(0, _duiStyle.CharHeight);
    }

    DUI_MoveCursorRelative(_duiStyle.PanelPadding, _duiStyle.PanelPadding);

    SDL_SetRenderTarget(_duiRenderer, _duiPanelStack[_duiPanelStackIndex].Texture);
    SDL_SetRenderDrawColor(_duiRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_duiRenderer);
}

void DUI_PanelEnd()
{
    DUI_PanelInfo * panel = DUI_getCurrentPanel();

    panel->Bounds.w += _duiStyle.PanelPadding;
    panel->Bounds.h += _duiStyle.PanelPadding;

    SDL_Rect bounds = panel->Bounds;

    if (panel->Title) {
        SDL_Rect bounds = panel->Bounds;
        bounds.x += _duiStyle.CharWidth;
        bounds.y -= (_duiStyle.CharHeight / 2);
        bounds.w = (_duiStyle.CharWidth * (strlen(panel->Title) + 2));
        bounds.h = _duiStyle.CharHeight;
        
        DUI_SetColorBackground();
        SDL_RenderFillRect(_duiRenderer, &bounds);

        DUI_PrintAt(bounds.x + _duiStyle.CharWidth, bounds.y, panel->Title);
    }

    SDL_SetRenderTarget(_duiRenderer, _duiPanelStack[_duiPanelStackIndex - 1].Texture);

    DUI_SetColorBackground();
    SDL_RenderFillRect(_duiRenderer, &bounds);

    DUI_SetColorBorder();
    SDL_RenderDrawRect(_duiRenderer, &bounds);

    SDL_RenderCopy(_duiRenderer, _duiPanelStack[_duiPanelStackIndex].Texture, NULL, NULL);

    DUI_MoveCursor(panel->Bounds.x, 
        panel->Bounds.y + panel->Bounds.h + _duiStyle.LinePadding);

    DUI_popPanel();
}

bool DUI_Button(const char * text)
{
    int width = (strlen(text) * _duiStyle.CharWidth) 
        + (_duiStyle.ButtonPadding * 2);

    int height = _duiStyle.CharHeight
        + (_duiStyle.ButtonPadding * 2);

    SDL_Rect bounds = {
        .x = _duiCursor.x,
        .y = _duiCursor.y,
        .w = width,
        .h = height,
    };

    bool hover = SDL_PointInRect(&_duiMouse, &bounds);
    bool clicked = (hover && _duiClicked);

    if (hover) {
        DUI_SetColorHover();
    }
    else {
        DUI_SetColorDefault();
    }

    SDL_RenderFillRect(_duiRenderer, &bounds);

    DUI_SetColorBorder();
    SDL_RenderDrawRect(_duiRenderer, &bounds);

    _duiCursor.x += _duiStyle.ButtonPadding;
    _duiCursor.y += _duiStyle.ButtonPadding;

    DUI_Print("%s", text);

    _duiCursor.x = bounds.x + bounds.w + _duiStyle.ButtonMargin;
    _duiCursor.y = bounds.y;

    return clicked;
}

bool DUI_ButtonAt(int x, int y, const char * text)
{
    SDL_Point tmp = _duiCursor;
    _duiCursor.x = x;
    _duiCursor.y = y;

    bool clicked = DUI_Button(text);

    _duiCursor = tmp;

    return clicked;
}

bool DUI_Checkbox(const char * text, bool * checked)
{
    int width = (strlen(text) * _duiStyle.CharWidth)
        + (_duiStyle.CharWidth * 2)
        + (_duiStyle.ButtonPadding * 2);

    int height = _duiStyle.CharHeight
        + (_duiStyle.ButtonPadding * 2);

    SDL_Rect bounds = {
        .x = _duiCursor.x,
        .y = _duiCursor.y,
        .w = width,
        .h = height,
    };

    bool hover = SDL_PointInRect(&_duiMouse, &bounds);
    bool clicked = (hover && _duiClicked);

    if (hover) {
        DUI_SetColorHover();
    }
    else {
        DUI_SetColorDefault();
    }

    SDL_RenderFillRect(_duiRenderer, &bounds);

    DUI_SetColorBorder();
    SDL_RenderDrawRect(_duiRenderer, &bounds);

    SDL_Rect mark = { 
        .x = bounds.x + (_duiStyle.CharWidth / 2),
        .y = bounds.y + (_duiStyle.CharHeight / 2),
        .w = _duiStyle.CharWidth,
        .h = _duiStyle.CharWidth,
    };

    SDL_RenderDrawRect(_duiRenderer, &mark);

    if (clicked) {
        *checked ^= true;
    }

    if (*checked) {
        ++mark.x;
        ++mark.y;
        mark.w -= 2;
        mark.h -= 2;

        SDL_RenderFillRect(_duiRenderer, &mark);
    }

    _duiCursor.x += _duiStyle.ButtonPadding
        + _duiStyle.CharWidth
        + (_duiStyle.CharWidth / 2);
    _duiCursor.y += _duiStyle.ButtonPadding;

    DUI_Print("%s", text);

    _duiCursor.x = bounds.x + bounds.w + _duiStyle.ButtonMargin;
    _duiCursor.y = bounds.y;

    return *checked;
}

bool DUI_CheckboxAt(int x, int y, const char * text, bool * checked)
{
    SDL_Point tmp = _duiCursor;
    _duiCursor.x = x;
    _duiCursor.y = y;

    bool selected = DUI_Checkbox(text, checked);

    _duiCursor = tmp;

    return selected;
}

bool DUI_Radio(const char * text, int index, int * currentIndex)
{
    int width = (strlen(text) * _duiStyle.CharWidth) 
        + (_duiStyle.CharWidth * 2)
        + (_duiStyle.ButtonPadding * 2);

    int height = _duiStyle.CharHeight 
        + (_duiStyle.ButtonPadding * 2);

    SDL_Rect bounds = {
        .x = _duiCursor.x,
        .y = _duiCursor.y,
        .w = width,
        .h = height,
    };

    bool hover = SDL_PointInRect(&_duiMouse, &bounds);
    bool clicked = (hover && _duiClicked);

    if (clicked) {
        *currentIndex = index;
    }

    bool active = (*currentIndex == index);

    if (hover) {
        DUI_SetColorHover();
    }
    else {
        DUI_SetColorDefault();
    }

    SDL_RenderFillRect(_duiRenderer, &bounds);

    DUI_SetColorBorder();
    SDL_RenderDrawRect(_duiRenderer, &bounds);

    SDL_Rect mark = { 
        .x = bounds.x + (_duiStyle.CharWidth / 2),
        .y = bounds.y + (_duiStyle.CharHeight / 2),
        .w = _duiStyle.CharWidth, 
        .h = _duiStyle.CharWidth
    };

    SDL_RenderDrawRect(_duiRenderer, &mark);

    if (active) {
        ++mark.x;
        ++mark.y;
        mark.w -= 2;
        mark.h -= 2;

        SDL_RenderFillRect(_duiRenderer, &mark);
    }

    _duiCursor.x += _duiStyle.ButtonPadding
        + _duiStyle.CharWidth
        + (_duiStyle.CharWidth / 2);
    _duiCursor.y += _duiStyle.ButtonPadding;
    
    DUI_Print("%s", text);

    _duiCursor.x = bounds.x + bounds.w + _duiStyle.ButtonMargin;
    _duiCursor.y = bounds.y;

    return active;
}

bool DUI_RadioAt(int x, int y, const char * text, int index, int * currentIndex)
{
    SDL_Point tmp = _duiCursor;
    _duiCursor.x = x;
    _duiCursor.y = y;

    bool selected = DUI_Radio(text, index, currentIndex);

    _duiCursor = tmp;

    return selected;
}

void DUI_BeginTabBar()
{
    _duiTabCursor = _duiCursor;
}

bool DUI_Tab(const char * text, int index, int * currentIndex)
{
    _duiCursor = _duiTabCursor;

    int width = (strlen(text) * _duiStyle.CharWidth) 
        + (_duiStyle.TabPadding * 2);

    int height = _duiStyle.CharHeight
        + (_duiStyle.TabPadding * 2);

    SDL_Rect bounds = {
        .x = _duiCursor.x,
        .y = _duiCursor.y,
        .w = width,
        .h = height,
    };

    bool hover = SDL_PointInRect(&_duiMouse, &bounds);
    bool clicked = (hover && _duiClicked);

    if (clicked) {
        *currentIndex = index;
    }

    bool active = (*currentIndex == index);

    if (hover || active) {
        DUI_SetColorHover();
    }
    else {
        DUI_SetColorDefault();
    }

    SDL_RenderFillRect(_duiRenderer, &bounds);

    DUI_SetColorBorder();
    SDL_RenderDrawRect(_duiRenderer, &bounds);

    _duiCursor.x += _duiStyle.TabPadding;
    _duiCursor.y += _duiStyle.TabPadding;
    
    DUI_Print("%s", text);

    _duiCursor.x = bounds.x + bounds.w + _duiStyle.TabMargin;
    _duiCursor.y = bounds.y;

    _duiTabCursor = _duiCursor;

    return active;
}

#endif