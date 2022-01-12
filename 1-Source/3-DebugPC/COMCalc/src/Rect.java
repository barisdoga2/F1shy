
public class Rect{
		public String name;
		public int x, y, width, height, weight, rand, volume, fBuoyancyXDiff, fBuoyancyYDiff, comXDiff, comYDiff;
		
		public Rect(String name, int x, int y, int width, int height, int weight, int volume, int fBuoyancyXDiff, int fBuoyancyYDiff, int comXDiff, int comYDiff){
			this.name = name;
			this.x = x;
			this.y = y;
			this.width = width;
			this.height = height;
			this.weight = weight;
			this.volume = volume;
			this.fBuoyancyXDiff = fBuoyancyXDiff;
			this.fBuoyancyYDiff = fBuoyancyYDiff;
			this.comXDiff = comXDiff;
			this.comYDiff = comYDiff;
			this.rand = 50;
		}
	}