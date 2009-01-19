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

require("System/Console");

Object.is = function (obj, type) {
    var name;

    if (typeof(type) == "function") {
        var match = /^function (\w+)/.exec(type.toString());
        name = match[1];

        if (name == "klass") {
            name = type.prototype.__type__;
        }
    }
    else if (typeof(type) == "object") {
        var match = /^function (\w+)/.exec(type.constructor.toString());
        name = match[1];

        if (name == "klass") {
            name = type.prototype.__type__;
        }
    }
    else {
        name = type;
    }

    return (obj.constructor.toString().match(/function klass/)
        ? obj.__proto__.__type__
        : obj.constructor.toString()).match(new RegExp("^.*"+name))
            ? true
            : false;
}

