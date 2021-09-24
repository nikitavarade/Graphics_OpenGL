package com.astromedicomp.bluescreen;

//ADDED BY ME
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
//FOR EVENTS
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;
//OPENGL
import android.opengl.GLSurfaceView;
import android.opengl.view.GLES32;
import javax.microedition.khronous.opengles.GL10;
import javax.microedition.khronous.egl.EGLConfig;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;
	private GestureDetector gestureDetector;

	public GLESView(Context drawingContext)
	{
		super(drawingContext);
		context = drawingContext;

		setEGLContextClientVersion(3);

		setRenderer(this);

		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		gestureDetector = new GestureDetector(drawingContext,this,null,false);
		gestureDetector.setOnDoubleTapListener(this);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		//code
		int eventaction = event.getAction();
		if(!gestureDetector.onTouchEvent(event))
			super.onTouchEvent(event);
		return(true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent event)
	{
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent event)
	{
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent event)
	{
		return(true);
	}

	@Override
	public boolean onDown(MotionEvent event)
	{
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
	{
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent event)
	{
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
	{
		System.exit(0);
		return(true);
	}

	@Override
	public void onShowPress(MotionEvent event)
	{
	}

	@Override
	public boolean onSingleTapUp(MotionEvent event)
	{
		return(true);
	}

	/*IMPLEMENT GLSurfaceView.Renderer methods */
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		String version = gl.glGetString(GL10.GL_VERSION);
		System.out.println("RTR: "+version);
		initialize();
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height)
	{
		resize(width,height);
	}

	@Override
	public void onDrawFrame(GL10 unused)
	{
		display();
	}

	private void initialize()
	{
		GLES32.glClearColor(0.0f,0.0f,1.0f,0.0f);
	}

	private void resize(int width, int height)
	{
		GLES32.glViewport(0,0,width,height);
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
	
		requestRender();
	}
}
