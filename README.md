# NSES - Natural Selection Evolution Simulator
This program simulates natural selection in a small world. The program initially spawns animals with random 'genes' which define their movement pattern. The plants are spawned randomly in the world area but their concentration is more in the middle of the world, called the 'jungle' area. Each animal has a fixed amount of energy. The animals must move efficiently so as to encounter maximum and gain their energy by eating them. 

## World Layout

                                 The World Layout
       _______________________________________________________________________
       |                                                                      |
       |                                                          #           |
       |                       O                                              |
       |                                           #                          |
       |                             #                    O                   |
       |                                                                      |
       |   #         #                                                        |
       |                                                              #       |
       |                      _______________________                         | 
       |                      |                      |                        |
       |                      |   #          O     # |                        |
       |                      |      #  O            |                        |
       |                      |               #   #  |                        |
       |                      |   #      #     #     |                        |
       |      #               |       O      #    O  |                        |
       |                      |______________________|                        |
       |                                                          O           |
       |                                                                      |
       |                                                                      |
       |                                                                      |
       |                O                      #                              |
       |                                                                      |
       |                                                     #                |
       |                                                                      |
       |______________________________________________________________________|
         100 X 30 UNITS
         
- The middle jungle has more dense vegetation than the rest of the world
- The animals are represented by 'O' and plants by '#'
- The world is contnuous. if animal crosses boundry, it willl appear on the orther side
        
## The animals
Each animal has a set of eight numbers associated with it called its 'gene'. They represent the relative probability of moving in that particular direction.

        the direction coding of an animal :
                    
                    0   1   2
                    
                    7   A   3
                    
                    6   5   4
                    
        each number corresponds to the direction animal is facing
        
The animals reproduce by binary fission if they have a certain amount of energy. The genes are exactly copied but a small amount of mutation can occur which allows for evolution. The animals with most suitable genes survive.

## Compiling and running
The program can be compiled using
    
    g++ nses.cpp -o nses -pthread -std=c++11
    
The graph.cpp program can be used to draw live time graphs while the program is running. Compile it and run after the nses program starts running using the following command 
  
    watch -n 0.2 ./graph
    
## Commands
program commands 
    
    p   - pause
    s   - start
    x   - exit 
    d   - display animal data (can be only used when the simulation is paused)
    g   - skip 1000 generations forward
    .   - increase simulation speed
    ,   - decrease simulation speed

animal color modes

    1   - animal colors based on energy level. blue - high, red - low, white - normal.
    2   - animal colors based on detected specie
