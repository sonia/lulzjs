/****************************************************************************
* This file is part of lulzJS-ncurses                                       *
* Copyleft meh.                                                             *
*                                                                           *
* lulzJS-ncurses is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS-ncurses is distributed in the hope that it will be useful.         *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS-ncurses.  If not, see <http://www.gnu.org/licenses/>.   *
****************************************************************************/

#include "Panel.h"

extern JSObject** panels;
extern int        panelsSize;

JSBool exec (JSContext* cx) { return Panel_initialize(cx); }

JSBool
Panel_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "ncurses", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Panel_class,
        Panel_constructor, 1, NULL, Panel_methods, NULL, Panel_static_methods
    );

    if (object) {
        jsval property;

        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Panel_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    jsval property;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "ncurses", &property);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(property), "Window",&property);

    JSObject* class = JSVAL_TO_OBJECT(property);
    JS_GetProperty(cx, class, "prototype", &property);
    JSObject* proto = JSVAL_TO_OBJECT(property);
    
    JSObject* Window = JS_ConstructObject(cx, JS_GET_CLASS(cx, class), proto, NULL);
    JS_CallFunctionValue(cx, Window, OBJECT_TO_JSVAL(class), argc, argv, &property);
    
    property = OBJECT_TO_JSVAL(Window);
    JS_SetProperty(cx, object, "__window", &property);

    PANEL* panel = new_panel(((WindowInformation*)JS_GetPrivate(cx, Window))->win);
    JS_SetPrivate(cx, object, panel);
    
    update_panels();

    panels = realloc(panels, (++panelsSize)*sizeof(JSObject*));
    panels[panelsSize-1] = object;
    
    JS_DefineProperty(
        cx, object, "below", JSVAL_NULL,
        (JSPropertyOp) JS_GetFunctionObject(JS_NewFunction(cx, Panel_below, 0, 0, NULL, NULL)),
        NULL, JSPROP_GETTER|JSPROP_READONLY
    );
    JS_DefineProperty(
        cx, object, "above", JSVAL_NULL,
        (JSPropertyOp) JS_GetFunctionObject(JS_NewFunction(cx, Panel_above, 0, 0, NULL, NULL)),
        NULL, JSPROP_GETTER|JSPROP_READONLY
    );

    return JS_TRUE;
}

void
Panel_finalize (JSContext* cx, JSObject* object)
{
    PANEL* panel = JS_GetPrivate(cx, object);

    if (panel) {
        // Find the object being finalized and remove it from the list
        int i;
        for (i = 0; i < panelsSize; i++) {
            if (object == panels[i]) {
                break;
            }
        }
        for (i++; i < panelsSize; i++) {
            panels[i-1] = panels[i];
        }
        panels = realloc(panels, (--panelsSize)*sizeof(JSObject*));

        del_panel(panel);
    }
}

JSBool
Panel_hide (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    PANEL* panel = JS_GetPrivate(cx, object);
    hide_panel(panel);

    jsval val = JSVAL_TRUE;
    JS_SetProperty(cx, object, "hidden", &val);

    return JS_TRUE;
}

JSBool
Panel_show (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    PANEL* panel = JS_GetPrivate(cx, object);
    show_panel(panel);

    jsval val = JSVAL_FALSE;
    JS_SetProperty(cx, object, "hidden", &val);

    return JS_TRUE;
}

JSBool
Panel_toTop (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    PANEL* panel = JS_GetPrivate(cx, object);
    top_panel(panel);

    return JS_TRUE;
}

JSBool
Panel_toBottom (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    PANEL* panel = JS_GetPrivate(cx, object);
    bottom_panel(panel);

    return JS_TRUE;
}

JSBool
Panel_above (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    PANEL* panel = JS_GetPrivate(cx, object);
    PANEL* above = panel_above(panel);

    int i;
    for (i = 0; i < panelsSize; i++) {
        PANEL* check = JS_GetPrivate(cx, panels[i]);

        if (check == above) {
            *rval = OBJECT_TO_JSVAL(panels[i]);
            break;
        }
    }

    return JS_TRUE;
}

JSBool
Panel_below (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    PANEL* panel = JS_GetPrivate(cx, object);
    PANEL* below = panel_below(panel);

    int i;
    for (i = 0; i < panelsSize; i++) {
        PANEL* check = JS_GetPrivate(cx, panels[i]);

        if (check == below) {
            *rval = OBJECT_TO_JSVAL(panels[i]);
            break;
        }
    }

    return JS_TRUE;
}

JSBool
Panel_static_update (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    update_panels();
    return JS_TRUE;
}

