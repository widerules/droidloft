package com.moandroid.cocos2d.actions.camera;

import com.moandroid.cocos2d.util.CCUtils;
import com.moandroid.cocos2d.actions.interval.CCIntervalAction;
import com.moandroid.cocos2d.camera.CCCamera;
import com.moandroid.cocos2d.nodes.CCNode;

public class CCOrbitCamera extends CCCameraAction{

	   private float radius;
	    private float deltaRadius;
	    private float angleZ;
	    private float deltaAngleZ;
	    private float angleX;
	    private float deltaAngleX;

	    private float radZ;
	    private float radDeltaZ;
	    private float radX;
	    private float radDeltaX;

	    public static CCOrbitCamera action(float t, float r, float dr, float z, float dz, float x, float dx) {
	        return new CCOrbitCamera(t, r, dr, z, dz, x, dx);
	    }

	    protected CCOrbitCamera(float t, float r, float dr, float z, float dz, float x, float dx) {
	        super(t);

	        radius = r;
	        deltaRadius = dr;
	        angleZ = z;
	        deltaAngleZ = dz;
	        angleX = x;
	        deltaAngleX = dx;

	        radDeltaZ = CCUtils.CC_DEGREES_TO_RADIANS(dz);
	        radDeltaX = CCUtils.CC_DEGREES_TO_RADIANS(dx);
	    }

	    public CCIntervalAction copy() {
	        return new CCOrbitCamera(_duration, radius, deltaRadius, angleZ, deltaAngleZ, angleX, deltaAngleX);
	    }


	    @Override
	    public void start(CCNode aTarget) {
	        super.start(aTarget);
	        float[] r = new float[1], zenith = new float[1], azimuth = new float[1];

	        spherical(r, zenith, azimuth);
	        if (Float.isNaN(radius))
	            radius = r[0];
	        if (Float.isNaN(angleZ))
	            angleZ = CCUtils.CC_RADIANS_TO_DEGREES(zenith[0]);
	        if (Float.isNaN(angleX))
	            angleX = CCUtils.CC_RADIANS_TO_DEGREES(azimuth[0]);

	        radZ = CCUtils.CC_DEGREES_TO_RADIANS(angleZ);
	        radX = CCUtils.CC_DEGREES_TO_RADIANS(angleX);
	    }

	    @Override
	    public void update(float t) {
	        float r = (radius + deltaRadius * t) * CCCamera.zEye();
	        float za = radZ + radDeltaZ * t;
	        float xa = radX + radDeltaX * t;

	        float i = (float) Math.sin(za) * (float) Math.cos(xa) * r + centerXOrig;
	        float j = (float) Math.sin(za) * (float) Math.sin(xa) * r + centerYOrig;
	        float k = (float) Math.cos(za) * r + centerZOrig;

	        _target.camera().setEye(i, j, k);
	    }

	    private void spherical(float newRadius[], float zenith[], float azimuth[]) {
	        float[] ex = new float[1], ey = new float[1], ez = new float[1];
	        float[] cx = new float[1], cy = new float[1], cz = new float[1];
	        float x, y, z;
	        float r; // radius
	        float s;

	        _target.camera().eye(ex, ey, ez);
	        _target.camera().center(cx, cy, cz);

	        x = ex[0] - cx[0];
	        y = ey[0] - cy[0];
	        z = ez[0] - cz[0];

	        r = (float) Math.sqrt(Math.pow(x, 2) + (float) Math.pow(y, 2) + Math.pow(z, 2));
	        s = (float) Math.sqrt(Math.pow(x, 2) + (float) Math.pow(y, 2));

	        if (s == 0.0f)
	            s = 0.00000001f;
	        if (r == 0.0f)
	            r = 0.00000001f;

	        zenith[0] = (float) Math.acos(z / r);
	        
	        if (x < 0)
	            azimuth[0] = (float) Math.PI - (float) Math.asin(y / s);
	        else
	            azimuth[0] = (float) Math.asin(y / s);

	        newRadius[0] = r / CCCamera.zEye();
	    }

}
