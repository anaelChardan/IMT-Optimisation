
public class Cercle implements Forme {
	private final double rayon;

	public Cercle(double rayon) {
		this.rayon = rayon;
	}

	@Override
	public double getSurface() {
		return 2 * Math.PI * rayon;
	}
}
