package legume;

public class Avocat extends AbstractLegume implements Legume {
    @Override
    public double getPricePerUnit() {
        return 0.75;
    }

    @Override
    public String getName() {
        return "Avocat";
    }

    @Override
    public String getUnit() {
        return Unit.piece;
    }
}
