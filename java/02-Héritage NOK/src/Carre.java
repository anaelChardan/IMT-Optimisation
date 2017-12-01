
public class Carre implements Forme {
	private Rectangle rectangle;

	public Carre(double arrete) {
		this.rectangle = new Rectangle(arrete, arrete);
	}

	@Override
	public double getSurface() {
		return rectangle.getSurface();
	}
}
