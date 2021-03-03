var columns = 64;
var rows = 95;
var canvas, ctx;
var size = 0;
var wall = new Image();
var wallLeftCorner = new Image();
var wallRightCorner = new Image();
var image = new Image();
image.src = "./image.png";
wall.src = "./wooden-wall-view-front.png";
wallLeftCorner.src = "./wooden-wall-front-left-corner.png";
wallRightCorner.src = "./wooden-wall-front-right-corner.png";
window.onload = function(){
    canvas = document.getElementById("canvas");
    canvas.width = 1000;
    canvas.height = 1000;
    ctx = canvas.getContext("2d");
    ctx.imageSmoothingEnabled = false;
    size = canvas.width / 16;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    drawImage(wallLeftCorner, size*2, size*5);
    drawImage(wall, size*3, size*5);
    drawImage(wall, size*4, size*5);
    drawImage(wall, size*5, size*5);
    drawImage(wall, size*6, size*5);
    drawImage(wall, size*7, size*5);
    drawImage(wallRightCorner, size*8, size*5);
}
function drawImage(im, x, y){
    ctx.drawImage(im, x, y, size, size); 
}