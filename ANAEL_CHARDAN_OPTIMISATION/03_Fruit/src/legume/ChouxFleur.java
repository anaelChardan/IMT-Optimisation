package legume;

public class ChouxFleur extends AbstractLegume implements Legume {
    @Override
    public double getPricePerUnit() {
        return 1.70;
    }

    @Override
    public String getName() {
        return "Choux fleur";
    }

    @Override
    public String getUnit() {
        return Unit.piece;
    }
}
