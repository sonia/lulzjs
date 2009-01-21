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

Object.extend = function (to, from) {
    for (var name in from) {
        to[name] = from[name];
    }

    return to;
};

Object.extend(Object, {
    is: function (obj, type) {
        if (typeof type == 'string') {
            type = eval(type);;
        }

        return obj instanceof type;
    },

    inspect: function (obj) {
        try {
            if (typeof obj == 'undefined') {
                return 'undefined'
            }
            else if (obj === null) {
                return 'null';
            }
        
            return (obj.inspect) ? obj.inspect() : obj.toString();
        }
        catch (e) {
            return e;
        }
    },

    toArray: function (obj) {
        if (!obj) {
            return new Array;
        }

        if (obj.toArray) {
            return obj.toArray();
        }

        var result = new Array;
        for (let i = 0; i < obj.length; i++) {
            result[i] = obj[i];
        }

        return result;
    }
});

