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

var Class = {
    create: function () {
        var parent = null;
        arguments  = Object.toArray(arguments);

        if (Object.is(arguments[0], Function)) {
            parent = arguments.shift();
        }

        var init = function () {
            this.initialize.apply(this, arguments);
        }

        init.superclass = parent;
        init.subclasses = new Array;

        if (parent) {
            subclass           = Function.empty;
            subclass.prototype = parent.prototype;
            init.prototype     = new subclass;
            parent.subclasses.push(init);
        }

        for (let [name, value] in arguments[0]) {
            init.prototype[name] = value;
        }

        if (!init.prototype.initialize) {
            init.prototype.initialize = Function.empty;
        }

        init.prototype.constructor = init;
        return init;
    }
};

