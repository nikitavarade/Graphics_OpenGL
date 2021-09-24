package com.astromedicomp.applog;

//ADDED BY ME
import androidx.appcompat.widget.AppCompatTextView;
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;

//FOR EVENTS
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

public class GLESView extends AppCompatTextView implements OnGestureListener, OnDoubleTapListener
{
	private GestureDetector gestureDetector;

	public GLESView(Context drawingContext)
	{
		super(drawingContext);
		setTextColor(Color.rgb(0,255,0));
		setTextSize(60);
		setGravity(Gravity.CENTER);
		setText("Hello World!!!");

		gestureDetector = new GestureDetector(drawingContext,this,null,false);
		gestureDetector.setOnDoubleTapListener(this);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		//code
		ystem.out.println("RTR: Iam in onTouchEvent");
		int eventaction = event.getAction();
		if(!gestureDetector.onTouchEvent(event))
			super.onTouchEvent(event);
		return(true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent event)
	{
		System.out.println("RTR: Iam in onDoubleTap");
		setText("Double Tap");
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent event)
	{
		System.out.println("RTR: Iam in onCreonDoubleTapEventate");
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent event)
	{
	public boolean onSingleTapConfirmed(MotionEvent event)
		System.out.println("RTR: Iam in onSingleTapConfirmed");
		setText("Single Tap");
		return(true);
	}

	@Override
	public boolean onDown(MotionEvent event)
	{

		System.out.println("RTR: Iam in onDown");
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
	{
		System.out.println("RTR: Iam in onFling");
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent event)
	{
		System.out.println("RTR: Iam in onLongPress");
		setText("Long Press");
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
	{
		System.out.println("RTR: Iam in onScroll");
		System.exit(0);
		return(true);
	}

	@Override
	public void onShowPress(MotionEvent event)
	{
				System.out.println("RTR: Iam in onShowPress");

	}

	@Override
	public boolean onSingleTapUp(MotionEvent event)
	{
				System.out.println("RTR: Iam in onSingleTapUp");

		return(true);
	}
}
