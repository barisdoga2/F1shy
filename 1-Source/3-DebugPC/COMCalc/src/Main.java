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
	
	public static final int SCREEN_WIDTH = 1640;
	public static final int SCREEN_HEIGHT = 768;
	
	private static final int waterDensity = 1;

	
	
	private ArrayList<String> comments;
	private ArrayList<Rect> rects;
	
	private Image offScreenImageDrawed = null;
    private Graphics offScreenImg = null;  

	private Rect selected, hover;
	private int oldX, oldY;
    private int currentMode = 0;
    private int buffer = 0;
    
    private int totalGraX = 0, totalMass = 0, totalVolume = 0, totalGraY = 0, totalBuoX = 0;

	public Main() {
		setBackground(Color.white);
			    
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		rects = new ArrayList<Rect>();
		comments = new ArrayList<String>();
		
		// Read Part Info
		try {
			BufferedReader br = new BufferedReader(new FileReader("C:\\Users\\Baris\\Desktop\\Basliksiz2.csv"));

            String line;
            while ((line = br.readLine()) != null) {
            	if(line.startsWith("#")) {
            		comments.add(line.substring(1));
            	}else if(line.startsWith(",")) {
            		String[] tokens = line.split(",");
            		rects.add(new Rect(tokens[2], Integer.parseInt(tokens[3]), Integer.parseInt(tokens[4]), Integer.parseInt(tokens[5]), Integer.parseInt(tokens[6]),
            						   Integer.parseInt(tokens[7]), Integer.parseInt(tokens[8]), Integer.parseInt(tokens[9]), Integer.parseInt(tokens[10]),
            						   Integer.parseInt(tokens[11]), Integer.parseInt(tokens[12])));
            	}
            }
            br.close();
		} catch (Exception e1) {
			e1.printStackTrace();
		}
	}

	public void paint(Graphics g) {
        Dimension d = getSize();
        if (offScreenImageDrawed == null) 
            offScreenImageDrawed = createImage(d.width, d.height);   
        
        offScreenImg = offScreenImageDrawed.getGraphics();      
        offScreenImg.setColor(new Color(82, 82, 122));
        offScreenImg.fillRect(0, 0, d.width, d.height);
        totalGraX = 0;
        totalGraY = 0;
        totalBuoX = 0;
        totalMass = 0;
        totalVolume = 0;
        
    	for(Rect r : rects) {
    		offScreenImg.setColor(Color.BLACK);
    		offScreenImg.drawRect(r.x - r.width, r.y - r.height, r.width * 2, r.height * 2);
    		offScreenImg.setColor(new Color(255,100,30,100));
    		offScreenImg.drawString(r.name, (int) (r.x - g.getFontMetrics().getStringBounds(r.name, g).getWidth() / 2), r.y + (int)(g.getFontMetrics().getStringBounds(r.name, g).getHeight() / 3));
    		
        	// Draw Center
        	offScreenImg.setColor(Color.RED);
        	offScreenImg.fillOval(r.x - 4 / 2, r.y - 4 / 2, 4, 4);
        	
    		// Draw CoB
        	if(r.volume != 0) {
        		offScreenImg.setColor(Color.BLUE);
            	offScreenImg.fillOval(r.x + r.fBuoyancyXDiff - 4 / 2, r.y + r.fBuoyancyYDiff - 4 / 2, 4, 4);
        	}
        	
        	// Draw CoM
        	offScreenImg.setColor(Color.CYAN);
        	offScreenImg.fillOval(r.x + r.comXDiff - 4 / 2, r.y + r.comYDiff - 4 / 2, 4, 4);
        	
        	// Maths
    		totalGraX += (r.x - SCREEN_WIDTH / 2 + r.comXDiff) * r.weight;
    		totalGraY += (r.y - SCREEN_HEIGHT / 2 + r.comYDiff) * r.weight;
    		totalBuoX += (r.x - SCREEN_WIDTH / 2 + r.fBuoyancyXDiff) * r.volume * waterDensity;
    		totalVolume += r.volume;
    		totalMass += r.weight;
    		
    	}
    	
    	// Draw CoM
    	offScreenImg.setColor(Color.CYAN);
    	offScreenImg.fillOval((totalGraX / totalMass) + SCREEN_WIDTH / 2 - 8 / 2, (totalGraY / totalMass) + SCREEN_HEIGHT / 2 - 8 / 2, 8, 8);
    	
    	// Draw Screen Center
    	offScreenImg.setColor(Color.RED);
    	offScreenImg.fillOval(SCREEN_WIDTH / 2 - 8 / 2, SCREEN_HEIGHT / 2 - 8 / 2, 8, 8);
    	
    	// Draw Buoyancy Center
    	offScreenImg.setColor(Color.BLUE);
    	offScreenImg.fillOval(SCREEN_WIDTH / 2 - 8 / 2  + totalBuoX / totalVolume, SCREEN_HEIGHT / 2 - 8 / 2, 8, 8);
    	
    	// Draw Hovered Object Info
    	if(hover != null) {
			offScreenImg.setColor(new Color(220,220,220,175));
			offScreenImg.fillRect(oldX, oldY, 200, 100);
			offScreenImg.setColor(Color.RED);
			offScreenImg.drawString(hover.name, oldX + 15, oldY + 15);
			offScreenImg.setColor(Color.BLUE);
			offScreenImg.drawString("X:      " + (hover.x - SCREEN_WIDTH / 2)  + "    Y:      " + (hover.y - SCREEN_HEIGHT / 2), oldX + 15, oldY + 30);
			offScreenImg.drawString("Weight: " + hover.weight + 		 "    Volume: " + hover.volume, oldX + 15, oldY + 45);
			offScreenImg.drawString("Width : " + hover.width + 			 "    Height: " + hover.height, oldX + 15, oldY + 60);
			offScreenImg.drawString("fBX :   " + hover.fBuoyancyXDiff +  "    fBY:    " + hover.fBuoyancyYDiff, oldX + 15, oldY + 75);

		}
    	
    	offScreenImg.setColor(Color.RED);
		offScreenImg.drawString("" + (oldX - SCREEN_WIDTH / 2), 10, 10);
		offScreenImg.drawString("" + (oldY - SCREEN_HEIGHT / 2), 10, 25);
		offScreenImg.drawString("Total Mass   : " + totalMass, 10, 40);
		offScreenImg.drawString("Total Volume : " + totalVolume, 10, 55);
		offScreenImg.drawString("CoM : " + ((float)totalGraX / totalMass) + "," + (totalGraY / totalMass), 10, 70);
		offScreenImg.drawString("CoB : " + ((float)totalBuoX / totalVolume) + "," + (0), 10, 85);
        g.drawImage(offScreenImageDrawed, 0, 0, null);
        
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
				selected.x = buffer + SCREEN_WIDTH / 2;
				break;
			case 2:
				selected.y = buffer + SCREEN_HEIGHT / 2;
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
				System.out.println("buffering");
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

			int cX = SCREEN_WIDTH / 2 - (maxX + minX) / 2;
			for(Rect r : rects) {
				r.x += cX;
			}
			
			paint(getGraphics());
		}
		
		
	}
	
	public void mouseMoved(MouseEvent e) {
		oldX = e.getX();
		oldY = e.getY();

		boolean f = false;
		for(Rect r : rects)
    		if(r.x - r.width / 2 <= e.getX() && r.x + r.width / 2 >= e.getX() && r.y - r.height / 2 <= e.getY() && r.y + r.height / 2 >= e.getY()) {
    			hover = r;
    			f = true;
    		}
		if(f == false)
			hover = null;
		paint(getGraphics());
	}

	public void windowClosing(WindowEvent e) {
		try {
			FileWriter wr = new FileWriter(new File("C:\\Users\\Baris\\Desktop\\Basliksiz22.csv"));
			for(String c : comments) {
				wr.write("#" + c + "\n");
			}
			for(Rect r : rects) {
				wr.write(",R," + r.name + "," + r.x + "," + r.y + "," + r.width + "," + r.height + "," + r.weight + "," + r.volume + "," + r.fBuoyancyXDiff + "," + r.fBuoyancyYDiff + "," + r.comXDiff + "," + r.comYDiff + "\n");
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
