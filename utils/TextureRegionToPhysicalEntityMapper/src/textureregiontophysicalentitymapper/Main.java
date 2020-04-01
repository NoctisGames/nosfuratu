package textureregiontophysicalentitymapper;

/**
 *
 * @author sgowen
 */
public class Main
{
    private static final double PIXEL_WIDTH_FOR_GAME = 1808.0d;
    private static final double PIXEL_HEIGHT_FOR_GAME = 4096.0d;

    private static final double CAM_WIDTH = 15.890625d;
    private static final double CAM_HEIGHT = 9.0d;
    private static final double GAME_HEIGHT = 36.0d;

    private static final int MODE_USE_CAM_WIDTH_PLACE_AT_TOP_OF_GAME_HEIGHT = 0;
    private static final int MODE_USE_CAM_WIDTH_PLACE_AT_BOTTOM_OF_GAME_HEIGHT = 1;
    private static final int MODE_USE_CAM_WIDTH_PLACE_AT_Y_OF_GAME_HEIGHT = 2;
    private static final int MODE_USE_CAM_WIDTH_AND_GAME_HEIGHT = 3;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        if (args != null && args.length >= 1)
        {
            int mode = Integer.parseInt(args[0]);
            switch (mode)
            {
                case MODE_USE_CAM_WIDTH_PLACE_AT_TOP_OF_GAME_HEIGHT:
                    handleMODE_USE_CAM_WIDTH_PLACE_AT_TOP_OF_GAME_HEIGHT(args);
                    break;
                case MODE_USE_CAM_WIDTH_PLACE_AT_BOTTOM_OF_GAME_HEIGHT:
                    handleMODE_USE_CAM_WIDTH_PLACE_AT_BOTTOM_OF_GAME_HEIGHT(args);
                    break;
                case MODE_USE_CAM_WIDTH_PLACE_AT_Y_OF_GAME_HEIGHT:
                    handleMODE_USE_CAM_WIDTH_PLACE_AT_Y_OF_GAME_HEIGHT(args);
                    break;
                case MODE_USE_CAM_WIDTH_AND_GAME_HEIGHT:
                    handleMODE_USE_CAM_WIDTH_AND_GAME_HEIGHT(args);
                    break;
                default:
                    printUsage();
                    break;
            }
        } else
        {
            printUsage();
        }
    }

    private static void printUsage()
    {
        System.out.println("Args: <int mode> <additional params as per mode>");
        System.out.println("Args For mode 0: <double pixelHeight>");
        System.out.println("Args For mode 1: <double pixelHeight>");
        System.out.println("Args For mode 2: <double pixelHeight> <double pixelTopY>");
        System.out.println("Args For mode 3: <double pixelWidth> <double pixelHeight>");
    }

    private static void printPosition(double x, double y, double width, double height)
    {
        System.out.println("{x, y, width, height}");
        System.out.println("" + x + "f, " + y + "f, " + width + "f, " + height + "f");
    }

    private static void printSize(double width, double height)
    {
        System.out.println("{width, height}");
        System.out.println("" + width + "f, " + height + "f");
    }

    private static void handleMODE_USE_CAM_WIDTH_PLACE_AT_TOP_OF_GAME_HEIGHT(String[] args)
    {
        // Top
        if (args.length == 2)
        {
            double pixelHeight = Double.parseDouble(args[1]);
            double height = pixelHeight / PIXEL_HEIGHT_FOR_GAME * GAME_HEIGHT;
            double halfHeight = height / 2;
            double x = CAM_WIDTH / 2;
            double y = GAME_HEIGHT - halfHeight;
            double width = CAM_WIDTH;

            printPosition(x, y, width, height);
        }
    }

    private static void handleMODE_USE_CAM_WIDTH_PLACE_AT_BOTTOM_OF_GAME_HEIGHT(String[] args)
    {
        // Bottom
        if (args.length == 2)
        {
            double pixelHeight = Double.parseDouble(args[1]);
            double height = pixelHeight / PIXEL_HEIGHT_FOR_GAME * GAME_HEIGHT;
            double halfHeight = height / 2;
            double x = CAM_WIDTH / 2;
            double y = halfHeight;
            double width = CAM_WIDTH;

            printPosition(x, y, width, height);
        }
    }

    private static void handleMODE_USE_CAM_WIDTH_PLACE_AT_Y_OF_GAME_HEIGHT(String[] args)
    {
        if (args.length == 3)
        {
            double pixelHeight = Double.parseDouble(args[1]);
            double pixelTopY = Double.parseDouble(args[2]);
            double height = pixelHeight / PIXEL_HEIGHT_FOR_GAME * GAME_HEIGHT;
            double halfHeight = height / 2;
            double x = CAM_WIDTH / 2;
            double width = CAM_WIDTH;
            double y = (pixelTopY / PIXEL_HEIGHT_FOR_GAME * GAME_HEIGHT) + halfHeight;

            printPosition(x, GAME_HEIGHT - y, width, height);
        }
    }

    private static void handleMODE_USE_CAM_WIDTH_AND_GAME_HEIGHT(String[] args)
    {
        if (args.length == 3)
        {
            double pixelWidth = Double.parseDouble(args[1]);
            double pixelHeight = Double.parseDouble(args[2]);

            double width = pixelWidth / PIXEL_WIDTH_FOR_GAME * CAM_WIDTH;
            double height = pixelHeight / PIXEL_HEIGHT_FOR_GAME * GAME_HEIGHT;

            printSize(width, height);
        }
    }
}