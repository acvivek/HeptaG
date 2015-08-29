# HeptaG
HeptaG the 7 in 1 game built in C++

7G   (7 in 1 Game )
By
A.C.VIVEK

1.BRICKS

The popular Brick breaking or ARKANOID game in which the player moves a paddle to strike a ball against a brick wall thereby breaking it
Totally there are 50 bricks.

2.CANNON BALL

In this game the player is to strike a tower located at a random distance.The player has to specify the velocity and the angle wrt horizontal the cannon is to be fired .The cannon takes a parabolic path.There are 5 levels & the height of the tower decreases in every level wheras the no. of towers to be hit and points obtained increase.
If a cannon doesnt hit a tower u lose a life (max 3 per level).

3.ALPHA BETA

This is an extension of the word game cows & bulls.
The computer thinks of a word which is to be guessed by the player it may be a 4 ,5 ,6 letter word.
The player gives words to the computer based on which the computer gives the values of A & B.The player also needs to specify the entry ie left or right.
A is the number of letters in the correct position and B is the no. of letters in the wrong position.
eg.
Computer thinks of the word QUICK

player gives the word             FLICK
here I , C,K  are in correct position hence A=3 B=0
player gives the word             USHER
here U is present in the word but at wrong position hence  A=0 B=1
Now let us see an eg in which the no. of letters are not the same
user  gives the word                GIVE
and specifies LEFT entry
here the computer compares QUIC  with GIVE ie, 1st 4 letters and gives result
A=0,B=1
user gives word QUIT and specifies RIGHT entry
the computer compares UICK with QUIT ie last 4 letters and gives the result
A=0 B=2

4.NIBBLE

This is a popular game also known as snake or worm(blue object)  in which the player moves the worm around in order to obtain food ( red object)  while doing so the worm should not collide with itself or the boundary which results in loss of life . There are 3 lives per level and 5 levels in all & at each higher level the worm moves faster.

5.BATTLE SHIP

In this game the computer sets a battlefield with some destructive weapons at strategic positions which the player has to find and destroy .The player gets 4 chances every cycle and the computer tells the player which of its weapons are destroyed.
The computer has 
5 Aircrafts arranged as 1 central and 1 each at every diagonal position
4 Battlers arranged in a vertical line
3 Cruisers arranged in a horizontal line
2 Destroyers arranged in a diagonal
1 Submarine 
The player clicks on a blank position and after 4 such chances the computer gives the weapons that have been destroyed 
eg. abxc	if 1 A 1B & 1C have been destroyed.
The player is to give the next set of positions based on the relative positions of the weapons.

6.GOBBLE
In this the player 'H' who wears blue color is meant to collect diamonds 'D' (yellow) without colliding with his enemies 'M' (red)who track him down .Collision results in loss of life  of both the enemy and the player .The player can reappear on the other side of the arena .A slow starter also loses out because he is 'gobbled' by his enemies.

7.HUNTER
In the game the player(BROWN) moves a gun around with the aim of shooting the target(RED) which also shoots back .Both adversaries have an initial health of 100 each stationary shootin doesnt help as the target will overpower the stationary player and the player has 150 bullets to shoot.
