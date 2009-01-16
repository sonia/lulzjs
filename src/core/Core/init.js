/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh.                                                             *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be useful.                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

// Isolate the used variables.
{
    var PATH = ENV("JSPATH");

    if (PATH) {
        var re = /([^:])+/g;
    
        var path;
        while (path = re.exec(PATH)) {
            __PATH__.push(path[0]);
        }
    }
};

require("Thread");

require("Prototype");

require("String.js");

Program.GCExecution = new PeriodicalExecuter(function(){Program.GC()}, 60);
