import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Client implements Runnable{
	
	private DatagramSocket socket;
	private InetAddress address;
	private byte[] buf;
	
	public Client() throws Exception {
		socket = new DatagramSocket();
		address = InetAddress.getByName("192.168.1.22");
		
		new Thread(this).start();
		
		while(true) {
			buf = new byte[1000];
			DatagramPacket packet = new DatagramPacket(buf, buf.length);
	        socket.receive(packet);
	        System.out.println("UDPPacket Received: " + new String(packet.getData(), 0, packet.getLength()));
	        Thread.sleep(1000);
		}
	}

	int i = 0;
	@Override
	public void run() {
		while(true) {
			try {
				System.out.println("UDPPacket Sent: " + "Ping " + i);
				buf = ("Ping " + ++i).getBytes();
				DatagramPacket packet = new DatagramPacket(buf, buf.length, address, 4445);
				socket.send(packet);
				Thread.sleep(1000);
	        }catch (Exception e) {
				e.printStackTrace();
			}
		}
		
	}
	
}
