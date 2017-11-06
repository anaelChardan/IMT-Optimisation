
public class Etudiant {
	String nom = "";
	String nomBinome = "";
	String sport = "";

	public Etudiant(String nom) {
		this.nom = nom;
	}

	public void commenceSport(String nomSport) {
		sport = nomSport;
	}

	public void arreteSport() {
		sport = "";
	}

	public Boolean creerBinome(Etudiant e1, Etudiant e2) {
		Boolean reussi = false;

		if( e1 != null && e2 != null ) {
			if( (e1.nomBinome == null || e1.nomBinome.equals("")) &&
					(e2.nomBinome == null || e2.nomBinome.equals("")) ) {
				// les deux étudiants ne sont pas déjà en binôme
				if( e1.sport == null || e1.sport.equals("") ) {
					// e1 ne fait pas de sport, e2 ne doit pas en faire non plus
					if( e2.sport == null || e2.sport.equals("") ) {
						reussi = true;
					}
				} else {
					// e1 fait un sport, il faut que e2 aussi
					if( e2.sport != null && !e2.sport.equals("") ) {
						reussi = true;
					}
				}
				if( reussi ) {
					e1.nomBinome = e2.nom;
					e2.nomBinome = e1.nom;
				}
			} else {
				if( e1.nomBinome != null && e2.nomBinome != null ) {
					// déjà en binôme
					reussi = e1.nomBinome.equals(e2.nomBinome);
				}
			}
		}

		return reussi;
	}

	public Boolean detruireBinome(Etudiant e1, Etudiant e2) {
		Boolean reussi = false;

		if( e1 != null && e2 != null ) {
			if( (e1.nomBinome != null || e1.nomBinome.equals(e2.nom)) &&
					(e2.nomBinome != null || e2.nomBinome.equals(e1.nom)) ) {
				// les deux étudiants sont bien en binôme
				e1.nomBinome = "";
				e2.nomBinome = "";
				reussi = true;
			}
		}

		return reussi;
	}
}
