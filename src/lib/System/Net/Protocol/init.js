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

require("System/System.so");

require("System/Net/Net.so");

require(["System/Net/Socket/Socket.so", "System/Net/Socket/Socket.js"]);

require("Protocol.so");

require([
    "HTTP/HTTP.so", "HTTP/HTTP.js",
    "HTTP/Request.js", "HTTP/Response.js",
    "HTTP/Client.js"
]);

var Protocol = System.Net.Protocol;
