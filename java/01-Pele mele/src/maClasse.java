
public class maClasse {
	
	// la méthode qui suit ne change pas l'état
	// de l'instance qui la traite
	public int incrementer(int x) {
		return x + 1;
	}
	
	// c'est donc une méthode de classe
	public static int sIncrementer(int x) {
		return x + 1;
	}
	
	
	// la méthode suivante sature la mémoire inutilement
	public static void brouillerNOK() {
		long counter = 99999999, resultat = 0;
		while( counter-- != 0 ) {
			maClasse a = new maClasse();
			a.initialiserEtat(resultat);
			resultat += a.faireUnTourDeMagie(resultat);
		}
	}

	// celle-ci non car a peut être réutilisé même si
	// son état est modifié par faireUnTourDeMagie
	public static void brouillerOK() {
		long counter = 99999999, resultat = 0;
		maClasse a = new maClasse();
		while( counter-- != 0 ) {
			a.initialiserEtat(resultat);
			resultat += a.faireUnTourDeMagie(resultat);
		}
	}

	
	// il faut factoriser les expressions autant que possible
	public static long mauvaisExempleFactorisation(long v) {
		maClasse a = new maClasse();
		long x = a.faireUnTourDeMagie(v) * 89;
		long y = a.faireUnTourDeMagie(v) * 152;
		return (x + y) / 2;
	}
	
	public static long bonExempleFactorisation(long v) {
		maClasse a = new maClasse();
		long temp = a.faireUnTourDeMagie(v);
		long x = temp * 89;
		long y = temp * 152;
		return (x + y) / 2;
	} // note : d'autant plus vrai dans une boucle
	

	
	// méthodes fantasques qui permettent de compiler
	private void initialiserEtat(long valeur) {
		unMembredInstance = valeur;
	}

	private long faireUnTourDeMagie(long v) {
		return v * unMembredInstance + v / 10;
	}
	
	private long unMembredInstance = 15;
	
}
