# canvasBalls
Simple browser ball animation

Run:

emcc lib/canvas.c -s WASM=1 -s EXPORTED_FUNCTIONS="['_getCircles','_main']" -o public/canvas.js

node server.js
