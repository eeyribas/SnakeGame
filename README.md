# SnakeGame
Snake game is a game where the snake grows by eating apples and can pass through walls without dying. The game design is coded with OpenGL support.

Programming Language: C/C++ Programming Language

IDE: Visual Studio 2019

Output: The output of the game is shown in the SnakeGame.png image.

OpenGL Installation: To use OpenGL, copy the following paths from the glut-3.7.6-bin folder.

glut32.lib --> $(MSDevDir)....\VisualStudio\lib

glut.h  --> $(MSDevDir)....\VisualStudio\include

Running: By placing the glut32.dll file from the glut-3.7.6-bin folder into the "Debug" or "Release" folder of your compiled output, the snake game will run without errors.

Game Controls:

	Arrow Keys: Move the snake.

	'a': Increases the game's field of view along the x-axis.

	'A': Decreases the game's field of view along the x-axis.

	's': Increases the game's field of view along the y-axis.

	'S': Decreases the game's field of view along the y-axis.

	'd': Increases the game's field of view along the z-axis.

	'f': Increases the size of the snake.

	'F': Decreases the size of the snake.

	'g': Zoom in the game's field of view.

	'G': Zoom out the game's field of view.

	'e': Restart the game.

	'ESC': Close the game.