import legume.*;

public class Main {
    public static void main(String[] args) {
        Tomate tomate = new Tomate();
        PommeDeTerre pommeDeTerre = new PommeDeTerre();
        Avocat avocat = new Avocat();
        ChouxFleur chouxFleur = new ChouxFleur();
        Carotte carotte = new Carotte();

        double [] quantities = { 1.5, 5.1, 3.0, 1.0, 0.9 };
        Legume[] articles = { tomate , pommeDeTerre, avocat, chouxFleur, carotte };

        StringBuilder builder = new StringBuilder();

        for (int i = 0; i < quantities.length ; i++) {
            builder.setLength(0);
            System.out.println(getResult(builder, articles[i], quantities[i]));
        }
    }

    public static String getResult(StringBuilder builder, Legume legume, double quantity) {
        builder.append(legume.getName());
        builder.append(" : ");
        builder.append(quantity);
        builder.append(" x ");
        builder.append(legume.getPricePerUnit());
        builder.append(" €/");
        builder.append(legume.getUnit());
        builder.append(" = ");
        builder.append(legume.getPrice(quantity));
        builder.append("€");

        return builder.toString();
    }
}
