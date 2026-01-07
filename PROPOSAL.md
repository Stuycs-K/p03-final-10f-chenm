# Final Project Proposal

## Group Members: 

Michelle Chen

# Intentions:

A multiplayer game of blackjack. Players will be able to connect to a server where the computer plays the dealer. The server manages game logic while multiple clients connect as players.  Everyone in the session will be able to see the dealer's hand 
and they are assigned 2 random card values. Everyone must choose hit or stand and then the dealer plays until someone wins or loses. These rounds will repeat and the scores will be kept.

# Intended usage:

- Players should be able to connect to the main game server.
- Upon joining, players are added to the game session
- During the game, players can view their own cards, dealer's hand, and the scores.
- Players interact with the game by sending simple commands like hit or stand.
- The score is updated after every round.

# Technical Details:

- Sockets will be used to allow players to connect to the server and it accoets multiple connections.
- Using select to monitor multiple clients at a time
- fork to handle each client separately
- Signals will be use to end the game and clean up open sockets
- Server handles deck creation and shuffling, player and dealer hands, simple Blackjack rules, and synchronization 


# Intended pacing:

Get a working server that can handle multiple clients by the end of the week. Work on the technicalities of a game of blackjack over the weekened and debugging along the way. Try to get multiple people playing at once with the scores and game fully working. 
