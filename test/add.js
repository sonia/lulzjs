#! /usr/bin/ljs
#include <System>

System.IO.Console.write("Insert a number: ");
var a = parseInt(System.IO.Console.readLine());
System.IO.Console.write("Insert another number: ");
var b = parseInt(System.IO.Console.readLine());
System.IO.Console.writeLine("\nThe sum is: "+(a+b));
