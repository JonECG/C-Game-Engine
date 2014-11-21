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
        static List<float[]> tangents = new List<float[]>();
        static List<float[]> uvs = new List<float[]>();
        static List<float[]> indices = new List<float[]>();

        static List<float[][]> vertices = new List<float[][]>();
        static List<int[]> actualIndices = new List<int[]>();

        static void Main(string[] args)
        {
            if (args.Length > 1)
            {
                string inVal = "-?[0-9]*.[0-9]*(?:e-?[0-9]*)?";
                Regex positionRegex = new Regex("v +(" + inVal + ") (" + inVal + ") (" + inVal + ")");
                Regex normalRegex = new Regex("vn +(" + inVal + ") (" + inVal + ") (" + inVal + ")");
                Regex textureRegex = new Regex("vt +(" + inVal + ") (" + inVal + ")");
                Regex quadRegex = new Regex("f +[0-9]*/[0-9]*/[0-9]* ([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*)");
                Regex faceRegex = new Regex("f +([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*) ([0-9]*)/([0-9]*)/([0-9]*)");

                StreamReader read = new StreamReader(args[0]);

                while (!read.EndOfStream)
                {
                    string line = read.ReadLine();
                    //Console.WriteLine(line);


                    if (!MatchSet(vals =>
                    {
                        positions.Add(vals);
                    },
                    line, positionRegex))
                    {
                        int i = 1;
                    }

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

                //PrintList("Positions", positions);
                //PrintList("Normals", normals);
                //PrintList("Tangents", tangents);
                //PrintList("Textures", uvs);
                //PrintList("Indices", indices);

                string end = args[1];// args[0].Substring(0, args[0].LastIndexOf('.')) + ".mod";
                BinaryWriter write = new BinaryWriter(new FileStream(end, FileMode.Create));

                for (int i = 0; i < indices.Count; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        vertices.Add(new float[][] { positions[(int)(indices[i][j * 3] - 1)], new float[]{1,1,1,1}, normals[(int)(indices[i][j * 3 + 2] - 1)], uvs[(int)(indices[i][j * 3 + 1] - 1)] });
                        
                    }
                    actualIndices.Add(new int[] { i * 3, i * 3 + 1, i * 3 + 2 });
                }

                removeDupes();

                calculateTangents();

                write.Write((uint)vertices.Count);
                write.Write((uint)actualIndices.Count * 3);

                for (int i = 0; i < actualIndices.Count; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        write.Write((ushort)(actualIndices[i][j]));
                    }
                }

                for (int i = 0; i < vertices.Count; i++)
                {
                    for (int j = 0; j < vertices[i].Length; j++)
                    {
                        WriteArray(vertices[i][j], write);
                    }
                }

                //write.Write((uint)indices.Count * 3);
                //write.Write((uint)indices.Count * 3);

                //for (int i = 0; i < indices.Count; i++)
                //{
                //    for (int j = 0; j < 3; j++)
                //    {
                //        if (3 * i + j > int.MaxValue)
                //        {
                //            int start = 0;
                //        }
                //        write.Write((uint)(3 * i + j));
                //    }
                //}

                //foreach (var face in indices)
                //{
                //    float[] color = { 1, 1, 1, 1 };
                //    for (int i = 0; i < 3; i++)
                //    {
                //        //Console.Write("Position ");
                //        WriteArray(positions.ElementAt((int)face[i * 3] - 1), write);
                //        //Console.Write("Color(tangents, lol) ");
                //        WriteArray( color, write );//tangents.ElementAt((int)face[i * 3 + 2] - 1), write);
                //        //Console.Write("Normal ");
                //        WriteArray(normals.ElementAt((int)face[i * 3 + 2] - 1), write);
                //        //Console.Write("UV ");
                //        WriteArray(uvs.ElementAt((int)face[i * 3 + 1] - 1), write);
                //        //Console.WriteLine();
                //    }
                //}

                write.Close();

                Console.WriteLine("Output can be found at " + Path.GetFullPath(end));
            }
        }

        private static void removeDupes()
        {
            for (int i = 0; i < vertices.Count; i++)
            {
                for (int j = i + 1; j < vertices.Count; j++)
                {
                    if (equality(vertices[i][0], vertices[j][0]) && equality(vertices[i][1], vertices[j][1]) && equality(vertices[i][2], vertices[j][2]) && equality(vertices[i][3], vertices[j][3]))
                    {
                        for (int k = 0; k < actualIndices.Count; k++)
                        {
                            for (int l = 0; l < 3; l++)
                            {
                                if (actualIndices[k][l] == j)
                                {
                                    actualIndices[k][l] = i;
                                }

                                if (actualIndices[k][l] > j)
                                {
                                    actualIndices[k][l] -= 1;
                                }
                            }
                        }

                        vertices.RemoveAt(j);

                        j--;
                    }
                }
            }
        }

        private static bool equality(float[] v1, float[] v2)
        {
            float epsilon = 0.000000001f;

            bool result = true;

            for (int i = 0; i < v1.Length; i++)
            {
                if (Math.Abs(v1[i] - v2[i]) > epsilon)
                    result = false;
            }

            return result;
        }

        private static void calculateTangents()
        {
            //glm::vec3 *tan1 = new glm::vec3[shapeData->numVerts * 2];
            float[][] tan1 = new float[ vertices.Count ][];
            //glm::vec3 *tan2 = tan1 + shapeData->numVerts;
            float[][] tan2 = new float[vertices.Count][];

            for (int i = 0; i < vertices.Count; i++)
            {
                tan1[i] = new float[3];
                tan2[i] = new float[3];
            }
    
	        for (int a = 0; a < actualIndices.Count; a++)
            {
		        //long i1 = shapeData->indices[a*3+0];
                int i1 = (int)actualIndices[a][0];
                int i2 = (int)actualIndices[a][1];
                int i3 = (int)actualIndices[a][2];
        
		        //const Neumont::Vertex& v1 = shapeData->verts[i1];
                float[] vp1 = vertices[i1][0];
                float[] vp2 = vertices[i2][0];
                float[] vp3 = vertices[i3][0];

                float[] vu1 = vertices[i1][3];
                float[] vu2 = vertices[i2][3];
                float[] vu3 = vertices[i3][3];

                float[] vn1 = vertices[i1][2];
                float[] vn2 = vertices[i2][2];
                float[] vn3 = vertices[i3][2];
        
                /*const Point2D& w1 = texcoord[i1];
                const Point2D& w2 = texcoord[i2];
                const Point2D& w3 = texcoord[i3];*/

                int x = 0, y = 1, z = 2;
        
                //float x1 = v2.position.x - v1.position.x;
                float x1 = vp2[x] - vp1[x];
                float x2 = vp3[x] - vp1[x];
                float y1 = vp2[y] - vp1[y];
                float y2 = vp3[y] - vp1[y];
                float z1 = vp2[z] - vp1[z];
                float z2 = vp3[z] - vp1[z];
        
		        float s1 = vu2[x] - vu1[x];
                float s2 = vu3[x] - vu1[x];
                float t1 = vu2[y] - vu1[y];
                float t2 = vu3[y] - vu1[y];
        
                float r = 1.0F / (s1 * t2 - s2 * t1);
                float[] sdir = {(t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                        (t2 * z1 - t1 * z2) * r};
                float[] tdir = {(s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                        (s1 * z2 - s2 * z1) * r};
        
                for( int i = 0; i < 3; i++ )
                {
                    tan1[i1][i] += sdir[i];
                    tan1[i2][i] += sdir[i];
                    tan1[i3][i] += sdir[i];
        
                    tan2[i1][i] += tdir[i];
                    tan2[i2][i] += tdir[i];
                    tan2[i3][i] += tdir[i];
                }
            }

            for (int a = 0; a < vertices.Count; a++)
            {
                float[] n = vertices[a][2];
                float[] t = tan1[a];
        
                // Gram-Schmidt orthogonalize
		        //shapeData->verts[a].color = glm::vec4( glm::normalize(t - n * glm::dot(n, t)), 0 );
                float doty = dot( n, t );
                float[] stuff = normalize( new float[] { t[0] - n[0] * doty, t[1] - n[1] * doty, t[2] - n[2] * doty } );
        
                // Calculate handedness
		        //shapeData->verts[a].color.w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
                float handedness = (dot(cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;

                vertices[a][1][0] = stuff[0];
                vertices[a][1][1] = stuff[1];
                vertices[a][1][2] = stuff[2];

                vertices[a][1][3] = handedness;
            }
        }

        private static float dot(float[] v1, float[] v2)
        {
            float result = 0;
            for (int i = 0; i < Math.Min(v1.Length, v2.Length); i++)
            {
                result += v1[i] * v2[i];
            }
            return result;
        }

        private static float[] cross(float[] v1, float[] v2)
        {
            return new float[] { v1[1] * v2[2] - v1[2] * v2[1], v1[2]*v2[0] - v1[0]*v2[2], v1[0]*v2[1] - v1[1]*v2[0] };
        }

        private static float[] normalize(float[] v)
        {
            float magnitude = 0;
            for (int i = 0; i < v.Length; i++)
            {
                magnitude += v[i] * v[i];
            }
            magnitude = (float)Math.Sqrt(magnitude);
            float[] normal = new float[v.Length];
            for (int i = 0; i < v.Length; i++)
            {
                normal[i] = v[i] / magnitude;
            }
            return normal;
        }

        private static void WriteArray(float[] arr, BinaryWriter write)
        {
            foreach (float num in arr)
            {
                //Console.Write(num + " ");
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

        static bool MatchSet(Action<float[]> func, string str, Regex reg)
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

                if( str.Contains( 'e' ) )
                {
                    //int test = 0;
                }

                func.Invoke(vals);
            }

            return match.Success;
        }
    }
}
