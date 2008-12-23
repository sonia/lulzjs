/* This is partly the Prototype JavaScript framework
 * (c) 2005-2008 Sam Stephenson
 *
 * Prototype is freely distributable under the terms of an MIT-style license.
 * For details, see the Prototype web site: http://www.prototypejs.org/
 *--------------------------------------------------------------------------*/


Object.extend(Number.prototype, {
  succ: function() {
    return this + 1;
  },
  
  times: function(iterator, context) {
    $R(0, this, true).each(iterator, context);
    return this;
  },
  
  toPaddedString: function(length, radix) {
    var string = this.toString(radix || 10);
    return '0'.times(length - string.length) + string;
  }
});

$w('abs round ceil floor').each(function(method){
  Number.prototype[method] = Math[method].methodize();
});
