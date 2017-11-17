
public class Tremplin {

	public static void main(String[] args) {
		Cercle ce = new Cercle(2.0);
		Carre ca = new Carre(5.6);
		Rectangle r = new Rectangle(1.6, 4.3);
		
		System.out.println("Surface cercle : " + ce.getSurface());
		System.out.println("Surface carré : " + ca.getSurface());
		System.out.println("Surface rectangle : " + r.getSurface());

	}

}
