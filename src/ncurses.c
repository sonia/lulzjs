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

#include "ncurses.h"

JSBool exec (JSContext* cx) { return ncurses_initialize(cx); }

JSBool
ncurses_initialize (JSContext* cx)
{
    JSObject* object = JS_DefineObject(
        cx, JS_GetGlobalObject(cx),
        ncurses_class.name, &ncurses_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );

    if (object) {
        JS_DefineFunctions(cx, object, ncurses_methods);

        jsval property;

        JSObject* Buffering   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsBuffering = OBJECT_TO_JSVAL(Buffering);
        JS_SetProperty(cx, object, "Buffering", &jsBuffering);
            property = INT_TO_JSVAL(Normal);
            JS_SetProperty(cx, Buffering, "Normal", &property);
            property = INT_TO_JSVAL(Raw);
            JS_SetProperty(cx, Buffering, "Raw", &property);
            property = INT_TO_JSVAL(CBreak);
            JS_SetProperty(cx, Buffering, "CBreak", &property);

        JSObject* Colors   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsColors = OBJECT_TO_JSVAL(Colors);
        JS_SetProperty(cx, object, "Colors", &jsColors);
            property = INT_TO_JSVAL(-1);
            JS_SetProperty(cx, Colors, "Normal", &property);
            property = INT_TO_JSVAL(COLOR_BLACK);
            JS_SetProperty(cx, Colors, "Black", &property);
            property = INT_TO_JSVAL(COLOR_RED);
            JS_SetProperty(cx, Colors, "Red", &property);
            property = INT_TO_JSVAL(COLOR_GREEN);
            JS_SetProperty(cx, Colors, "Green", &property);
            property = INT_TO_JSVAL(COLOR_YELLOW);
            JS_SetProperty(cx, Colors, "Yellow", &property);
            property = INT_TO_JSVAL(COLOR_BLUE);
            JS_SetProperty(cx, Colors, "Blue", &property);
            property = INT_TO_JSVAL(COLOR_MAGENTA);
            JS_SetProperty(cx, Colors, "Magenta", &property);
            property = INT_TO_JSVAL(COLOR_CYAN);
            JS_SetProperty(cx, Colors, "Cyan", &property);
            property = INT_TO_JSVAL(COLOR_WHITE);
            JS_SetProperty(cx, Colors, "White", &property);

        JSObject* Attributes   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsAttributes = OBJECT_TO_JSVAL(Attributes);
        JS_SetProperty(cx, object, "Attributes", &jsAttributes);
            property = INT_TO_JSVAL(A_NORMAL); // Normal display (no highlight)
            JS_SetProperty(cx, Attributes, "Normal", &property);
            property = INT_TO_JSVAL(A_BLINK); // Blinking
            JS_SetProperty(cx, Attributes, "Blink", &property);
            property = INT_TO_JSVAL(A_BOLD); // Extra bright or bold
            JS_SetProperty(cx, Attributes, "Bold", &property);
            property = INT_TO_JSVAL(A_DIM); // Half bright
            JS_SetProperty(cx, Attributes, "Dim", &property);
            property = INT_TO_JSVAL(A_REVERSE); // Reverse video
            JS_SetProperty(cx, Attributes, "Reverse", &property);
            property = INT_TO_JSVAL(A_STANDOUT); // Best highlighting mode of the terminal.
            JS_SetProperty(cx, Attributes, "Standout", &property);
            property = INT_TO_JSVAL(A_UNDERLINE); // Underlining
            JS_SetProperty(cx, Attributes, "Underline", &property);
            property = INT_TO_JSVAL(A_PROTECT); // Protected mode
            JS_SetProperty(cx, Attributes, "Protect", &property);
            property = INT_TO_JSVAL(A_INVIS);
            JS_SetProperty(cx, Attributes, "Invisible", &property);

        JSObject* Cursor   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsCursor = OBJECT_TO_JSVAL(Cursor);
        JS_SetProperty(cx, object, "Cursor", &jsCursor);
            property = INT_TO_JSVAL(0);
            JS_SetProperty(cx, Cursor, "Invisible", &property);
            property = INT_TO_JSVAL(1);
            JS_SetProperty(cx, Cursor, "Normal", &property);
            property = INT_TO_JSVAL(2);
            JS_SetProperty(cx, Cursor, "Visible", &property);

        JSObject* Keys   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsKeys = OBJECT_TO_JSVAL(Keys);
        JS_SetProperty(cx, object, "Keys", &jsKeys);
            property = INT_TO_JSVAL(KEY_BREAK);
            JS_SetProperty(cx, Keys, "Break", &property);
            property = INT_TO_JSVAL(KEY_DOWN);
            JS_SetProperty(cx, Keys, "Down", &property);
            property = INT_TO_JSVAL(KEY_UP);
            JS_SetProperty(cx, Keys, "Up", &property);
            property = INT_TO_JSVAL(KEY_LEFT);
            JS_SetProperty(cx, Keys, "Left", &property);
            property = INT_TO_JSVAL(KEY_RIGHT);
            JS_SetProperty(cx, Keys, "Right", &property);
            property = INT_TO_JSVAL(KEY_HOME);
            JS_SetProperty(cx, Keys, "Home", &property);
            property = INT_TO_JSVAL(KEY_BACKSPACE);
            JS_SetProperty(cx, Keys, "Backspace", &property);
            property = OBJECT_TO_JSVAL(JS_GetFunctionObject(
                JS_NewFunction(cx, ncurses_KEY_F, 1, 0, NULL, "KEY_F")
            ));
            JS_SetProperty(cx, Keys, "F", &property);
            property = INT_TO_JSVAL(KEY_DL);
            JS_SetProperty(cx, Keys, "DeleteLine", &property);
            property = INT_TO_JSVAL(KEY_IL);
            JS_SetProperty(cx, Keys, "InsertLine", &property);
            property = INT_TO_JSVAL(KEY_DC);
            JS_SetProperty(cx, Keys, "DeleteChar", &property);
            property = INT_TO_JSVAL(KEY_IC);
            JS_SetProperty(cx, Keys, "EnterInsertMode", &property);
            property = INT_TO_JSVAL(KEY_EIC);
            JS_SetProperty(cx, Keys, "EndInsertMode", &property);
            property = INT_TO_JSVAL(KEY_CLEAR);
            JS_SetProperty(cx, Keys, "Clear", &property);
            property = INT_TO_JSVAL(KEY_EOS);
            JS_SetProperty(cx, Keys, "EndOfScreen", &property);
            property = INT_TO_JSVAL(KEY_EOL);
            JS_SetProperty(cx, Keys, "EndOfLine", &property);
            property = INT_TO_JSVAL(KEY_SF);
            JS_SetProperty(cx, Keys, "ScrollForward", &property);
            property = INT_TO_JSVAL(KEY_SR);
            JS_SetProperty(cx, Keys, "ScrollBackward", &property);
            property = INT_TO_JSVAL(KEY_NPAGE);
            JS_SetProperty(cx, Keys, "NextPage", &property);
            property = INT_TO_JSVAL(KEY_PPAGE);
            JS_SetProperty(cx, Keys, "PreviousPage", &property);
            property = INT_TO_JSVAL(KEY_STAB);
            JS_SetProperty(cx, Keys, "SetTab", &property);
            property = INT_TO_JSVAL(KEY_CTAB);
            JS_SetProperty(cx, Keys, "ClearTab", &property);
            property = INT_TO_JSVAL(KEY_CATAB);
            JS_SetProperty(cx, Keys, "ClearAllTabs", &property);
            property = INT_TO_JSVAL(KEY_ENTER);
            JS_SetProperty(cx, Keys, "Enter", &property);
            property = INT_TO_JSVAL(KEY_SRESET);
            JS_SetProperty(cx, Keys, "SoftReset", &property);
            property = INT_TO_JSVAL(KEY_RESET);
            JS_SetProperty(cx, Keys, "Reset", &property);
            property = INT_TO_JSVAL(KEY_PRINT);
            JS_SetProperty(cx, Keys, "Print", &property);
            property = INT_TO_JSVAL(KEY_LL);
            JS_SetProperty(cx, Keys, "Bottom", &property);
            property = INT_TO_JSVAL(KEY_A1);
            JS_SetProperty(cx, Keys, "UpperLeft", &property);
            property = INT_TO_JSVAL(KEY_A3);
            JS_SetProperty(cx, Keys, "UpperRight", &property);
            property = INT_TO_JSVAL(KEY_B2);
            JS_SetProperty(cx, Keys, "Center", &property);
            property = INT_TO_JSVAL(KEY_C1);
            JS_SetProperty(cx, Keys, "LowerLeft", &property);
            property = INT_TO_JSVAL(KEY_C3);
            JS_SetProperty(cx, Keys, "LowerRight", &property);
            property = INT_TO_JSVAL(KEY_BTAB);
            JS_SetProperty(cx, Keys, "BackTab", &property);
            property = INT_TO_JSVAL(KEY_BEG);
            JS_SetProperty(cx, Keys, "Beginning", &property);
            property = INT_TO_JSVAL(KEY_CANCEL);
            JS_SetProperty(cx, Keys, "Cancel", &property);
            property = INT_TO_JSVAL(KEY_CLOSE);
            JS_SetProperty(cx, Keys, "Close", &property);
            property = INT_TO_JSVAL(KEY_COMMAND);
            JS_SetProperty(cx, Keys, "Command", &property);
            property = INT_TO_JSVAL(KEY_COPY);
            JS_SetProperty(cx, Keys, "Copy", &property);
            property = INT_TO_JSVAL(KEY_CREATE);
            JS_SetProperty(cx, Keys, "Create", &property);
            property = INT_TO_JSVAL(KEY_END);
            JS_SetProperty(cx, Keys, "End", &property);
            property = INT_TO_JSVAL(KEY_EXIT);
            JS_SetProperty(cx, Keys, "Exit", &property);
            property = INT_TO_JSVAL(KEY_FIND);
            JS_SetProperty(cx, Keys, "Find", &property);
            property = INT_TO_JSVAL(KEY_HELP);
            JS_SetProperty(cx, Keys, "Help", &property);
            property = INT_TO_JSVAL(KEY_MARK);
            JS_SetProperty(cx, Keys, "Mark", &property);
            property = INT_TO_JSVAL(KEY_MESSAGE);
            JS_SetProperty(cx, Keys, "Message", &property);
            property = INT_TO_JSVAL(KEY_MOUSE);
            JS_SetProperty(cx, Keys, "Mouse", &property);
            property = INT_TO_JSVAL(KEY_MOVE);
            JS_SetProperty(cx, Keys, "Move", &property);
            property = INT_TO_JSVAL(KEY_NEXT);
            JS_SetProperty(cx, Keys, "Next", &property);
            property = INT_TO_JSVAL(KEY_OPEN);
            JS_SetProperty(cx, Keys, "Open", &property);
            property = INT_TO_JSVAL(KEY_OPTIONS);
            JS_SetProperty(cx, Keys, "Options", &property);
            property = INT_TO_JSVAL(KEY_PREVIOUS);
            JS_SetProperty(cx, Keys, "Previous", &property);
            property = INT_TO_JSVAL(KEY_REDO);
            JS_SetProperty(cx, Keys, "Redo", &property);
            property = INT_TO_JSVAL(KEY_REFERENCE);
            JS_SetProperty(cx, Keys, "Reference", &property);
            property = INT_TO_JSVAL(KEY_REFRESH);
            JS_SetProperty(cx, Keys, "Refresh", &property);
            property = INT_TO_JSVAL(KEY_REPLACE);
            JS_SetProperty(cx, Keys, "Replace", &property);
            property = INT_TO_JSVAL(KEY_RESIZE);
            JS_SetProperty(cx, Keys, "Resize", &property);
            property = INT_TO_JSVAL(KEY_RESTART);
            JS_SetProperty(cx, Keys, "Restart", &property);
            property = INT_TO_JSVAL(KEY_RESUME);
            JS_SetProperty(cx, Keys, "Resume", &property);
            property = INT_TO_JSVAL(KEY_SAVE);
            JS_SetProperty(cx, Keys, "Save", &property);
            property = INT_TO_JSVAL(KEY_SELECT);
            JS_SetProperty(cx, Keys, "Select", &property);
            property = INT_TO_JSVAL(KEY_SUSPEND);
            JS_SetProperty(cx, Keys, "Suspend", &property);
            property = INT_TO_JSVAL(KEY_UNDO);
            JS_SetProperty(cx, Keys, "Undo", &property);
            JSObject* Shifted   = JS_NewObject(cx, NULL, NULL, NULL);
            jsval     jsShifted = OBJECT_TO_JSVAL(Shifted);
            JS_SetProperty(cx, Keys, "Shifted", &jsShifted);
                property = INT_TO_JSVAL(KEY_SBEG);
                JS_SetProperty(cx, Shifted, "Beginning", &property);
                property = INT_TO_JSVAL(KEY_SCANCEL);
                JS_SetProperty(cx, Shifted, "Cancel", &property);
                property = INT_TO_JSVAL(KEY_SCOMMAND);
                JS_SetProperty(cx, Shifted, "Command", &property);
                property = INT_TO_JSVAL(KEY_SCOPY);
                JS_SetProperty(cx, Shifted, "Copy", &property);
                property = INT_TO_JSVAL(KEY_SCREATE);
                JS_SetProperty(cx, Shifted, "Create", &property);
                property = INT_TO_JSVAL(KEY_SDC);
                JS_SetProperty(cx, Shifted, "DeleteChar", &property);
                property = INT_TO_JSVAL(KEY_SDL);
                JS_SetProperty(cx, Shifted, "DeleteLine", &property);
                property = INT_TO_JSVAL(KEY_SEND);
                JS_SetProperty(cx, Shifted, "End", &property);
                property = INT_TO_JSVAL(KEY_SEOL);
                JS_SetProperty(cx, Shifted, "EndOfLine", &property);
                property = INT_TO_JSVAL(KEY_SEXIT);
                JS_SetProperty(cx, Shifted, "Exit", &property);
                property = INT_TO_JSVAL(KEY_SFIND);
                JS_SetProperty(cx, Shifted, "Find", &property);
                property = INT_TO_JSVAL(KEY_SHELP);
                JS_SetProperty(cx, Shifted, "Help", &property);
                property = INT_TO_JSVAL(KEY_SHOME);
                JS_SetProperty(cx, Shifted, "Home", &property);
                property = INT_TO_JSVAL(KEY_SIC);
                JS_SetProperty(cx, Shifted, "EnterInsertMode", &property);
                property = INT_TO_JSVAL(KEY_SLEFT);
                JS_SetProperty(cx, Shifted, "Left", &property);
                property = INT_TO_JSVAL(KEY_SMESSAGE);
                JS_SetProperty(cx, Shifted, "Message", &property);
                property = INT_TO_JSVAL(KEY_SMOVE);
                JS_SetProperty(cx, Shifted, "Move", &property);
                property = INT_TO_JSVAL(KEY_SNEXT);
                JS_SetProperty(cx, Shifted, "Next", &property);
                property = INT_TO_JSVAL(KEY_SOPTIONS);
                JS_SetProperty(cx, Shifted, "Options", &property);
                property = INT_TO_JSVAL(KEY_SPREVIOUS);
                JS_SetProperty(cx, Shifted, "Previous", &property);
                property = INT_TO_JSVAL(KEY_SPRINT);
                JS_SetProperty(cx, Shifted, "Print", &property);
                property = INT_TO_JSVAL(KEY_SREDO);
                JS_SetProperty(cx, Shifted, "Redo", &property);
                property = INT_TO_JSVAL(KEY_SREPLACE);
                JS_SetProperty(cx, Shifted, "Replace", &property);
                property = INT_TO_JSVAL(KEY_SRIGHT);
                JS_SetProperty(cx, Shifted, "Right", &property);
                property = INT_TO_JSVAL(KEY_SRSUME);
                JS_SetProperty(cx, Shifted, "Resume", &property);
                property = INT_TO_JSVAL(KEY_SSAVE);
                JS_SetProperty(cx, Shifted, "Save", &property);
                property = INT_TO_JSVAL(KEY_SSUSPEND);
                JS_SetProperty(cx, Shifted, "Suspend", &property);
                property = INT_TO_JSVAL(KEY_SUNDO);
                JS_SetProperty(cx, Shifted, "Undo", &property);

        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
ncurses_KEY_F (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    jsint n; JS_ValueToInt32(cx, argv[0], &n);

    if (n < 0 || n > 63) {
        JS_ReportError(cx, "Out of range.");
        return JS_FALSE;
    }

    *rval = INT_TO_JSVAL(KEY_F(n));

    return JS_TRUE;
}

