
public class Rectangle implements Forme {

    private final double hauteur;
    private final double largeur;

    public Rectangle(double l, double h) {
		largeur = l;
		hauteur = h;
	}

    @Override
    public double getSurface() {
        return hauteur * largeur;
    }
}
