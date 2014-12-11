package com.bullet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;







import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;

public class BulletRenderer implements Renderer{
	private final Context context;
	GL10 gl;
	int width, height;
	boolean init = true;
	
	public BulletRenderer(Context context) {
        this.context = context;
    }
	public void onDrawFrame(GL10 gl) {
		
        DemoLib.step();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
    	this.width = width;
    	this.height = height;
		DemoLib.change(width, height);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        this.gl = gl;
        DemoLib.create();
    }
	
	public void handleTouchPress(float normalizedX, float normalizedY, int heightPixels, int widthPixels){
    	DemoLib.add(normalizedX,normalizedY,heightPixels,widthPixels);
    	
    }
	
	public void stepLeft(){
		DemoLib.stepLeft(this.width, this.height);
		
	}
	public void stepFront(){
		DemoLib.stepFront(this.width, this.height);
		
	}
	
	public void stepBack(){
		DemoLib.stepBack(this.width, this.height);
		
	}
	public void stepRight(){
		DemoLib.stepRight(this.width, this.height);
		
	}
	
	public void planeRight(){
		DemoLib.planeRight();
	}

	public void planeLeft(){
		DemoLib.planeLeft();
	}
}
