package readlevels;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import static java.util.Collections.sort;
import java.util.Comparator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.JOptionPane;

/**
 *
 * @author sgowen
 */
public final class Main
{
    private static final Pattern EXTRACT_CHAPTER_AND_LEVEL_REGEX = Pattern.compile(".*?_c([0-9]++)_l([0-9]++).json");

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
            if (OPERATING_FROM_COMMAND_LINE)
            {
                pathToRepository = args[0];
            } else
            {
                pathToRepository = JOptionPane.showInputDialog("Enter path to the \"nosfuratu-levels\" repository:");
            }

            File folder = new File(pathToRepository);
            File[] listOfFilesArray = folder.listFiles(new FilenameFilter()
            {
                @Override
                public boolean accept(File dir, String name)
                {
                    return name.endsWith(".json");
                }
            });

            List<File> listOfFiles = Arrays.asList(listOfFilesArray);

            Collections.sort(listOfFiles, new Comparator<File>()
            {
                @Override
                public int compare(File o1, File o2)
                {
                    String o1level = findValue(o1.getName(), EXTRACT_CHAPTER_AND_LEVEL_REGEX, 2);
                    String o2level = findValue(o2.getName(), EXTRACT_CHAPTER_AND_LEVEL_REGEX, 2);

                    Integer o1levelInt = Integer.parseInt(o1level);
                    Integer o2levelInt = Integer.parseInt(o2level);

                    return o1levelInt.compareTo(o2levelInt);
                }
            });

            String levelsPrefix = "#include <sstream>\n\n";
            String prefix = "";
            prefix += "/// Chapter %1$s Level %2$s ///\n";
            prefix += "\n";
            prefix += "Chapter%1$sLevel%2$s * Chapter%1$sLevel%2$s::s_pInstance = nullptr;\n";
            prefix += "\n";
            prefix += "void Chapter%1$sLevel%2$s::create()\n";
            prefix += "{\n";
            prefix += "    assert(!s_pInstance);\n";
            prefix += "\n";
            prefix += "    std::stringstream ss;\n";
            prefix += "\n";

            String suffix = "";
            suffix += "    static std::string text = ss.str();\n";
            suffix += "\n";
            suffix += "    s_pInstance = new Chapter%1$sLevel%2$s(text.c_str());\n";
            suffix += "}\n";
            suffix += "\n";
            suffix += "void Chapter%1$sLevel%2$s::destroy()\n";
            suffix += "{\n";
            suffix += "    assert(s_pInstance);\n";
            suffix += "\n";
            suffix += "delete s_pInstance;\n";
            suffix += "s_pInstance = nullptr;\n";
            suffix += "}\n";
            suffix += "\n";
            suffix += "Chapter%1$sLevel%2$s * Chapter%1$sLevel%2$s::getInstance()\n";
            suffix += "{\n";
            suffix += "    return s_pInstance;\n";
            suffix += "}\n";
            suffix += "\n";
            suffix += "Chapter%1$sLevel%2$s::Chapter%1$sLevel%2$s(const char* json) : Level(json)\n";
            suffix += "{\n";
            suffix += "    // Empty\n";
            suffix += "}\n";
            suffix += "\n";

            StringBuilder mainSb = new StringBuilder();
            StringBuilder c10Sb = new StringBuilder();
            StringBuilder c21Sb = new StringBuilder();
            
            mainSb.append(levelsPrefix);
            c10Sb.append(levelsPrefix);
            c21Sb.append(levelsPrefix);

            for (File file : listOfFiles)
            {
                String chapter = findValue(file.getName(), EXTRACT_CHAPTER_AND_LEVEL_REGEX, 1);
                String level = findValue(file.getName(), EXTRACT_CHAPTER_AND_LEVEL_REGEX, 2);

                String levelPrefix = String.format(prefix, chapter, level);
                String levelSuffix = String.format(suffix, chapter, level);

                // Open the file
                FileInputStream fis = new FileInputStream(file);
                BufferedReader br = new BufferedReader(new InputStreamReader(fis));

                //Read File Line By Line
                String line;
                while ((line = br.readLine()) != null)
                {
                    line = line.replace("\"", "\\\"");

                    if (level.equals("10"))
                    {
                        handleLine(c10Sb, levelPrefix, line, levelSuffix);
                    } else if (level.equals("21"))
                    {
                        handleLine(c21Sb, levelPrefix, line, levelSuffix);
                    } else
                    {
                        handleLine(mainSb, levelPrefix, line, levelSuffix);
                    }
                }

                //Close the input stream
                br.close();
            }
            
            String main = mainSb.toString().replace("\\\" << \"\"", "\\\"\" << \"");
            String chapter_10 = c10Sb.toString().replace("\\\" << \"\"", "\\\"\" << \"");
            String chapter_21 = c21Sb.toString().replace("\\\" << \"\"", "\\\"\" << \"");

            writeCodeToFile(folder, main, "main");
            writeCodeToFile(folder, chapter_10, "chapter_10");
            writeCodeToFile(folder, chapter_21, "chapter_21");
        } catch (IOException e)
        {
            logException(e);
        }
    }

    private static void handleLine(StringBuilder sb, String levelPrefix, String line, String levelSuffix)
    {
        String entriesPrefix = "    ss";
        String entryPrefix = " << \"";
        String entrySuffix = "\"";
        String entriesSuffix = ";\n\n";
            
        sb.append(levelPrefix);
        sb.append(entriesPrefix);
        if (line.length() > 8000)
        {
            for (int start = 0; start < line.length(); start += 8000)
            {
                sb.append(entryPrefix);
                
                if (start + 8000 > line.length())
                {
                    sb.append(line.substring(start));
                } else
                {
                    sb.append(line.substring(start, start + 8000));
                }
                
                sb.append(entrySuffix);
            }
        }
        else
        {
            sb.append(entryPrefix);
            sb.append(line);
            sb.append(entrySuffix);
        }

        sb.append(entriesSuffix);
        sb.append(levelSuffix);
    }

    private static boolean writeCodeToFile(File repositoryFolder, String cPlusPlusCode, String prefix) throws IOException
    {
        File file = new File(repositoryFolder, prefix + "_code_to_paste_into_GameScreenLevels.cpp.txt");

        boolean isWriteSuccessful = false;
        BufferedWriter writer = null;
        try
        {
            writer = new BufferedWriter(new FileWriter(file));
            writer.write(cPlusPlusCode);

            isWriteSuccessful = true;
        } catch (IOException e)
        {
            logException(e);
        } finally
        {
            try
            {
                if (writer != null)
                {
                    writer.close();
                }
            } catch (IOException e)
            {
                logException(e);
            }
        }

        if (isWriteSuccessful)
        {
            String cmd = "open /Applications/TextEdit.app " + file.getAbsolutePath();
            Runtime.getRuntime().exec(cmd);
        }

        return isWriteSuccessful;
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