
public class Forme {
	protected double arrete, largeur, hauteur, rayon;
	
	public double getSurface() {
		if( this instanceof Carre ) { return arrete * arrete; }
		else if( this instanceof Cercle ) { return 2 * Math.PI * rayon; }
		else if( this instanceof Rectangle ) { return largeur * hauteur; }
		else return 0.d;
	}
	
}
