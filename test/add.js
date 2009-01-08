#! /usr/bin/ljs
require("System/Console");

Console.write("Insert a number: ");
var a = parseInt(Console.readLine());
Console.write("Insert another number: ");
var b = parseInt(Console.readLine());
Console.writeLine("\nThe sum is: "+(a+b));
