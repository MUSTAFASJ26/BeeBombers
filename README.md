# BeeBombers
This project, "Bee Bombers", is a 2D arcade-style game developed using C++ and the SFML (Simple and Fast Multimedia Library). The game combines elements of skill, strategy, and timing as the player battles worker bees, killer bees, and navigates a challenging environment filled with obstacles.




 Through this project, I have:
 
•	Mastered Vectors and Dynamic Data Structures: The use of vectors was integral in dynamically managing entities like bees, honeycombs, obstacles, and power-ups. This has strengthened my understanding of dynamic data structures and memory management.

•	Advanced SFML Proficiency: Implementing SFML to handle graphics, animations, textures, sprites, and audiohas significantly improved my skills in multimedia programming and real-time rendering.

•	Improved Programming Logic and Problem Solving: By implementing collision detection, event handling, and game mechanics such as spawning, movement, and scoring, I’ve honed my ability to break down complex systems into manageable components.

•	Proficiency in Modular Programming: Developing reusable functions and modular code has enhanced my ability to write maintainable and scalable software.

•	Debugging and Testing: Testing gameplay elements like collision systems, scoring, and transitions between levels taught me to rigorously debug and ensure functional correctness in larger systems.

•	Project Organization: Designing a complete game from scratch required structuring code logically, managing global and local states effectively, and integrating multiple interdependent features seamlessly.

This project has greatly boosted my confidence in tackling real-world programming challenges and solidified my foundation in game development.
 
Code Structure and Game Mechanics :


The "Bee Bombers" game code is structured into several interconnected components, each serving a distinct role. Here's an overview of the architecture and implementation:
1. Game Initialization and Setup
Each level starts with a dedicated setup function (e.g., playLevel1, playLevel2, playBossLevel) that initializes the environment:
•	Bees, honeycombs, obstacles, and the player are dynamically initialized using vectors to accommodate changing game states.
•	Background textures, music, and UI elements (score, lives) are reset and loaded for the current level.
•	Modular functions handle recurring tasks like resetting player state, generating entities, and configuring visual elements.
2. Game Logic and Mechanics
The game logic is driven by game loops that handle rendering, event processing, and gameplay updates:
•	Player Movement: The player moves left or right while avoiding obstacles and shooting bullets at bees.
•	Bee Behavior: Worker bees (blue) and killer bees (red) move dynamically and spawn obstacles upon reaching the ground.
•	Honeycomb System: Bees drop honeycombs when hit, which the hummingbird or player interacts with for further rewards or power-ups.
•	Scoring System: Scores are dynamically updated based on player actions (e.g., hitting bees, hummingbird eating honeycombs) and tier-based bonuses.
3. Level Progression and Boss Level
•	Levels progressively increase in difficulty with more bees, obstacles, and dynamic challenges.
•	The boss level combines all game elements, requiring mastery of mechanics and strategic use of power-ups and bonus lives to succeed.
4. Player Feedback and Leaderboard
•	Player scores and names are logged in a leaderboard system, promoting replayability and competition.
•	Visual cues, sound effects, and animations enhance gameplay immersion.
 
Game Rules and Mechanics
1. Bees and Obstacles
1.	Killer Bees (Red):
o	Spawn 3 obstacles on their first descent to the ground.
o	Spawn 2 obstacles on every subsequent descent.
2.	Worker Bees (Blue):
o	Spawn 2 obstacles on their first descent to the ground.
o	Spawn 1 obstacle on every subsequent descent.
2. Lives and Death
3.	The player starts with 3 lives (one currently active).
4.	The player dies if:
o	Stuck between obstacles and unable to move.
o	On top of obstacles when a power-up timer ends.
3. Honeycombs and Scoring
5.	Hitting Worker Bees drops Yellow Honeycombs.
6.	Hitting Killer Bees drops Red Honeycombs.
7.	Honeycombs themselves do not give points, but:
o	Hummingbird Eating Honeycombs earns points.
o	Hitting Bees earns points directly.
4. Scoring System
8.	Bee Hit Points:
o	Worker Bee: 100 points.
o	Killer Bee: 1,000 points.
9.	Hummingbird Eating Honeycombs (Tier-based Scoring):
o	Top Tier (1–2): Yellow Honeycomb: 1,000 points | Red Honeycomb: 2,000 points.
o	Middle Tier (3–5): Yellow Honeycomb: 800 points | Red Honeycomb: 1,800 points.
o	Bottom Tier (6–10): Yellow Honeycomb: 500 points | Red Honeycomb: 1,500 points.
5. Beehives
10.	Hitting bee hives neither adds nor deducts points.
11.	Bee hives left unhit at the end of a level earn bonus points:
o	Top Tier: 2,000 points.
o	Middle Tier: 1,600 points.
o	Bottom Tier: 1,000 points.
6. Bonus Lives
12.	A bonus life is awarded at:
o	20,000 points, 40,000 points, 80,000 points, and every 80,000 points thereafter.
13.	Bonus lives are crucial for survival, especially in the boss level.
7. Power-Ups
14.	Power-ups spawn when:
o	The hummingbird eats a honeycomb.
o	The player hits a red honeycomb.
15.	Power-ups allow the player to move on top of obstacles for 5 seconds. If the timer ends while the player is still on obstacles, the player dies.
 
This project demonstrates a comprehensive understanding of game mechanics, real-time rendering, and programming logic, culminating in a challenging and rewarding game experience.

