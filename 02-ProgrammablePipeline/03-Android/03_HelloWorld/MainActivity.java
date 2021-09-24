package com.astromedicomp.helloworld;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
//added by me
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.view.View;


public class MainActivity extends AppCompatActivity {

	private MyView myView;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
       // setContentView(R.layout.activity_main); //get rid of the titlebar
    	this.supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
    	//MAKE FULLSCREEN
    	this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
    	//FORCED LANDSCAPE ORIENTATION
    	this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    	//SET BACKGROUND COLOR
    	this.getWindow().getDecorView().setBackgroundColor(Color.BLACK);
    	//REMOVE NAVIGATION BAR
    	this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
    	//DEFINE OUR OWN VIEW
    	myView = new MyView(this);
    	//SET THIS VIEW AS OUR VIEW
    	setContentView(myView);
    }

    @Override 
    protected void onPause()
    {
    	super.onPause();
    }

    @Override
    protected void onResume()
    {
    	super.onResume();
    }

}
