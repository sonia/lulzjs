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

Program.Bytes = Class.create({
    initialize: function (base) {
        if (!Object.is(base, Array)) {
            throw "You have to pass an array.";
        }

        this.array = base;
    },

    get length() {
        return (this.array) ? this.array.length : 0;
    },

    byteAt: function (index) {
        if (index >= this.array.length) {
            throw "Index out of range.";
        }

        return this.array[index];
    },

    toArray: function () {
        return this.array;
    },

    toText: function () {
        var str = new String;

        for (var i = 0; i < this.array.length; i++) {
            str += String.fromCharCode(this.array[i]);
        }

        return str;
    },
    
    toString: function () {
        var str = new String;

        for (var i = 0; i < this.array.length; i++) {
            str += "\\x"+this.array[i].toPaddedString(2, 16);
        }

        return str;
    }
});

