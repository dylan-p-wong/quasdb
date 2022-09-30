# Architecture

## Lexer

The lexer is fairly simple. It will consume white space and then get the next string, It will try and then match the string to a keyword or other value or else it will be considered a identifer.

## Parser

The parser will parser the statements into an abstract syntax tree. It will create an expression tree in any places where expressions are used in the statement.

## Planner

The planner will take the abstract syntax tree and create plan nodes depending on the various statements and clauses. Currently indexes have not been added so every query will include sequential scans of the table. This will likely change in the future.

## Optimizer

There is currently no optimizer but there are plans for one in the future.

## Exectuor

The executor uses the materialization model for queries. Bascilly the tuple values are stored in memory and collected all at once at each node while being brought up the execution tree. This is bad for workloads with large queries so is a temporary solution for the time being.
