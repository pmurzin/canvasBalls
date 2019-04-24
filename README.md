# canvasBalls
**Simple browser ball animation**

Run:
```
emcc lib/canvas.c -s WASM=1 -s EXPORTED_FUNCTIONS="['_getCircles','_main']" -o public/canvas.js

node server.js
```

Finally, go to localhost:2222 in your browser. Enjoy :)
