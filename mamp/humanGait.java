import org.opensim.modeling.*;

public class HumanBodyModel {
    public static void main(String[] args) {
        try {
            Model model = new Model("fullbody.osim");
            State initialState = model.initSystem();

            double simulationDuration = 1.0;
            double timeStep = 0.01;

            ArrayDouble gaitData = new ArrayDouble();
            ArrayStr gaitLabels = new ArrayStr();

            for (double t = 0; t <= simulationDuration; t += timeStep) {
                initialState.setTime(t);
                model.setState(initialState);

                Vec3 pelvisPosition = model.getMarkerSet().get("pelvis").getLocationInGround(initialState);
                Vec3 pelvisVelocity = model.getMarkerSet().get("pelvis").getVelocityInGround(initialState);

                gaitData.append(pelvisPosition.get(0));
                gaitData.append(pelvisPosition.get(1));
                gaitData.append(pelvisPosition.get(2));
                gaitData.append(pelvisVelocity.get(0));
                gaitData.append(pelvisVelocity.get(1));
                gaitData.append(pelvisVelocity.get(2));

                gaitLabels.append("Pelvis_X");
                gaitLabels.append("Pelvis_Y");
                gaitLabels.append("Pelvis_Z");
                gaitLabels.append("Pelvis_Velocity_X");
                gaitLabels.append("Pelvis_Velocity_Y");
                gaitLabels.append("Pelvis_Velocity_Z");
            }

            System.out.println("Gait Data:");
            for (int i = 0; i < gaitData.getSize(); i++) {
                System.out.println(gaitLabels.getitem(i) + ": " + gaitData.getitem(i));
            }

            ArrayDoubleGrid gaitCycle = new ArrayDoubleGrid(gaitData, 6, gaitData.getSize() / 6, false);
            gaitCycle.setColumnLabels(gaitLabels);

            Plotter plotter = new Plotter();
            plotter.plot(gaitCycle);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
