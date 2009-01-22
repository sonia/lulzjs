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

Object.extend(ncurses.Screen.prototype, {
    printLine: function () {
        switch (arguments.length) {
            case 1: this.print(arguments[0]+"\n"); break;
            case 2: this.print(arguments[0]+"\n", arguments[1]); break;
            case 3: this.print(arguments[0]+"\n", arguments[1], arguments[2]); break;
            case 4: this.print(arguments[0]+"\n", arguments[1], arguments[2], arguments[3]); break;
        }
    }
});
