package com.droidloft;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;

public class OpenGLRenderer implements Renderer{
	Square square=new Square();
	float angle=0f;
	FloatBuffer colorBuffer;
	private float[] colors={
			1f, 0f,0f,1f,//vertex 0 red
			0f, 1f,0f,1f,//vertex 1 green
			0f, 0f,1f,1f,//vertex 2 blue
			1f, 0f,1f,1f //vertex 3 magenta
			};
	
	public void onSurfaceCreated(GL10 gl,EGLConfig config){
		
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//rgba,black
		gl.glShadeModel(GL10.GL_SMOOTH);
		gl.glClearDepthf(1.0f);
		gl.glEnable(GL10.GL_DEPTH_TEST);
		gl.glDepthFunc(GL10.GL_LEQUAL);
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT,GL10.GL_NICEST);
		
		ByteBuffer cbb=ByteBuffer.allocateDirect(colors.length*4);
		cbb.order(ByteOrder.nativeOrder());
		colorBuffer=cbb.asFloatBuffer();
		colorBuffer.put(colors);
		colorBuffer.position(0);
	}
	
	public void onDrawFrame(GL10 gl){
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT|GL10.GL_DEPTH_BUFFER_BIT);
		gl.glLoadIdentity();
		gl.glTranslatef(0, 0, -4);//GL will not draw near the view port,important
		gl.glColor4f(0.5f, 0.5f, 0.0f, 1.0f);//0x808000FF,not like translate, it will not be saved or recovered,affect all here after,until you set color again
		
		gl.glPushMatrix();
		gl.glRotatef(angle, 0, 0, 1);
		square.draw(gl);
		gl.glPopMatrix();
		
		gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
		gl.glColorPointer(4, GL10.GL_FLOAT, 0, colorBuffer);
		
		gl.glPushMatrix();
		gl.glRotatef(-angle, 0, 0, 1);
		gl.glTranslatef(2, 0, 0);
		gl.glScalef(0.5f, 0.5f, 0.5f);
		square.draw(gl);
		
		gl.glDisableClientState(GL10.GL_COLOR_ARRAY);
		
		gl.glPushMatrix();
		gl.glRotatef(-angle, 0, 0, 1);//the z
		gl.glTranslatef(2, 0, 0);
		
		gl.glScalef(0.5f, 0.5f, 0.5f);
		gl.glRotatef(angle*10, 0, 0, 1);
		square.draw(gl);
		gl.glPopMatrix();
		gl.glPopMatrix();
		angle++;
		//gl.glLoadIdentity();//because of the pop ,here not needed,but reset the matrix,important, try to comment this out, to see what happened
	}
	
	public void onSurfaceChanged(GL10 gl,int width,int height){
		
		gl.glViewport(0, 0, width, height);
		
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		GLU.gluPerspective(gl, 45.0f, (float)width/(float)height, 0.1f, 100.0f);
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
	}

}