package com.droidloft;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.opengles.GL10;

public class Square {
	private FloatBuffer vertexBuffer;
	private ShortBuffer indexBuffer;
	public Square(){
		float[] vertices={
				-0.5f, 0.5f,0.0f,//0,top left
				-0.5f,-0.5f,0.0f,//1,bottom left
				 0.5f,-0.5f,0.0f,//2,bottom right
				 0.5f, 0.5f,0.0f,//3,top right
				};
		vertexBuffer=Util.convertToFloatBuffer(vertices);
		
		short[] indices={0,1,2,0,2,3};
		indexBuffer=Util.convertToShortBuffer(indices);
	}
	
	public void draw(GL10 gl){
		gl.glFrontFace(GL10.GL_CCW);
		gl.glEnable(GL10.GL_CULL_FACE);
		gl.glCullFace(GL10.GL_BACK);
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0,vertexBuffer);//3 elements(xyz) per group
		gl.glDrawElements(GL10.GL_TRIANGLES, 6,/*indices.length*/
				GL10.GL_UNSIGNED_SHORT, indexBuffer);
		//gl.glDrawArrays(mode, first, count);
		
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glDisable(GL10.GL_CULL_FACE);
		
	}
}
