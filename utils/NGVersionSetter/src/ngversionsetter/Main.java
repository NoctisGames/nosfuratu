package ngversionsetter;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.JOptionPane;

/**
 *
 * @author sgowen
 */
public final class Main
{
    private static final Pattern EXTRACT_MAJOR_MINOR_BUILD = Pattern.compile("^([0-9]++)\\.([0-9]++)\\.([0-9]++)$");

    // GameConstants.h
    private static final String EXTRACT_MAJOR = "#define\\sVERSION_MAJOR\\s([0-9]++)";
    private static final String EXTRACT_MINOR = "#define\\sVERSION_MINOR\\s([0-9]++)";
    private static final String EXTRACT_BUILD = "#define\\sVERSION_BUILD\\s([0-9]++)";

    // build.gradle
    private static final String EXTRACT_VERSION_NAME = "versionName\\s\"(.*?)\"";
    private static final String EXTRACT_VERSION_CODE = "versionCode\\s([0-9]++)";

    // Info.plist
    private static final String EXTRACT_VERSION_ELEMENT = "<string>(.*?)</string>";

    // Package.appxmanifest
    private static final String EXTRACT_VERSION_ATTRIBUTE = "\\sVersion=\"(.*?)\"\\s/>";

    private static boolean OPERATING_FROM_COMMAND_LINE;

    /**
     * @param args the command line arguments
     */
    public static void main(String args[])
    {
        try
        {
            OPERATING_FROM_COMMAND_LINE = args != null && args.length > 0;
            String pathToRepository;
            String version;
            boolean isSupportingWindows81 = false;
            if (OPERATING_FROM_COMMAND_LINE)
            {
                pathToRepository = args[0];
                version = args[1];
                if (args.length >= 3)
                {
                    isSupportingWindows81 = Boolean.parseBoolean(args[2]);
                }
            } else
            {
                pathToRepository = JOptionPane.showInputDialog("Enter path to the game repository:");
                version = JOptionPane.showInputDialog("Enter the new version (in the form MAJOR.MINOR.REVISION)");
            }

            File folder = new File(pathToRepository);

            handleGameConstantsHeader(new File(folder, "src/core/game/logic/GameConstants.h"), version);
            handleBuildGradleContents(new File(folder, "src/platform/android/app/build.gradle"), version);
            handleInfoPlistContents(new File(folder, "src/platform/apple/ios/Info.plist"), version, false);
            handleInfoPlistContents(new File(folder, "src/platform/apple/mac/Info.plist"), version, true);
            if (isSupportingWindows81)
            {
                handleAppxmanifestContents(new File(folder, "src/platform/windows/8.1/NosFURatu/NosFURatu.Windows/Package.appxmanifest"), version, false);
                handleAppxmanifestContents(new File(folder, "src/platform/windows/8.1/NosFURatu/NosFURatu.WindowsPhone/Package.appxmanifest"), version, false);
            }
            handleAppxmanifestContents(new File(folder, "src/platform/windows/uwp/Package.appxmanifest"), version, isSupportingWindows81);
        } catch (IOException e)
        {
            logException(e);
        }
    }

    private static void handleGameConstantsHeader(File file, final String version) throws IOException
    {
        final String major = findValue(version, EXTRACT_MAJOR_MINOR_BUILD, 1);
        final String minor = findValue(version, EXTRACT_MAJOR_MINOR_BUILD, 2);
        final String build = findValue(version, EXTRACT_MAJOR_MINOR_BUILD, 3);

        readAndWriteFileWithLineByLineHandler(file, new LineHandler()
        {
            @Override
            public String onLineRead(String line)
            {
                line = line.replaceAll(EXTRACT_MAJOR, "#define VERSION_MAJOR " + major);
                line = line.replaceAll(EXTRACT_MINOR, "#define VERSION_MINOR " + minor);
                line = line.replaceAll(EXTRACT_BUILD, "#define VERSION_BUILD " + build);

                return line;
            }
        });
    }

