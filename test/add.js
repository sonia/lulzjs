#! /usr/bin/ljs
require("System");

System.Console.write("Insert a number: ");
var a = parseInt(System.Console.readLine());
System.Console.write("Insert another number: ");
var b = parseInt(System.Console.readLine());
System.Console.writeLine("\nThe sum is: "+(a+b));
