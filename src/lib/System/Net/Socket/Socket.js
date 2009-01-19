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

Object.extend(System.Net.Socket.prototype, {
    sendLine: function (str, options) {
        options = options || {};
        var flags     = options.flags || 0;
        var separator = options.separator || "\r\n";

        if (!Object.is(str, Array)) {
            str = new Array(str);
        }

        for (var i = 0; i < str.length; i++) {
            this.send(str[i]+separator, flags)
        }
    },

    receiveLine: function (options) {
        options = options || {};

        var times     = options.times     || 1;
        var separator = options.separator || "\r\n";
        var flags     = options.flags     || 0;

        var str = "";
        for (var i = 0; i < times; i++) {
            var position = 0;
            var ch;
            while (ch = this.receive(1, flags)) {
                if (ch == separator[position]) {
                    position++;

                    if (position == separator.length)
                        break;

                    continue;
                }
                else if (position > 0) {
                    str += separator.substr(0, position+1);
                    position = 0;
                }

                str += ch;
            }
        }

        return str;
    }
});

