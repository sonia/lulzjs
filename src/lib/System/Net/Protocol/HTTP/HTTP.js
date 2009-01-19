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

Object.extend(System.Net.Protocol.HTTP, {
    parseResponse: function (text) {
        var matches = /^HTTP\/(.+) (\d+) (.*)$/.exec(text);

        return {
            version: matches[1],
            code   : matches[2],
            message: matches[3]
        };
    },

    parseHeaders: function (text) {
        var re      = /\s*([\w\-]+)\s*:\s*(.*)/g;
        var headers = new Object;

        var header;
        while (header = re.exec(text)) {
            headers[header[1].capitalizeAll()] = header[2];
        }

        return headers;
    },

    getTextParams: function (params) {
        var text = '';

        if (Object.is(params, Array)) {
            if (params.length % 2) {
                throw "The array has a odd length.";
            }

            var paramz = params;
            params     = new Object;
            for (var i = 0; i < paramz.length; i+=2) {
                params[paramz[i]] = paramz[i+1];
            }
        }

        for (var param in params) {
            text += "&{name}={value}".format({
                name: param,
                value: params[param]
            });
        }
        return text.substr(1);
    }
});
