package toolbox;

import org.lwjgl.util.vector.Matrix4f;
import org.lwjgl.util.vector.Quaternion;
import org.lwjgl.util.vector.Vector3f;

import entities.Camera;

public class Maths {

	public static Matrix4f createTransformationMatrix(Vector3f translation, float rx, float ry, float rz, float scale) {
		Matrix4f matrix = new Matrix4f();
		matrix.setIdentity();
		Matrix4f.translate(translation, matrix, matrix);
		Matrix4f.rotate((float) Math.toRadians(rx), new Vector3f(1, 0, 0), matrix, matrix);
		Matrix4f.rotate((float) Math.toRadians(ry), new Vector3f(0, 1, 0), matrix, matrix);
		Matrix4f.rotate((float) Math.toRadians(rz), new Vector3f(0, 0, 1), matrix, matrix);
		Matrix4f.scale(new Vector3f(scale, scale, scale), matrix, matrix);
		return matrix;
	}
	
	public static Matrix4f createTransformationMatrix(Vector3f translation, Quaternion q, float scale) {
		Matrix4f matrix = new Matrix4f();
		matrix.setIdentity();
		Matrix4f.translate(translation, matrix, matrix);
		Matrix4f.mul(matrix, convertQuaternionToMatrix4f(q), matrix);
		Matrix4f.scale(new Vector3f(scale, scale, scale), matrix, matrix);
		return matrix;
	}
	
	public static Matrix4f convertQuaternionToMatrix4f(Quaternion q)
    {
        Matrix4f matrix = new Matrix4f();
        matrix.m00 = 1.0f - 2.0f * ( q.getY() * q.getY() + q.getZ() * q.getZ() );
        matrix.m01 = 2.0f * (q.getX() * q.getY() + q.getZ() * q.getW());
        matrix.m02 = 2.0f * (q.getX() * q.getZ() - q.getY() * q.getW());
        matrix.m03 = 0.0f;

        matrix.m10 = 2.0f * ( q.getX() * q.getY() - q.getZ() * q.getW() );
        matrix.m11 = 1.0f - 2.0f * ( q.getX() * q.getX() + q.getZ() * q.getZ() );
        matrix.m12 = 2.0f * (q.getZ() * q.getY() + q.getX() * q.getW() );
        matrix.m13 = 0.0f;

        matrix.m20 = 2.0f * ( q.getX() * q.getZ() + q.getY() * q.getW() );
        matrix.m21 = 2.0f * ( q.getY() * q.getZ() - q.getX() * q.getW() );
        matrix.m22 = 1.0f - 2.0f * ( q.getX() * q.getX() + q.getY() * q.getY() );
        matrix.m23 = 0.0f;

        matrix.m30 = 0;
        matrix.m31 = 0;
        matrix.m32 = 0;
        matrix.m33 = 1.0f;
 
        return matrix;
    }

	public static Matrix4f createViewMatrix(Camera camera) {
		Matrix4f viewMatrix = new Matrix4f();
		viewMatrix.setIdentity();
		Matrix4f.rotate((float) Math.toRadians(camera.getPitch()), new Vector3f(1, 0, 0), viewMatrix, viewMatrix);
		Matrix4f.rotate((float) Math.toRadians(camera.getYaw()), new Vector3f(0, 1, 0), viewMatrix, viewMatrix);
		Vector3f cameraPos = camera.getPosition();
		Vector3f negativeCameraPos = new Vector3f(-cameraPos.x, -cameraPos.y, -cameraPos.z);
		Matrix4f.translate(negativeCameraPos, viewMatrix, viewMatrix);
		return viewMatrix;
	}

}
