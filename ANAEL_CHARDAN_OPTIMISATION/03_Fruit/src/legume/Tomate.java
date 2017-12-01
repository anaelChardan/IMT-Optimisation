package legume;

public class Tomate extends AbstractLegume implements Legume {
    @Override
    public double getPricePerUnit() {
        return 1.10;
    }

    @Override
    public String getName() {
        return "Tomate";
    }

    @Override
    public String getUnit() {
        return Unit.kg;
    }
}
