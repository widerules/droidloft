package com.droidloft;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class TextureSprite {
	int _texId;
	
	String path;
	Bitmap mBitmap;
	static Activity atvt;

	private FloatBuffer vertexBuffer;
	private FloatBuffer mCoordinates;

	public TextureSprite(Activity atvt, String path) {
		TextureSprite.atvt = atvt;
		this.path = path;

		if (mBitmap == null) {
			ByteBuffer tfb = ByteBuffer.allocateDirect(2 * 4 * 4);//texture buffer,2(x,y)*4num*4byte
			tfb.order(ByteOrder.nativeOrder());
			mCoordinates = tfb.asFloatBuffer();
			mBitmap = Util.createTextureBitmapFromFilePath(atvt,path,mCoordinates);
		}
	}

	public void applyTexParameters(GL10 gl) {
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER,
				GL10.GL_LINEAR);
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER,
				GL10.GL_LINEAR);
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_S,
				GL10.GL_CLAMP_TO_EDGE);
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_T,
				GL10.GL_CLAMP_TO_EDGE);
	}
	
	public void draw(GL10 gl, float x, float y, int w, int h) {

		gl.glEnable(GL10.GL_TEXTURE_2D);
		gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);

		if (_texId == 0) {
			int[] textures = new int[1];
			gl.glGenTextures(1, textures, 0);

			_texId = textures[0];
			gl.glBindTexture(GL10.GL_TEXTURE_2D, _texId);

			// gl.glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			applyTexParameters(gl);
			GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, mBitmap, 0);
			mBitmap.recycle();
			mBitmap = null;
		}
		
		gl.glBindTexture(GL10.GL_TEXTURE_2D, _texId);
		if(vertexBuffer==null){
			float vertices[]  = { 
					x, h + y, 0.0f,// left top
					w + x, h + y, 0.0f,// right top
					x, y, 0.0f,// left bootom
					w + x, y, 0.0f // right bottom
			};//  z sequence,has different order with texture coordinates,
			
			vertexBuffer=Util.convertToFloatBuffer(vertices);
		}
		// short indices = { 0, 1, 2, 3, 2, 1 };
		// mIndices.put(indices);
		// mIndices.position(0);
		// gl.glBindTexture(GL_TEXTURE_2D, _name);

		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_S, GL10.GL_REPEAT);
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_T, GL10.GL_REPEAT);

		gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, mCoordinates);
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);

		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4);

		// gl.glDrawElements(GL10.GL_TRIANGLES, 6, GL10.GL_UNSIGNED_SHORT,
		// mIndices);
		// Clear the vertex and color arrays
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		gl.glDisable(GL10.GL_TEXTURE_2D);
	}
}
