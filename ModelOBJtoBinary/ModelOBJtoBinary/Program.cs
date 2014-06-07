using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ModelOBJtoBinary
{
    class Program
    {
        static List<float[]> positions = new List<float[]>();
        static List<float[]> normals = new List<float[]>();
        static List<float[]> uvs = new List<float[]>();
        static List<float[]> indices = new List<float[]>();

        static void Main(string[] args)
        {
            if (args.Length > 0)
            {
                Regex positionRegex = new Regex("v +(-?[0-9]*.[0-9]*) (-?[0-9]*.[0-9]*) (-?[0-9]*.[0-9]*)");
                Regex normalRegex = new Regex("vn +(-?[0-9]*.[0-9]*) (-?[0-9]*.[0-9]*) (-?[0-9]*.[0-9]*)");
                Regex textureRegex = new Regex("vt +(-?[0-9]*.[0-9]*) (-?[0-9]*.[0-9]*)");
                Regex quadRegex = new Regex("f +[0-9]*/[0-9]*/[0-9]* ([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*)");
                Regex faceRegex = new Regex("f +([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*)");

                StreamReader read = new StreamReader(args[0]);

                while (!read.EndOfStream)
                {
                    string line = read.ReadLine();
                    //Console.WriteLine(line);


                    MatchSet(vals =>
                    {
                        positions.Add(vals);
                    },
                    line, positionRegex);

                    MatchSet(vals =>
                    {
                        normals.Add(vals);
                    },
                    line, normalRegex);

                    MatchSet(vals =>
                    {
                        uvs.Add(vals);
                    },
                    line, textureRegex);

                    MatchSet(vals =>
                    {
                        indices.Add(vals);
                    },
                    line, quadRegex);

                    MatchSet(vals =>
                    {
                        indices.Add(vals);
                    },
                    line, faceRegex);

                }

                read.Close();

                PrintList("Positions", positions);
                PrintList("Normals", normals);
                PrintList("Textures", uvs);
                PrintList("Indices", indices);

                string end = args[0].Substring( 0, args[0].LastIndexOf('.') ) + ".mod";
                BinaryWriter write = new BinaryWriter(new FileStream(end, FileMode.Create));

                write.Write(indices.Count * 3);
                write.Write(indices.Count * 3);

                for (int i = 0; i < indices.Count; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        write.Write((short)(3 * i + j));
                    }
                }

                foreach (var face in indices)
                {
                    float[] color = { 1, 1, 1, 1 };
                    for (int i = 0; i < 3; i++)
                    {
                        Console.Write("Position ");
                        WriteArray(positions.ElementAt((int)face[i * 3] - 1), write);
                        Console.Write("Color ");
                        WriteArray(color, write);
                        Console.Write("Normal ");
                        WriteArray(normals.ElementAt((int)face[i * 3 + 2] - 1), write);
                        Console.Write("UV ");
                        WriteArray(uvs.ElementAt((int)face[i * 3 + 1] - 1), write);
                        Console.WriteLine();
                        //Console.Read();
                    }
                }

                write.Close();

                Console.WriteLine("Output can be found at " + Path.GetFullPath(end));
            }
        }

        private static void WriteArray(float[] arr, BinaryWriter write)
        {
            foreach (float num in arr)
            {
                Console.Write(num + " ");
                write.Write(num);
            }
        }

        private static void WriteArrayAsInt(float[] arr, BinaryWriter write)
        {
            foreach (float num in arr)
            {
                write.Write((int)num);
            }
        }

        private static void PrintList(string message, List<float[]> list)
        {
            Console.WriteLine(message);
            Console.WriteLine("_____________");
            foreach (var array in list)
            {
                foreach (var num in array)
                {
                    Console.Write( num + " " );
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }

        static void MatchSet(Action<float[]> func, string str, Regex reg)
        {
            Match match = reg.Match(str);
            if (match.Success)
            {
                float[] vals = new float[ match.Groups.Count - 1 ];

                int i = -1;
                foreach( Group group in match.Groups )
                {
                    if ( i >= 0 )
                    {
                        vals[i] = float.Parse( group.ToString() );
                    }
                    i++;
                }

                func.Invoke(vals);
            }
        }
    }
}
