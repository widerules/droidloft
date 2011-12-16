package com.droidloft;

import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;

public class OpenGLRenderer implements Renderer{
	Activity atvt;
	Square square=new Square();
	float angle=0f;
	FloatBuffer colorBuffer;
	TextureSprite ts;
	
	public void onSurfaceCreated(GL10 gl,EGLConfig config){
		
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//rgba,black
		gl.glColor4f(0.5f, 0.5f, 0.0f, 1.0f);//0x808000FF,not like translate, it will not be saved or recovered,affect all here after,until you set color again
		gl.glShadeModel(GL10.GL_SMOOTH);
		gl.glClearDepthf(1.0f);
		gl.glEnable(GL10.GL_DEPTH_TEST);
		gl.glDepthFunc(GL10.GL_LEQUAL);
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT,GL10.GL_NICEST);
		
		float[] colors={
				1f, 0f,0f,1f,//vertex 0 red
				0f, 1f,0f,1f,//vertex 1 green
				0f, 0f,1f,1f,//vertex 2 blue
				1f, 0f,1f,1f //vertex 3 magenta
				};
		
		colorBuffer=Util.convertToFloatBuffer(colors);
		ts=new TextureSprite(atvt,"sds.bmp");//can be png, the file need to be adjust, to get better or to be displayed, size must be power of 2
	}
	
	public void onDrawFrame(GL10 gl){
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT|GL10.GL_DEPTH_BUFFER_BIT);
		
		//==================================
		gl.glPushMatrix();
		
		gl.glRotatef(angle, 0, 0, 1);
		ts.draw(gl, -1f, -1f, 2, 2);

		gl.glPopMatrix();
		
		//========================
		gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
		gl.glColorPointer(4, GL10.GL_FLOAT, 0, colorBuffer);
		
		gl.glPushMatrix();
		gl.glRotatef(-angle, 0, 0, 1);
		gl.glTranslatef(2, 0, 0);
		gl.glScalef(0.5f, 0.5f, 0.5f);
		square.draw(gl);
		
		gl.glDisableClientState(GL10.GL_COLOR_ARRAY);
		//==========================
		
		gl.glRotatef(-angle, 0, 0, 1);//the z
		gl.glTranslatef(2, 0, 0);
		
		gl.glScalef(0.5f, 0.5f, 0.5f);
		gl.glRotatef(angle*10, 0, 0, 1);
		square.draw(gl);

		gl.glPopMatrix();
		
		angle++;
	}
	
	public void onSurfaceChanged(GL10 gl,int width,int height){
		float ratio=(float)width/(float)height;
		gl.glViewport(0, 0, width, height);
		
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();////because of the pop ,not needed in the end of draw frame,but reset the matrix,important, try to comment this out, to see what happened
		GLU.gluPerspective(gl, 45.0f, ratio, 10f, 60f);
		//gl.glFrustumf(-3*ratio, 3*ratio, -3, 3, 10, 60);
		//gl.glOrthof(-3*ratio, 3*ratio, -3, 3, 10, 60);
		//GLU.gluOrtho2D(gl,-3*ratio, 3*ratio, -3f, 3f);
		
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		
		gl.glTranslatef(0, 0, -20f);//important ,GL will not draw ,if z <=near;for GLU.gluOrtho2D ,do not need this
	}
	public OpenGLRenderer(Activity atvt){
		super();
		this.atvt=atvt;
	}
}
