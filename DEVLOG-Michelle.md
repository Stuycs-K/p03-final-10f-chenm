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
- Planning and figuring out how the game logic will work with code on paper and trying to implement a deck into server.c: 35 minutes

### 2025-01-10 to 2025-01-11 - Finishing sockets, server structure, and signals
- Change overall server/client structure: 10 minutes
- Implemented server using sockets,bind, listen, and accept: 25 minutes
- Added select() loop to let server monitor multiple connections at once and fixing it: 20 minutes
- Reorganized networking code into networking.c / networking.h: 10 mins
- Creating blackjack.c and blackjack.h files and began with game logic like creating struct for deck and shuffling function and printing out shuffled decks for player: 45 minutes

### 2025-01-12 - Fixing select server connecting
- Fixing connection by including IP address of computer labs: 20 minutes
- Debugging networking issues: 20 minutes

### 2025-01-13 - Finishing game logic and asking for input
- Finish function hand_value which calculates value of deck: 15 minutes
- Printing "Enter command" and taking in input from user: 30 minutes

### 2025-01-14 - Finished game logic
- Finished function for hit: 14 mins
- Finished function for stand: 5 minutes
- Compares values and prints who won/lose/tied: 20 minutes
- Making a player state so you can't spam hit after you spammed: 20 minutes

### 2025-01-15 - Finished most of game
- Debuggin: 180 + minutes
- Finishing flush and broadcasting functions: 40 minutes
- Starting new round function: 40 minutes
