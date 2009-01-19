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

Object.extend(System.IO.File.prototype, {
    readLine: function () {
        if (this.isEnd())
            return null;

        var str = "";
        var ch;
        while ((ch = this.read(1)) != '\n' && !this.isEnd()) {
            str += ch;
        }

        return str;
    },

    readToEnd: function () {
        var str = "";

        var line;
        while ((line = this.readLine()) != null) {
            str += line+"\n";
        }
        str = str.substr(0, str.length-1);

        return str;

    },

    readAll: function () {
        return this.readToEnd().split('\n');
    }
});