    private static void handleBuildGradleContents(File file, final String version) throws IOException
    {
        readAndWriteFileWithLineByLineHandler(file, new LineHandler()
        {
            @Override
            public String onLineRead(String line)
            {
                line = line.replaceAll(EXTRACT_VERSION_NAME, "versionName \"" + version + "\"");
                
                if (line.contains("versionCode"))
                {
                    final String versionCodeString = findValue(line, Pattern.compile(EXTRACT_VERSION_CODE), 1);
                    int versionCode = Integer.parseInt(versionCodeString);
                    final String bumpedVersionCode = String.valueOf(++versionCode);

                    line = line.replaceAll(EXTRACT_VERSION_CODE, "versionCode " + bumpedVersionCode);
                }

                return line;
            }
        });
    }

    private static void handleInfoPlistContents(File file, final String version, final boolean isMac) throws IOException
    {
        readAndWriteFileWithLineByLineHandler(file, new LineHandler()
        {
            boolean _containsCFBundleShortVersionString = false;
            boolean _containsCFBundleVersionString = false;

            @Override
            public String onLineRead(String line)
            {
                if (_containsCFBundleShortVersionString)
                {
                    line = line.replaceAll(EXTRACT_VERSION_ELEMENT, "<string>" + version + "</string>");
                }

                if (_containsCFBundleVersionString && isMac)
                {
                    final String versionNumberString = findValue(line, Pattern.compile(EXTRACT_VERSION_ELEMENT), 1);
                    int versionNumber = Integer.parseInt(versionNumberString);
                    final String bumpedVersionNumber = String.valueOf(++versionNumber);

                    line = line.replaceAll(EXTRACT_VERSION_ELEMENT, "<string>" + bumpedVersionNumber + "</string>");
                }

                _containsCFBundleShortVersionString = line.contains("CFBundleShortVersionString");
                _containsCFBundleVersionString = line.contains("CFBundleVersion");

                return line;
            }
        });
    }

    private static void handleAppxmanifestContents(File file, final String version, boolean bumpBuildNum) throws IOException
    {
        final String finalVersion;
        if (bumpBuildNum)
        {
            final String major = findValue(version, EXTRACT_MAJOR_MINOR_BUILD, 1);
            final String minor = findValue(version, EXTRACT_MAJOR_MINOR_BUILD, 2);
            final String build = findValue(version, EXTRACT_MAJOR_MINOR_BUILD, 3);

            int buildNum = Integer.parseInt(build);
            final String bumpedBuildNum = String.valueOf(++buildNum);

            finalVersion = major + "." + minor + "." + bumpedBuildNum + ".0";
        } else
        {
            finalVersion = version + ".0";
        }

        readAndWriteFileWithLineByLineHandler(file, new LineHandler()
        {
            @Override
            public String onLineRead(String line)
            {
                line = line.replaceAll(EXTRACT_VERSION_ATTRIBUTE, " Version=\"" + finalVersion + "\" />");

                return line;
            }
        });
    }

    private interface LineHandler
    {
        String onLineRead(String line);
    }

    private static void readAndWriteFileWithLineByLineHandler(File file, LineHandler lineHandler) throws FileNotFoundException, IOException
    {
        final String tempFileName = "tmp.dat";

        BufferedReader br = null;
        BufferedWriter bw = null;
        try
        {
            br = new BufferedReader(new FileReader(file));
            bw = new BufferedWriter(new FileWriter(tempFileName));
            String line;
            while ((line = br.readLine()) != null)
            {
                line = lineHandler.onLineRead(line);
                bw.write(line + "\n");
            }
        } finally
        {
            try
            {
                if (br != null)
                {
                    br.close();
                }
            } catch (IOException e)
            {
                logException(e);
            }
            try
            {
                if (bw != null)
                {
                    bw.close();
                }
            } catch (IOException e)
            {
                logException(e);
            }
        }

        file.delete();

        File newFile = new File(tempFileName);
        newFile.renameTo(file.getAbsoluteFile());
    }

    private static String findValue(String text, Pattern pattern, int groupIndex)
    {
        Matcher matcher = pattern.matcher(text);
        String value;
        if (matcher.find())
        {
            if (matcher.groupCount() > 0)
            {
                value = matcher.group(groupIndex);
                return value.length() > 0 ? value : "";
            }
        }

        return "";
    }

    private static void logException(Exception e)
    {
        System.err.println(e.toString());

        if (!OPERATING_FROM_COMMAND_LINE)
        {
            JOptionPane.showMessageDialog(null, e.toString());
        }
    }
}
