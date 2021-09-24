package com.astromedicomp.helloworld;

//ADDED BY ME
import androidx.appcompat.widget.AppCompatTextView;

import android.content.Context;

import android.graphics.Color;

import android.view.Gravity;

public class MyView extends AppCompatTextView
{
	public Context drawingContext;
	public MyView(Context drawingContext)
	{
		super(drawingContext);
		setTextColor(Color.rgb(0,255,0));
		setTextSize(60);
		setGravity(Gravity.CENTER);
		setText("Hello World!!!");
	}
}