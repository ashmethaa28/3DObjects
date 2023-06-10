
-Starting Code for Assignment 3-
--------------------------------
-this is the same code as the rotating cube sample but it has the data removed
-the loaddata.js file is where you will store the data from the ppm and obj files
-run the code by clicking on index.html
-there is currently no data in the loaddata.js file so nothing will be displayed 
-when you write assignment 3 you will need to load the formatted data from your C/Python program into the functions contained in the loaddata.js file
-you can modify anything you wish in loaddata.js
-locations where the data is returned are marked in the loaddata.js file but you can change how the data is returned to the drawing code if you wish
-there are functions to load the v, vt, vn, and indices from the obj file
-there are functions to load the width, height, and texture data from the ppm file 

-you should not need to edit the webgl-demo.js file 
-you are not required to write css with the assignment


Types of Obj Files
------------------
We will only be using obj files that contain all three types of data (v,vt,vn). The f lines in these files will contain three values in each group (e.g. 2/4/10). Most of the sample obj files contain all three types of data but the cow.obj file does not. It does not contain the normal vn data and will not be used for testing in this assignment.


Scaling the Object
------------------
There is a getdistance() function which controls the size of the object. It actually controls how far away the object is moved away from the viewer when it is drawn. If a model is too large to be easily seem then you can increase the value in the getdistance() function and the object will appear smaller and can be made to fit in the window. Set this value so the objects are visible when they are drawn.


