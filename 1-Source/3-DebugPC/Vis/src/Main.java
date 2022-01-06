import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JPanel;


public class Main extends JPanel implements MouseListener, MouseMotionListener, KeyListener, WindowListener{
	
	private static final long serialVersionUID = 1L;
	
	public static final int SCREEN_WIDTH = 1024;
	public static final int SCREEN_HEIGHT = 768;
	
	private static final int gramsPerCmBody = 9;
	private static final int waterDensity = 1;
	private static final float SHIP_r_mm = 50f;
	private ArrayList<String> comments;
	private ArrayList<Rect> rects;
	private Rect selected, hover;
	private int oldX, oldY;
	private Image offScreenImageDrawed = null;
    private Graphics offScreenImg = null;  
    private int currentMode = 0;
    private int buffer = 0;
    private int totalFD = 0, totalMass = 0, totalVolume = 0, totalFY = 0, totalBFX = 0;
	private Rect body;

	public Main() {
		setBackground(Color.white);
			    
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		rects = new ArrayList<Rect>();
		comments = new ArrayList<String>();
		
		try {
			BufferedReader br = new BufferedReader(new FileReader("asd.txt"));

            String line;
            while ((line = br.readLine()) != null) {
            	if(line.startsWith("#")) {
            		comments.add(line.substring(1));
            	}else if(line.startsWith("R")) {
            		String[] tokens = line.split(" ");
            		rects.add(new Rect(tokens[1], Integer.parseInt(tokens[2]), Integer.parseInt(tokens[3]), Integer.parseInt(tokens[4]), Integer.parseInt(tokens[5]), Integer.parseInt(tokens[6]), Integer.parseInt(tokens[7]), Integer.parseInt(tokens[8]), Integer.parseInt(tokens[9])));
            	}
            }
            br.close();
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		
		Rect h = null;
		Rect t = null;
		for(Rect rr : rects) {
			if(rr.name.equals("Head")) {
				h = rr;
			}
			if(rr.name.equals("Tail")) {
				t = rr;
			}
		}
		body = new Rect("Body", 
						(h.x - h.width / 2 + t.x + t.width / 2) / 2,
						768 / 2,
						h.x - t.x - t.width / 2 - h.width / 2,
						10,
						(int)((((h.x - h.width / 2.0f) - (t.x + t.width / 2.0f)) / 10.0f) * gramsPerCmBody),
						0,
						0,
						0);
		body.volume = (int)(3.14f * SHIP_r_mm * SHIP_r_mm * body.width / 1000f);

		
	}

	public void paint(Graphics g) {
        Dimension d = getSize();
        if (offScreenImageDrawed == null) 
            offScreenImageDrawed = createImage(d.width, d.height);   
        
        offScreenImg = offScreenImageDrawed.getGraphics();      
        offScreenImg.setColor(new Color(255, 255, 204));
        offScreenImg.fillRect(0, 0, d.width, d.height);
        totalFD = 0;
        totalFY = 0;
        totalBFX = 0;
        totalMass = 0;
        totalVolume = 0;
        
        rects.add(body);
    	for(Rect r : rects) {
    		offScreenImg.setColor(Color.BLACK);
    		offScreenImg.fillRect(r.x - r.width / 2, r.y - r.height / 2, r.width, r.height);
    		offScreenImg.setColor(new Color(255,100,30,100));
    		offScreenImg.drawString(r.name, (int) (r.x - g.getFontMetrics().getStringBounds(r.name, g).getWidth() / 2), r.y);

    		totalFD += (r.x - 1024 / 2) * r.weight;
    		totalFY += (r.y - 768 / 2) * r.weight;
    		totalBFX += (r.x - 1024 / 2 + r.fBuoyancyXDiff) * r.volume * waterDensity;
    		totalVolume += r.volume;
    		totalMass += r.weight;
    		
    	}
    	
    	rects.remove(body);
    	
    	offScreenImg.setColor(Color.cyan);
    	offScreenImg.fillOval((totalFD / totalMass) + 1024 / 2 - 5 / 2, (totalFY / totalMass) + 768 / 2 - 5 / 2, 5, 5);
    	
    	offScreenImg.setColor(Color.RED);
    	offScreenImg.fillOval(1024 / 2 - 5 / 2, 768 / 2 - 5 / 2, 5, 5);
    	
    	offScreenImg.setColor(Color.BLUE);
    	offScreenImg.fillOval(1024 / 2 - 5 / 2  + totalBFX / totalVolume, 768 / 2 - 5 / 2, 5, 5);
    	
    	if(hover != null) {
			offScreenImg.setColor(new Color(220,220,220,175));
			offScreenImg.fillRect(oldX, oldY, 200, 100);
			offScreenImg.setColor(Color.RED);
			offScreenImg.drawString(hover.name, oldX + 15, oldY + 15);
			offScreenImg.setColor(Color.BLUE);
			offScreenImg.drawString("X:      " + (hover.x - 1024 / 2)  + "    Y:      " + (hover.y - 768 / 2), oldX + 15, oldY + 30);
			offScreenImg.drawString("Weight: " + hover.weight + 		 "    Volume: " + hover.volume, oldX + 15, oldY + 45);
			offScreenImg.drawString("Width : " + hover.width + 			 "    Height: " + hover.height, oldX + 15, oldY + 60);
			offScreenImg.drawString("fBX :   " + hover.fBuoyancyXDiff +  "    fBY:    " + hover.fBuoyancyYDiff, oldX + 15, oldY + 75);

		}
    	
		offScreenImg.drawString("" + (oldX - 1024 / 2), 10, 10);
		offScreenImg.drawString("" + (oldY - 768 / 2), 10, 25);
        g.drawImage(offScreenImageDrawed, 0, 0, null);
        g.drawRect(1024 / 2 - 221, 768 / 2 - 50, 442, 100);
        System.out.println("Total Mass   : " + totalMass + "\tTotal Volume : " + totalVolume);
        
    }

	public void mousePressed(MouseEvent e) {	
		oldX = e.getX();
		oldY = e.getY();
		
		for(Rect r : rects)
    		if(r.x - r.width / 2 <= e.getX() && r.x + r.width / 2 >= e.getX() && r.y - r.height / 2 <= e.getY() && r.y + r.height / 2 >= e.getY()) {
    			selected = r;
    		}
	}

	public void mouseDragged(MouseEvent e) {
		if(selected != null) {
			selected.x -= oldX - e.getX();
			selected.y -= oldY - e.getY();;
		}
		oldX = e.getX();
		oldY = e.getY();
		
		this.paint(getGraphics());
	}
		
	public void mouseReleased(MouseEvent e) {
		if(selected != null) {
			switch (currentMode) {
			case 1: 
				selected.x = buffer + 1024 / 2;
				break;
			case 2:
				selected.y = buffer + 768 / 2;
				break;
			case 3:
				if(buffer != 0) {
					selected.weight = buffer;
				}

				break;
			}
			currentMode = 0;
		}
		this.paint(getGraphics());
		buffer = 0;
		
		selected = null;
		
		Rect h = null;
		Rect t = null;
		for(Rect rr : rects) {
			if(rr.name.equals("Head"))
				h = rr;
			if(rr.name.equals("Tail"))
				t = rr;
		}
		body.width = h.x - t.x - t.width / 2 - h.width / 2;
		body.x = (h.x - h.width / 2 + t.x + t.width / 2) / 2;

		body.y = 768 / 2;
		body.height = 10;
		body.weight = (int)((((h.x - h.width / 2.0f) - (t.x + t.width / 2.0f)) / 10.0f) * gramsPerCmBody);
		body.volume = (int)(3.14f * SHIP_r_mm * SHIP_r_mm * body.width / 1000f);

	}
	
	public void keyTyped(KeyEvent e) {
		if(selected != null) {
			switch (e.getKeyChar()) {
			case 'x':
				currentMode = 1;
				break;
			case 'y':
				currentMode = 2;
				break;
			case 'w':
				currentMode = 3;
				break;
				
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				buffer = Integer.parseInt(buffer + "" + e.getKeyChar());
				break;
							
			default:
				currentMode = 0;
			}
		}
		
		if(e.getKeyChar() == 'r') {

			int minX = 99999, minY = 9999, maxX = -9999, maxY = -9999;
			for(Rect r : rects) {
				if(r.x - r.width / 2 < minX) minX = r.x - r.width / 2;
				if(r.x + r.width / 2 > maxX) maxX = r.x + r.width / 2;
			}

			int cX = 1024 / 2 - (maxX + minX) / 2;
			//int cY = 1024 / 2 - minY + (maxY - minY) / 2;
			for(Rect r : rects) {
				r.x += cX;
			}
			body.x += cX;
			
			paint(getGraphics());
		}
		
		
	}
	
	public void mouseMoved(MouseEvent e) {
		oldX = e.getX();
		oldY = e.getY();

		boolean f = false;
		rects.add(body);
		for(Rect r : rects)
    		if(r.x - r.width / 2 <= e.getX() && r.x + r.width / 2 >= e.getX() && r.y - r.height / 2 <= e.getY() && r.y + r.height / 2 >= e.getY()) {
    			hover = r;
    			f = true;
    		}
		if(f == false)
			hover = null;
		rects.remove(body);

		paint(getGraphics());
	}

	public void windowClosing(WindowEvent e) {
		try {
			FileWriter wr = new FileWriter(new File("asd.txt"));
			for(String c : comments) {
				wr.write("#" + c + "\n");
			}
			for(Rect r : rects) {
				wr.write("R " + r.name + " " + r.x + " " + r.y + " " + r.width + " " + r.height + " " + r.weight + " " + r.volume + " " + r.fBuoyancyXDiff + " " + r.fBuoyancyYDiff + "\n");
			}
			wr.close();

		} catch (Exception e2) {
			e2.printStackTrace();
		}
		System.exit(-1);
	}
	
	public void windowClosed(WindowEvent e) {}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	public void mouseClicked(MouseEvent e) {}
	public void keyPressed(KeyEvent e) {}
	public void keyReleased(KeyEvent e) {}
	public void windowOpened(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowActivated(WindowEvent e) {}
	public void windowDeactivated(WindowEvent e) {}
	
	public static void main(String[] args) {       
		
        JFrame frame = new JFrame("COM Calculator");
        frame.setPreferredSize(new Dimension(SCREEN_WIDTH, SCREEN_HEIGHT));
        frame.setBackground(Color.white);
        Main m = new Main();
        frame.add(m);
        frame.addKeyListener(m);
        frame.addWindowListener(m);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        
    }
	
}
