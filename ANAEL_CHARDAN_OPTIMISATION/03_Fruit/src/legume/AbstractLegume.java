package legume;

public abstract class AbstractLegume implements Legume {
    abstract public double getPricePerUnit();

    public double getPrice(double piece) {
        return getPricePerUnit() * piece;
    }
}
