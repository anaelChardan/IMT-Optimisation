package legume;

public class Carotte extends AbstractLegume implements Legume {
    @Override
    public double getPricePerUnit() {
        return 0.70;
    }

    @Override
    public String getName() {
        return "Carotte";
    }

    @Override
    public String getUnit() {
        return Unit.kg;
    }
}
