<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>WebGL Demo</title>
    <link rel="stylesheet" href="./webgl.css" type="text/css">
    <?php
      $output = shell_exec('export PYTHONPATH="${PYTHONPATH}:./"');
      print exec("./a1 cow");
    ?>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js"
      integrity="sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ=="
      crossorigin="anonymous" defer>
    </script>
    <script src="webgl-demo.js" defer></script>
    <script src="loaddata.js" ></script>
  </head>

  <body>
    <canvas id="glcanvas" width="640" height="480"></canvas>
  </body>
</html>
