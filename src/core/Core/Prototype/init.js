/*  Prototype JavaScript framework, version 1.6.0.3
 *  (c) 2005-2008 Sam Stephenson
 *
 *  Prototype is freely distributable under the terms of an MIT-style license.
 *  For details, see the Prototype web site: http://www.prototypejs.org/
 *
 *--------------------------------------------------------------------------*/

var Prototype = {
  Version: '1.6.0.3',
  
  ScriptFragment: '<script[^>]*>([\\S\\s]*?)<\/script>',
  JSONFilter: /^\/\*-secure-([\s\S]*)\*\/\s*$/,  
  
  emptyFunction: function() { },
  K: function(x) { return x }
};

var Abstract = { };

var Try = {
  these: function() {
    var returnValue;
    
    for (var i = 0, length = arguments.length; i < length; i++) {
      var lambda = arguments[i];
      try {
        returnValue = lambda();
        break;
      } catch (e) { }
    }
    
    return returnValue;
  }
};

require(['lang/class.js', 'lang/object.js', 'lang/function.js']);
require(['lang/date.js', 'lang/regexp.js', 'lang/periodical_executer.js']);
require(['lang/string.js', 'lang/template.js']);
require(['lang/enumerable.js', 'lang/array.js', 'lang/hash.js']);
require(['lang/number.js', 'lang/range.js']);

