/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.bullet;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.Toast;



public class BulletDemo extends Activity {

	private GLSurfaceView glView;
	private FrameLayout mainView;
	private ImageButton btnUP;
	private ImageButton btnDown;
	private ImageButton btnLeft;
	private ImageButton btnRight;
	private Button btnPlnLeft;
	private Button btnPlnRight;
	private Button btnToggleEmit;
	private MyClickListener clickListener;
	private BulletRenderer bulletRenderer;
	
    @Override 
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        
        setContentView(R.layout.activity_bullet);
        this.initId();
        
        bulletRenderer = new BulletRenderer(this);
        
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000;
        glView.setEGLContextClientVersion(3);
        glView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        glView.setRenderer(bulletRenderer);
        glView.setOnTouchListener(new OnTouchListener() {      	
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event != null) {    
                	//final float normalizedX =(event.getX() / (float) v.getWidth()) * 2 - 1;
                	//final float normalizedY =-((event.getY() / (float) v.getHeight()) * 2 - 1);
                	
                	if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    	final float normalizedX = event.getX();
                    	final float normalizedY = event.getY();
                    	
                    	final DisplayMetrics dm = new DisplayMetrics();  
                    	getWindowManager().getDefaultDisplay().getMetrics(dm);
                    	glView.queueEvent(new Runnable() {
							@Override
							public void run() {
								bulletRenderer.handleTouchPress(normalizedX, normalizedY, dm.heightPixels, dm.widthPixels);
								
							}
                        });
                    }
                    return true;                    
                } else {
                    return false;
                }
            }
        });
      
       
    }
    
            
            
    private void initId(){
    	   mainView = (FrameLayout)findViewById(R.id.parent_layout);
          
    	   glView = (GLSurfaceView)findViewById(R.id.glsurface_view);
           btnUP = (ImageButton) findViewById(R.id.btn_up);
           btnDown = (ImageButton) findViewById(R.id.btn_down);
           btnLeft = (ImageButton) findViewById(R.id.btn_left);
           btnRight = (ImageButton) findViewById(R.id.btn_right);
           
           btnPlnLeft = (Button) findViewById(R.id.btn_pln_left);
           btnPlnRight = (Button) findViewById(R.id.btn_pln_right);
           btnToggleEmit = (Button) findViewById(R.id.btn_toggle_emit);
           
           
           clickListener = new MyClickListener();
           btnUP.setOnClickListener(clickListener);
           btnDown.setOnClickListener(clickListener);
           btnLeft.setOnClickListener(clickListener);
           btnRight.setOnClickListener(clickListener);
           btnPlnLeft.setOnClickListener(clickListener);
           btnPlnRight.setOnClickListener(clickListener);
           btnToggleEmit.setOnClickListener(clickListener);
    }

    private class  MyClickListener implements OnClickListener{

		@SuppressLint("ShowToast")
		@Override
		public void onClick(View v) {
			switch(v.getId()){
			case R.id.btn_up:
				Toast.makeText(BulletDemo.this, "Front", Toast.LENGTH_LONG).show();
				
				glView.queueEvent(new Runnable() {
					@Override
					public void run() {
						bulletRenderer.stepFront();
						
					}
                });
				break;
				
			case R.id.btn_down:
				Toast.makeText(BulletDemo.this, "Back", Toast.LENGTH_LONG).show();
				glView.queueEvent(new Runnable() {
					@Override
					public void run() {
						bulletRenderer.stepBack();
						
					}
                });
				break;
				
			case R.id.btn_left:
				Toast.makeText(BulletDemo.this, "left", Toast.LENGTH_LONG).show();
				
				
				glView.queueEvent(new Runnable() {
					@Override
					public void run() {
						bulletRenderer.stepLeft();
						
					}
                });
				
				break;
				
			case R.id.btn_right:
				Toast.makeText(BulletDemo.this, "right", Toast.LENGTH_LONG).show();

				glView.queueEvent(new Runnable() {
					@Override
					public void run() {
						bulletRenderer.stepRight();
						
					}
                });
				break;
				
			case R.id.btn_pln_right:
				glView.queueEvent(new Runnable() {
					@Override
					public void run() {
						bulletRenderer.planeLeft();
						
					}
                });
				break;
				
			case R.id.btn_pln_left:
				glView.queueEvent(new Runnable() {
					@Override
					public void run() {
						bulletRenderer.planeRight();
						
					}
                });
				break;
				
			}
			
		}
    	
    }
    
    @Override protected void onPause() {
        super.onPause();

    }

    @Override protected void onResume() {
        super.onResume();

    }
}
