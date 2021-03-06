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

[System.IO.Stream, System.IO.STDOUT, System.IO.STDIN, System.IO.STDERR].forEach(function(obj){
    Object.extend(obj, {
        writeLine: function (str) {
            this.write(str+"\n");
        },
    
        readLine: function () {
            var str = "";
            var ch;

            while ((ch = this.read(1)) != '\n') {
                str += ch;
            }
    
            return str;
        }
    });
});
