package engineTester;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.Arrays;

import org.lwjgl.opengl.Display;
import org.lwjgl.util.vector.Vector3f;

import com.fazecast.jSerialComm.SerialPort;

import entities.Camera;
import entities.Entity;
import entities.Light;
import models.RawModel;
import models.TexturedModel;
import renderEngine.DisplayManager;
import renderEngine.Loader;
import renderEngine.MasterRenderer;
import renderEngine.OBJLoader;
import textures.ModelTexture;

public class MotionAnalyzer {
	
	public Entity imu;
	
	public MotionAnalyzer() throws SocketException {
		
		
		//new UDPServer(this);
		new SerialServer(this, 1, 115200);
		
		// Create Display
		DisplayManager.createDisplay();
		
		// Init OBJ Loader
		Loader loader = new Loader();
		
		// Create Camera, Renderer, Sun
		Camera camera = new Camera();
		MasterRenderer renderer = new MasterRenderer();
		Light sun = new Light(new Vector3f(20000, 20000, 2000), new Vector3f(1, 1, 1));
		
		// Create IMU Model
		RawModel model = OBJLoader.loadObjModel("imu", loader);
		TexturedModel staticModel = new TexturedModel(model, new ModelTexture(loader.loadTexture("imu")));
		imu = new Entity(staticModel, new Vector3f(0, 0, 0), 0, 0, 0, 3);

		// Main Loop
		while (!Display.isCloseRequested()) {
			// Update Camera
			camera.update();

			// Register IMU Model
			renderer.processEntity(imu);

			// Render Everything
			renderer.render(sun, camera);
			DisplayManager.updateDisplay();
		}

		// Clean Up, Destroy Display
		renderer.cleanUp();
		loader.cleanUp();
		DisplayManager.closeDisplay();
		System.exit(0);
		
	}
	float zz = 0;
	private float overallRoll = 0;
	private float overallPitch = 0;
	public void dataReceived(String msg) {
		try {
			if(imu != null) {
				String[] tokens = msg.split(" ");
				float x = Float.parseFloat(tokens[1].split(":")[1]);
				float y = Float.parseFloat(tokens[2].split(":")[1]);
				float z = Float.parseFloat(tokens[3].split(":")[1]);
				double dt = Double.parseDouble(tokens[4].split(":")[1]);

				if(msg.contains("ACC")) {
					double roll = Math.atan(y / Math.sqrt(Math.pow(x, 2) + Math.pow(z, 2))) * 60f;
					double pitch = Math.atan(-1 * x / Math.sqrt(Math.pow(y, 2) + Math.pow(z, 2))) * 60f;
					overallRoll = (float) (0.94f * overallRoll + 0.06f * roll);
					overallPitch = (float) (0.94f * overallPitch + 0.06f * pitch);
					imu.setRotX((float)overallRoll % 360);
					imu.setRotZ((float)overallPitch % 360);
				}
				if(msg.contains("GYRO")) {

					zz = (float) (zz - z * 500f * (dt / 1000f) / 9);
					imu.setRotY((float)zz % 360);

					System.out.println("(" + x + "," + y + "," + z + ") \t" + zz);
				}
				if(msg.contains("MAG")) {
					//System.out.println("(" + x + "," + y + "," + z + ")");
				}
				
			}
		}catch (Exception e) {
			System.out.println(msg);
		}
	}

	public static void main(String[] args) throws SocketException {
		new MotionAnalyzer();
	}

}

class SerialServer implements Runnable{
	
	private MotionAnalyzer analyzer;
	private SerialPort serial;
	
	public SerialServer(MotionAnalyzer analyzer, int port, int baudrate) {
		this.analyzer = analyzer;
		serial = SerialPort.getCommPorts()[port];
		serial.setBaudRate(baudrate);
		serial.openPort();
		
		new Thread(this).start();
	}
	
	@Override
	public void run() {
		String inBuff = "";
		while(true) {
			if(serial.bytesAvailable() >= 1) {
				byte[] b = new byte[1];
				serial.readBytes(b, 1);
				inBuff += (char)b[0];
				if(b[0] == 10) {
					analyzer.dataReceived(inBuff);
					inBuff = "";
				}
			}
		}
	}
	
}

class UDPServer implements Runnable{
	
	private MotionAnalyzer analyzer;
	private DatagramSocket serverSocket;
	private byte[] receiveData = new byte[1024];

	public UDPServer(MotionAnalyzer analyzer) throws SocketException{
		this.analyzer = analyzer;
		serverSocket = new DatagramSocket(4445);
		new Thread(this).start();
	}

	@Override
	public void run() {
		while(true) {
			try {
				DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
				serverSocket.receive(receivePacket);
				analyzer.dataReceived(new String(receivePacket.getData()).trim());
				receiveData = new byte[1024];
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
}
