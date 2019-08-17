# NaiveParser

Simple recursive parser.

This example demonstrates 3 parsers to parse the following hypothetical
formats (just a demo):

* .ssproj file (a root solution-describing file. Think of .sln file)
* .ssmeta file (1st subproject data format. Think of .vcproj file)
* .ssdata file (2nd subproject data format. Think of .vcproj file, but different grammar)

Usage:

How to implement a parser for, say, .ssproj:
* Define your data structs (ASTs) which define an output of a parser (see project/project.h)
* Inherit your parser class from base::Parser (see project/project_parser.h)
* Implement your parser, using base::Parser helper methods.

Same for the other two parsers: See
* project/project_metadata_parser.h
* project/project_data_parser.h

example/example.cpp demonstrates, how two use all three parsers together.

Happy parsing!
