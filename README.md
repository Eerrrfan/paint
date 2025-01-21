Paint Application using OpenGL
This is a simple Paint Application implemented using OpenGL and GLUT. It allows users to draw shapes, lines, and freehand sketches with various colors and brush sizes. The application also includes features like saving the canvas as an image, clearing the canvas, and adjusting line thickness.

Features
Shapes: Draw lines, circles, and squares.

Freehand Drawing: Use the brush tool to draw freely.

Color Selection: Choose from predefined colors (Red, Green, Blue, Black) or use random colors.

Eraser: Erase parts of the drawing.

Line Thickness: Adjust the thickness of lines and shapes.

Save Canvas: Save the current canvas as a PNG image on your desktop.

Clear Canvas: Clear the entire canvas or just the lines.

Grid Background: A grid background is provided for better alignment.

Controls
Keyboard Shortcuts
1: Switch to Line drawing mode.

2: Switch to Circle drawing mode.

3: Switch to Square drawing mode.

R: Set color to Red.

G: Set color to Green.

B: Set color to Blue.

S: Set color to Black.

E: Activate Eraser.

C: Clear the entire canvas.

L: Clear only the lines.

A: Toggle Brush Mode for freehand drawing.

Q: Toggle Random Color Mode.

+: Increase line thickness.

-: Decrease line thickness.

W: Save the canvas as a PNG image on the desktop.

Space: Exit the application.

Mouse Controls
Left Click: Draw shapes or use the brush tool.

Left Click + Drag: Draw lines, circles, or squares dynamically.

Right Click: Exit the application.

Buttons
The application provides on-screen buttons for easy access to common features:

Line Button: Switch to line drawing mode.

Circle Button: Switch to circle drawing mode.

Square Button: Switch to square drawing mode.

Color Buttons: Red, Green, Blue, and Black.

Clear Button: Clear the entire canvas.

Line Clear Button: Clear only the lines.

Brush Button: Toggle brush mode.

Eraser Button: Activate the eraser.

Increase/Decrease Button: Adjust line thickness.

Save Button: Save the canvas as an image.

Random Color Button: Toggle random color mode.

How to Run
Prerequisites
OpenGL and GLUT libraries installed.

GLEW (OpenGL Extension Wrangler Library) installed.

A C++ compiler (e.g., GCC, MSVC).

Steps
Clone the repository:

bash
Copy
git clone https://github.com/your-username/Paint-Application.git
cd Paint-Application
Compile the program:

bash
Copy
g++ Paint.cpp -o Paint -lGL -lGLU -lglut -lGLEW
Run the executable:

bash
Copy
./Paint
Screenshots
Paint Application Screenshot

Code Structure
Paint.cpp: The main source file containing the OpenGL setup, drawing functions, and event handling.

stb_image_write.h: A single-file library for saving images in PNG format.

Dependencies
OpenGL: For rendering graphics.

GLUT: For window management and input handling.

GLEW: For managing OpenGL extensions.

stb_image_write: For saving the canvas as a PNG image.

License
This project is licensed under the MIT License. See the LICENSE file for details.

Contributing
Contributions are welcome! If you find any bugs or want to add new features, feel free to open an issue or submit a pull request.

Author
Your Name
GitHub: Eerrrfan
Email: erfan.ramezany22@gmail.com
