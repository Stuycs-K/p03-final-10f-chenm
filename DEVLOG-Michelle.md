# Dev Log:

This document must be updated daily every time you finish a work session.

## Michelle Chen

### 2025-01-06 - Uploaded Proposal, updated README.md, set up files
- Uploaded Proposal.md - less than a minute
- Filled out information for README.md - 5 minutes
- Setup .c and .h files - 2 minutes
- Setup makefile - 9 minutes

### 2025-01-07 - Setting up signal handler and sockets
- Setup up signal handler functions in server.c and into main(): 10 minutes
-Set up select sockets in networking.c in server_setup(): ~25 minutes

### 2025-01-08 - Finishing up sockets
- Still working on server_setup() in networking.c: 30 minutes

### 2025-01-09 - Figuring out game logic for blackjack and setting up deck
- Planning and figuring out how the game logic will work with code on paper and trying to implent a deck into server.c: 35 minutes

### 2025-01-10 to 2025-01-11 - Finishing sockets, server structure, and signals
- Change overall server/client structure: 10 minutes
- Implented server using sockets,bind, listen, and accept: 25 minutes
- Added select() loop to let server monitor multiple connections at once and fixing it: 20 minutes
- Reorganized networking code into networking.c / networking.h: 10 mins
- Creating blackjack.c and blackjack.h files and began with game logic liek creating struct for deck and shuffling function and printing out shuffled decks for player: 45 minutes
