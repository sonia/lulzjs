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

require("Prototype");
require("Extension");
require("Base");

//Program.GCExecution = new PeriodicalExecuter(function(){Program.GC()}, 60);

// Isolate the used variables.
(function(){
    // Include the environment paths in the current script.
    var PATH = ENV("JSPATH");
    if (PATH) {
        let re = /([^:])+/g;
    
        let path;
        while (path = re.exec(PATH)) {
            __PATH__.push(path[0]);
        }
    }

    // Include the standards include following the environment variable.
    var INCLUDE = ENV("JSINCLUDE");
    if (INCLUDE) {
        let re = /([^:])+/g;
    
        let file;
        while (file = re.exec(INCLUDE)) {
            include(file[0]);
        }
    }
})();
