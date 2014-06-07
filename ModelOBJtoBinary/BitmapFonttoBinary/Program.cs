using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BitmapFonttoBinary
{
    class Character
    {
        public int id, x, y, width, height, xoff, yoff, advance;

        internal void write(BinaryWriter write)
        {
            write.Write((ushort)x);
            write.Write((ushort)y);
            write.Write((byte)width);
            write.Write((byte)height);
            write.Write((byte)(xoff+128));
            write.Write((byte)yoff);
            write.Write((byte)advance);
        }
    }

    class Kerning
    {
        public int first, second, amount;

        internal void write(BinaryWriter write, int minChar)
        {
            write.Write((byte)(first-minChar));
            write.Write((byte)(second-minChar));
            write.Write((byte)(amount+128));
        }
    }

    class Program
    {
        static int getValue(string containing, string key)
        {
            int index = containing.IndexOf('=', containing.IndexOf(key))+1;
            int endIndex = containing.IndexOf(' ', index);
            string val;
            if (endIndex==-1)
                val = containing.Substring(index);
            else
                val = containing.Substring(index, containing.IndexOf(' ', index) - index);
            return int.Parse(val);
        }
        static void Main(string[] args)
        {
            if (args.Length > 0)
            {
                StreamReader read = new StreamReader(args[0]);

                string line;

                line = read.ReadLine();
                line = read.ReadLine();

                int lineHeight = getValue(line, "lineHeight");
                int baseHeight = getValue(line, "base");
                int width = getValue(line, "scaleW");
                int height = getValue(line, "scaleH");

                line = read.ReadLine();
                line = read.ReadLine();

                int chars = getValue(line, "count" );

                int minChar = int.MaxValue;
                int maxChar = int.MinValue;

                List<Character> characters = new List<Character>();

                for (int i = 0; i < chars; i++)
                {
                    line = read.ReadLine();
                    Character character = new Character();
                    character.id = getValue(line, "id");
                    minChar = Math.Min(minChar, character.id);
                    maxChar = Math.Max(maxChar, character.id);

                    character.x = getValue(line, "x");
                    character.y = getValue(line, "y");
                    character.width = getValue(line, "width");
                    character.height = getValue(line, "height");
                    character.xoff = getValue(line, "xoffset");
                    character.yoff = getValue(line, "yoffset");
                    character.advance = getValue(line, "xadvance");
                    characters.Add(character);
                }

                line = read.ReadLine();
                int kerns = getValue(line, "count");

                List<Kerning> kernings = new List<Kerning>();

                for (int i = 0; i < kerns; i++)
                {
                    line = read.ReadLine();
                    Kerning kerning = new Kerning();
                    kerning.first = getValue(line, "first");
                    kerning.second = getValue(line, "second");
                    kerning.amount = getValue(line, "amount");
                    kernings.Add(kerning);
                }

                string end = args[0].Substring(0, args[0].LastIndexOf('.')) + ".bfnt";
                BinaryWriter write = new BinaryWriter(new FileStream(end, FileMode.Create));

                write.Write((byte)lineHeight);
                write.Write((byte)baseHeight);
                write.Write((ushort)width);
                write.Write((ushort)height);
                write.Write((byte)minChar);
                write.Write((byte)maxChar);
                write.Write((byte)chars);
                write.Write((byte)kerns);

                byte[] actualPos = new byte[maxChar+1-minChar];

                for (int i = 0; i < characters.Count; i++)
                {
                    actualPos[characters[i].id - minChar] = (byte)(i);
                }

                foreach (byte byt in actualPos)
                {
                    write.Write(byt);
                }

                foreach (Character chara in characters)
                {
                    chara.write(write);
                }

                foreach (Kerning ker in kernings)
                {
                    ker.write(write, minChar);
                }

                write.Close();
            }
            //Console.In.ReadLine();
        }
    }
}
