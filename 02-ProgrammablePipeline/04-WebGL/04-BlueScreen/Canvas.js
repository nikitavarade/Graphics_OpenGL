//global variables

var canvas = null;
var gl= null;//webgl context
var bFullscreen=false;
var canvas_original_width;
var canvas_original_height;

//to start animation: To have requestAnimationFrame() to be called crossed browser compatible
var requestAnimationFrame = window.requestAnimationFrame || 
							window.webkitRequestAnimationFrame ||
							window.mozRequestAnimationFrame ||
							window.oRequestAnimationFrame ||
							window.msRequestAnimationFrame;
							
//to stop animation
var cancelAnimationFrame = window.cancelAnimationFrame ||
						   window.webkitCancelRequestAnimationFrame ||
						   window.webkitCancelAnimationFrame ||
						   window.mozCancelRequestAnimationFrame ||
						   window.mozCancelAnimationFrame ||
						   window.oCancelRequestAnimationFrame ||
						   window.oCancelAnimationFrame ||
						   window.msCancelRequestAnimationFrame ||
						   window.msCancelAnimationFrame;


//onload function
function main()
{
	//get <canvas> element
	canvas = document.getElementById("AMC");
	if(!canvas)
		console.log("Obtaining Canvas Failed\n");
	else
		console.log("Obtaining Canvas Succeeded\n");
		
	canvas_original_width =canvas.width;
	canvas_original_height =canvas.height;	
	
	//register keyboard's keydown event handler
	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);
	window.addEventListener("resize", resize, false);

	//initialize webgl
	init();
	
	//start drawing here as a warming up
	resize();
	draw();
}

function toggleFullScreen()
{
	//code
	var fullscreen_element = document.fullscreenElement || 
							 document.webkitFullscreenElement ||
							 document.mozFullScreenElement ||
							 document.msFullscreenElement ||
							 null;
							 
	//if not fullscreen
	if(fullscreen_element == null)
	{
		if(canvas.requestFullscreen)
				
				canvas.requestFullscreen();
		
		else if(canvas.mozRequestFullScreen)
		
				canvas.mozRequestFullScreen();
				
		else if(canvas.webkitRequestFullscreen)
		
				canvas.webkitRequestFullscreen();
		
		else if(canvas.msRequestFullscreen)
		
				canvas.msRequestFullscreen();
	}
	
	else //if already fullscreen
	{
		if(document.exitFullscreen)
			document.exitFullscreen();
		
		else if(document.mozCancelFullScreen)
			document.mozCancelFullScreen();
			
		else if(document.webkitExitFullscreen)
			document.webkitExitFullscreen();
	}
}

function init()
{

	//code
	//get WebGL 2.0 context
	gl= canvas.getContext("webgl2");
	if(gl==null) //failed to get context.
	{
		console.log("Failed to get the rendering context for WebGL");
		return;
	}
	gl.viewportWidth = canvas.width;
	gl.viewportHeight = canvas.height;
	
	//set clear color
	gl.clearColor(0.0,0.0,1.0,1.0); //blue
	
}

function resize()
{
		//code
		if(bFullscreen == true)
		{
			canvas.width = window.innerWidth;
			canvas.height = window.innerHeight;
		}
		else
		{
			canvas.width = canvas_original_width;
			canvas.height = canvas_original_height;
		}
	//set the viewport to match
	gl.viewport(0,0,canvas.width, canvas.height);
}

function draw()
{
	//code
	gl.clear(gl.COLOR_BUFFER_BIT);
	//animation loop
	requestAnimationFrame(draw,canvas);
}

	
function keyDown(event)
{
	//code
	switch(event.keyCode)
	{
		case 70: //for 'F' or 'f'
		toggleFullScreen();
		break;
	}
}

function mouseDown(event)
{
//code
}
