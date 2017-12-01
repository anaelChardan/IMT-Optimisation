package legume;

public class PommeDeTerre extends AbstractLegume implements Legume {
    @Override
    public double getPricePerUnit() {
        return 0.80;
    }

    @Override
    public String getName() {
        return "Pommes de terre";
    }

    @Override
    public String getUnit() {
        return Unit.kg;
    }
}
