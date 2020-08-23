# Boocher

Boocher is a tiny library system with a Memgraph database backend.

Boocher is just a toy for me to play with the memgraph client.
It doesnt really do much at the moment, and its lacking tests.

## Building

### Requirements: 
    * [mgclient](https://github.com/memgraph/mgclient) and it's deps
    * A C++ compiler

1: Run `get_mg.sh` to get the mgclient library
2: Run `make`

## Usage
```
"boocher init [hostname] [port]" - Initialise the database
"boocher add [hostname] [port] [title] [author]" - Add a book
"boocher set-status [hostname] [port] [title] [status]" - Sets a book's status ( e.g 'loaned', 'reading', 'finished' ). Books can have many statuses.
"boocher remove-status[hostname] [port] [title] [status]" - Removes a status from a book
"boocher get [hostname] [port] [title]" - Gets a book and it's assotiated statuses
"boocher get-all [hostname] [port] [status]" - Gets all books connected to given status.
```

